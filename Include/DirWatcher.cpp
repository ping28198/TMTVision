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
	CCommonFunc::SafeStringCpy(m_path, STR_LEN(m_path), path);
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
			CCommonFunc::SafeStringCpy(fileName, STR_LEN(fileName), pnotify->FileName);
			action = FILE_ACTION_ADDED;
			OutputDebugString(L"<DirWatcher::Watch() FILE_ACTION_ADDED.>\n");
			return true;
			break;
		case FILE_ACTION_REMOVED:
			CCommonFunc::SafeStringCpy(fileName, STR_LEN(fileName), pnotify->FileName);
			action = FILE_ACTION_REMOVED;
			OutputDebugString(L"<DirWatcher::Watch() FILE_ACTION_REMOVED.>\n");
			return true;
			break;
		case FILE_ACTION_MODIFIED:
			CCommonFunc::SafeStringCpy(fileName, STR_LEN(fileName), pnotify->FileName);
			action = FILE_ACTION_MODIFIED;
			OutputDebugString(L"<DirWatcher::Watch() FILE_ACTION_MODIFIED.>\n");
			return true;
			break;	
		case FILE_ACTION_RENAMED_OLD_NAME:
			CCommonFunc::SafeStringCpy(fileName, STR_LEN(fileName), pnotify->FileName);
			action = FILE_ACTION_RENAMED_OLD_NAME;
			OutputDebugString(L"<DirWatcher::Watch() FILE_ACTION_RENAMED_OLD_NAME.>\n");
			return true;
			break;
		case FILE_ACTION_RENAMED_NEW_NAME:
			CCommonFunc::SafeStringCpy(fileName, STR_LEN(fileName), pnotify->FileName);
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
	return DirWatcher::RegPath(path, action);
	LeaveCriticalSection(&m_section);
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
void DirWatchServer::Create(int times, long waiteTime)
{
	if (m_path[0] == 0 || m_hDir == 0)
	{
		OutputDebugString(L"<DirWatchServer: Create() failed.>\n");
		return;
	}
    Thread::Create(times, waiteTime);
}
//������������
void DirWatchServer::Destroy()
{
	FreePath();
}
//�����صĲ�������,���������ظ�ִ�иú���,�ڲ�����Watch()����,�����̱߳���
void DirWatchServer::Task()
{	
	FileItem tmpFileItem;
	tmpFileItem.m_fileName[0] = 0;
	tmpFileItem.m_fileAction = 0;
	tmpFileItem.m_fileProcessed = false;
	EnterCriticalSection(&m_section);
	DirWatcher::Watch(tmpFileItem.m_fileName, tmpFileItem.m_fileAction);
	m_fileNameQueue.ForcTail(tmpFileItem);
	LeaveCriticalSection(&m_section);
}
