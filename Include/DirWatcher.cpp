#include "DirWatcher.h"
#include "CommonFunc.h"

//�����ļ��ж�ȡ���, ��ʼ��ReadDirectoryChangesW��ز���
bool DirWatcher::RegPath(LONGWSTR path, DWORD action)
{
	HANDLE hDir = ::CreateFileW(path,
		GENERIC_READ | FILE_LIST_DIRECTORY,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);
	if (INVALID_HANDLE_VALUE == hDir)
	{
		return false;
		OutputDebugString(L"<DirWatcher::RegPath() failed.>\n");
	}
	m_hDir = hDir;
	m_action = action;
	CCommonFunc::SafeWStringCpy(m_path, STR_LEN(m_path), path);
	return true;
}
//ִ��ǰǿ��ֹͣ�߳�, ֹͣ����ļ���
void DirWatcher::FreePath()
{
	if (m_hDir != 0)
	{
		CloseHandle(m_hDir);
		m_hDir = 0;
	}
	m_path[0] = 0;
}
//�����صĲ�������,���������ظ�ִ�иú���
bool DirWatcher::Watch(LONGWSTR& fileName, DWORD& action)
{
	if (m_hDir == 0)
	{
		fileName[0] = 0;
		action = 0;
		OutputDebugString(L"<DirWatcher::Watch() failed.>\n");
		return false;
	}
	DWORD cbBytes;
	DWORD notify[NOTIFYSIZE] = { 0 };
	FILE_NOTIFY_INFORMATION *pnotify = (FILE_NOTIFY_INFORMATION*)notify;
	if (ReadDirectoryChangesW(m_hDir,
		&notify, NOTIFYSIZE, // Buffer and size
		true, // monitor children?
		m_action,
		&cbBytes, NULL, NULL))
	{
		if (cbBytes <= 0)
		{
			OutputDebugString(L"<DirWatcher::Watch() Buffer overflow.>\n");
			return true;
		}
		//�������͹�����,�����ļ����������ġ�ɾ������������
		switch (pnotify->Action)
		{
		case FILE_ACTION_ADDED:
			CCommonFunc::SafeWStringCpy(fileName, STR_LEN(fileName), pnotify->FileName);
			action = FILE_ACTION_ADDED;
			OutputDebugString(L"<DirWatcher::Watch() FILE_ACTION_ADDED.>\n");
			return true;
			break;
		case FILE_ACTION_REMOVED:
			CCommonFunc::SafeWStringCpy(fileName, STR_LEN(fileName), pnotify->FileName);
			action = FILE_ACTION_REMOVED;
			OutputDebugString(L"<DirWatcher::Watch() FILE_ACTION_REMOVED.>\n");
			return true;
			break;
		case FILE_ACTION_MODIFIED:
			CCommonFunc::SafeWStringCpy(fileName, STR_LEN(fileName), pnotify->FileName);
			action = FILE_ACTION_MODIFIED;
			OutputDebugString(L"<DirWatcher::Watch() FILE_ACTION_MODIFIED.>\n");
			return true;
			break;	
		case FILE_ACTION_RENAMED_OLD_NAME:
			CCommonFunc::SafeWStringCpy(fileName, STR_LEN(fileName), pnotify->FileName);
			action = FILE_ACTION_RENAMED_OLD_NAME;
			OutputDebugString(L"<DirWatcher::Watch() FILE_ACTION_RENAMED_OLD_NAME.>\n");
			return true;
			break;
		case FILE_ACTION_RENAMED_NEW_NAME:
			CCommonFunc::SafeWStringCpy(fileName, STR_LEN(fileName), pnotify->FileName);
			action = FILE_ACTION_RENAMED_NEW_NAME;
			return true;
			break;
		default:
			fileName[0] = 0;
			action = 0;
			OutputDebugString(L"<DirWatcher::Watch() Unknow action.>\n");
			return false;
			break;
		}
	}
	fileName[0] = 0;
	action = 0;
	OutputDebugString(L"<DirWatcher::Watch() No action.>\n");
	return false;
}


//�����ļ��ж�ȡ���, ��ʼ��ReadDirectoryChangesW��ز���
//ִ��ǰǿ��ֹͣ�߳�, ��Ҫ��Create()�ٴ�����
bool DirWatchServer::RegPath(LONGWSTR path, DWORD action)
{
	EnterCriticalSection(&m_section);
	m_fileNameQueue.Clear();
	Thread::ForceEnd();
	int rtVal= DirWatcher::RegPath(path, action);
	LeaveCriticalSection(&m_section);
	return rtVal;
}
//ִ��ǰǿ��ֹͣ�߳�, ֹͣ����ļ���
void DirWatchServer::FreePath()
{
	EnterCriticalSection(&m_section);
	Thread::ForceEnd();
	DirWatcher::FreePath();
	m_fileNameQueue.Clear();
	LeaveCriticalSection(&m_section);
}
//��ɨ��ע���ļ��������ļ�, �ļ�����������б���������ļ�
//��������������,�ڲ����FindFirstChangeNotification�����������
void DirWatchServer::Create(int times, long waiteTime, bool includeTaskTime)
{
	if (m_path[0] == 0 || m_hDir == 0)
	{
		OutputDebugString(L"<DirWatchServer: Create() failed.>\n");
		return;
	}
    Thread::Create(times, waiteTime, includeTaskTime);
}
//������������
void DirWatchServer::Destroy()
{
	FreePath();
}
//�����صĲ�������,���������ظ�ִ�иú���,�ڲ�����Watch()����,�����̱߳���
void DirWatchServer::Task()
{	
	FileItem tmpFileItem, tmpFileItem2;
	tmpFileItem.m_fileName[0] = 0;
	tmpFileItem.m_fileAction = 0;
	tmpFileItem.m_fileProcessed = false;
	EnterCriticalSection(&m_section);
	if (DirWatcher::Watch(tmpFileItem.m_fileName, tmpFileItem.m_fileAction))
	{
		if (m_fileNameQueue.GetLength() > 0)
		{
			bool isNew = true;
			for (int i = m_fileNameQueue.GetLength() - 1; i >= 0; i--)
			{
				if (wcscmp(tmpFileItem.m_fileName,
					m_fileNameQueue.GetData(i)->m_fileName)==0)
				{
					isNew = false;
					break;
				}
			}
			if (isNew)
			{
				m_fileNameQueue.AddTail(tmpFileItem);
			}
		}
		else
		{
			m_fileNameQueue.AddTail(tmpFileItem);
		}
	}
	LeaveCriticalSection(&m_section);
}

//���Ժ���,��ʾ������Ϣ
//�ֱ�����ɫ��Χ: 40--49        ����ɫ: 30--39
//	40 : ��                           30 : ��
//	41 : ��                           31 : ��
//	42 : ��                           32 : ��
//	43 : ��                           33 : ��
//	44 : ��                           34 : ��
//	45 : ��                           35 : ��
//	46 : ����                         36 : ����
//	47 : ��ɫ                         37 : ��ɫ
void DirWatchServer::ToString(HUGEWSTR & string, int method, int color)
{
	string[0] = 0;
	if (method==0)
	{
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<DirWatchServer m_path=\"%s\">\n", m_path);
		for (int i = m_fileNameQueue.GetLength() - 1; i >= 0; i--)
		{
			CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s  <m_fileNameQueue(%d)=\"%s\" %d %d>\n",
				string,
				i,
				m_fileNameQueue.GetData(i)->m_fileName,
				m_fileNameQueue.GetData(i)->m_fileAction, 
				m_fileNameQueue.GetData(i)->m_fileProcessed);
		}
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s</DirWatchServer>\n", string);
	}
	else if (method == 1)
	{
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<DirWatchServer m_path=\"%s\" Num=%d>\n",
			m_path, m_fileNameQueue.GetLength());
	}
	else if (method == 2)
	{
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<DirWatchServer m_path=\"%s\" Num=%d>\n", 
			m_path, m_fileNameQueue.GetLength());
		int i = m_fileNameQueue.GetLength() - 1;
		if(!m_fileNameQueue.IsEmpty())
		{
			CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s  <m_fileNameQueue(%d)=\"%s\" %d %d>\n",
				string,
				i,
				m_fileNameQueue.GetTail()->m_fileName,
				m_fileNameQueue.GetData(i)->m_fileAction, 
				m_fileNameQueue.GetTail()->m_fileProcessed);
		}
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s</DirWatchServer>\n", string);
	}
//#define COL(x)  L"\033[;" #x L"m"  
//#define RED     COL(31)  
//#define GREEN   COL(32)  
//#define YELLOW  COL(33)  
//#define BLUE    COL(34)  
//#define MAGENTA COL(35)  
//#define CYAN    COL(36)  
//#define WHITE   COL(0)
//#define GRAY    L"\033[0m" 
	if (color >= 30 && color <= 39)
	{
		HUGEWSTR testString = { 0 };
		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, string);
		CCommonFunc::SafeWStringCpy(string, TMTV_HUGESTRLEN, testString);
	}
}
