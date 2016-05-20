#include "DirWatcher.h"
#include "CommonFunc.h"

//�����ļ��ж�ȡ���, ��ʼ��ReadDirectoryChangesW��ز���
bool DirWatcher::RegPath(PATHWSTR path, DWORD action)
{
	int pathLen = CCommonFunc::GetStringLen(path, TMTV_PATHSTRLEN);
	if (pathLen < 1) return false;
	if (path[pathLen-1] != L'\\')
	{
		path[pathLen] = L'\\';
		if(pathLen+1<TMTV_PATHSTRLEN) path[pathLen + 1] = 0;
	}
	if (!CCommonFunc::DirExist(path))
	{
		return false;
	}
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
bool DirWatcher::Watch(PATHWSTR& fileName, TINYWSTR& fileTime, DWORD& action)
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
		fileName[0] = 0;
		//�������͹�����,�����ļ����������ġ�ɾ������������
		switch (pnotify->Action)
		{
		case FILE_ACTION_ADDED:
			CCommonFunc::SafeWStringPrintf(fileName, STR_LEN(fileName),L"%s%s",m_path,pnotify->FileName);
			action = FILE_ACTION_ADDED;
			CCommonFunc::GetFileTime(fileName, fileTime, TMTV_TINYSTRLEN, 0,0,0,0, false);
			OutputDebugString(L"<DirWatcher::Watch() FILE_ACTION_ADDED.>\n");
			return true;
			break;
		case FILE_ACTION_REMOVED:
			CCommonFunc::SafeWStringPrintf(fileName, STR_LEN(fileName), L"%s%s", m_path, pnotify->FileName);
			action = FILE_ACTION_REMOVED;
			CCommonFunc::GetFileTime(fileName, fileTime, TMTV_TINYSTRLEN, 0, 0, 0, 0, false);
			OutputDebugString(L"<DirWatcher::Watch() FILE_ACTION_REMOVED.>\n");
			return true;
			break;
		case FILE_ACTION_MODIFIED:
			CCommonFunc::SafeWStringPrintf(fileName, STR_LEN(fileName), L"%s%s", m_path, pnotify->FileName);
			action = FILE_ACTION_MODIFIED;
			CCommonFunc::GetFileTime(fileName, fileTime, TMTV_TINYSTRLEN, 0, 0, 0, 0, false);
			OutputDebugString(L"<DirWatcher::Watch() FILE_ACTION_MODIFIED.>\n");
			return true;
			break;	
		case FILE_ACTION_RENAMED_OLD_NAME:
			CCommonFunc::SafeWStringPrintf(fileName, STR_LEN(fileName), L"%s%s", m_path, pnotify->FileName);
			action = FILE_ACTION_RENAMED_OLD_NAME;
			CCommonFunc::GetFileTime(fileName, fileTime, TMTV_TINYSTRLEN, 0, 0, 0, 0, false);
			OutputDebugString(L"<DirWatcher::Watch() FILE_ACTION_RENAMED_OLD_NAME.>\n");
			return true;
			break;
		case FILE_ACTION_RENAMED_NEW_NAME:
			CCommonFunc::SafeWStringPrintf(fileName, STR_LEN(fileName), L"%s%s", m_path, pnotify->FileName);
			action = FILE_ACTION_RENAMED_NEW_NAME;
			CCommonFunc::GetFileTime(fileName, fileTime, TMTV_TINYSTRLEN, 0, 0, 0, 0, false);
			OutputDebugString(L"<DirWatcher::Watch() FILE_ACTION_RENAMED_NEW_NAME.>\n");
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
//
////���Ժ���,��ʾ������Ϣ
////�ֱ�����ɫ��Χ: 40--49        ����ɫ: 30--39
////	40 : ��                           30 : ��
////	41 : ��                           31 : ��
////	42 : ��                           32 : ��
////	43 : ��                           33 : ��
////	44 : ��                           34 : ��
////	45 : ��                           35 : ��
////	46 : ����                         36 : ����
////	47 : ��ɫ                         37 : ��ɫ
//void DirWatcher::ToString(MEGAWSTR & string, int method, int color)
//{
//	string[0] = 0;
//	if (1)
//	{
//		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<DirWatch m_path=\"%s\">\n", m_path);
//	}
//	if (color >= 30 && color <= 39)
//	{
//		MEGAWSTR testString = { 0 };
//		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, string);
//		CCommonFunc::SafeWStringCpy(string, TMTV_HUGESTRLEN, testString);
//	}
//}




//�����ļ��ж�ȡ���, ��ʼ��ReadDirectoryChangesW��ز���
//ִ��ǰǿ��ֹͣ�߳�, ��Ҫ��Create()�ٴ�����
bool DirWatchServer::RegPath(PATHWSTR path, DWORD action)
{
	Thread::ForceEnd();
	EnterCriticalSection(&m_section);
	m_fileNameQueue.Clear();
	int rtVal= DirWatcher::RegPath(path, action);
	LeaveCriticalSection(&m_section);
	return rtVal; 
}
//ִ��ǰǿ��ֹͣ�߳�, ֹͣ����ļ���
void DirWatchServer::FreePath()
{	
	Thread::ForceEnd();
	EnterCriticalSection(&m_section);
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
	if (m_path[0] == 0 || m_hDir == 0)
	{
		return;
	}
	FileItem tmpFileItem;
	tmpFileItem.m_fileName[0] = 0;
	tmpFileItem.m_fileAction = 0;
	tmpFileItem.m_fileProcessed = false;
	
	if (DirWatcher::Watch(tmpFileItem.m_fileName, 
		tmpFileItem.m_fileTime, tmpFileItem.m_fileAction))
	{
		EnterCriticalSection(&m_section);
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
		LeaveCriticalSection(&m_section);
	}
}
//
////���Ժ���,��ʾ������Ϣ
////�ֱ�����ɫ��Χ: 40--49        ����ɫ: 30--39
////	40 : ��                           30 : ��
////	41 : ��                           31 : ��
////	42 : ��                           32 : ��
////	43 : ��                           33 : ��
////	44 : ��                           34 : ��
////	45 : ��                           35 : ��
////	46 : ����                         36 : ����
////	47 : ��ɫ                         37 : ��ɫ
//void DirWatchServer::ToString(MEGAWSTR & string, int method, int color)
//{
//	string[0] = 0;
//	if (method==0)
//	{
//		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<DirWatchServer m_path=\"%s\">\n", m_path);
//		for (int i = m_fileNameQueue.GetLength() - 1; i >= 0; i--)
//		{
//			CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s  <m_fileNameQueue(%d)=\"%s\" %d %d>\n",
//				string,
//				i,
//				m_fileNameQueue.GetData(i)->m_fileName,
//				m_fileNameQueue.GetData(i)->m_fileAction, 
//				m_fileNameQueue.GetData(i)->m_fileProcessed);
//		}
//		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s</DirWatchServer>\n", string);
//	}
//	else if (method == 1)
//	{
//		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<DirWatchServer m_path=\"%s\" Num=%d>\n",
//			m_path, m_fileNameQueue.GetLength());
//	}
//	else if (method == 2)
//	{
//		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<DirWatchServer m_path=\"%s\" Num=%d>\n", 
//			m_path, m_fileNameQueue.GetLength());
//		int i = m_fileNameQueue.GetLength() - 1;
//		if(!m_fileNameQueue.IsEmpty())
//		{
//			CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s  <m_fileNameQueue(%d)=\"%s\" %d %d>\n",
//				string,
//				i,
//				m_fileNameQueue.GetTail()->m_fileName,
//				m_fileNameQueue.GetData(i)->m_fileAction, 
//				m_fileNameQueue.GetTail()->m_fileProcessed);
//		}
//		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s</DirWatchServer>\n", string);
//	}
//	if (color >= 30 && color <= 39)
//	{
//		MEGAWSTR testString = { 0 };
//		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, string);
//		CCommonFunc::SafeWStringCpy(string, TMTV_HUGESTRLEN, testString);
//	}
//}
