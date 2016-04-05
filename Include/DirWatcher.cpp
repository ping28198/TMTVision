#include "DirWatcher.h"
#include "CommonFunc.h"

//创建文件夹读取句柄, 初始化ReadDirectoryChangesW相关参数
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
//执行前强制停止线程, 停止监控文件夹
void DirWatcher::FreePath()
{
	if (m_hDir != 0)
	{
		CloseHandle(m_hDir);
		m_hDir = 0;
	}
	m_path[0] = 0;
}
//供重载的操作函数,主函数会重复执行该函数
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
		//设置类型过滤器,监听文件创建、更改、删除、重命名等
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


//创建文件夹读取句柄, 初始化ReadDirectoryChangesW相关参数
//执行前强制停止线程, 需要用Create()再次启动
bool DirWatchServer::RegPath(LONGWSTR path, DWORD action)
{
	EnterCriticalSection(&m_section);
	m_fileNameQueue.Clear();
	Thread::ForceEnd();
	return DirWatcher::RegPath(path, action);
	LeaveCriticalSection(&m_section);
}
//执行前强制停止线程, 停止监控文件夹
void DirWatchServer::FreePath()
{
	EnterCriticalSection(&m_section);
	Thread::ForceEnd();
	DirWatcher::FreePath();
	m_fileNameQueue.Clear();
	LeaveCriticalSection(&m_section);
}
//先扫描注册文件夹所有文件, 文件名推入队列中保留最近的文件
//再重载启动函数,内部添加FindFirstChangeNotification用于启动监控
void DirWatchServer::Create(int times, long waiteTime, bool includeTaskTime)
{
	if (m_path[0] == 0 || m_hDir == 0)
	{
		OutputDebugString(L"<DirWatchServer: Create() failed.>\n");
		return;
	}
    Thread::Create(times, waiteTime, includeTaskTime);
}
//重载启动函数
void DirWatchServer::Destroy()
{
	FreePath();
}
//供重载的操作函数,主函数会重复执行该函数,内部调用Watch()函数,具有线程保护
void DirWatchServer::Task()
{	
	FileItem tmpFileItem;
	tmpFileItem.m_fileName[0] = 0;
	tmpFileItem.m_fileAction = 0;
	tmpFileItem.m_fileProcessed = false;
	EnterCriticalSection(&m_section);
	DirWatcher::Watch(tmpFileItem.m_fileName, tmpFileItem.m_fileAction);
	//m_fileNameQueue.ForcTail(tmpFileItem);
	m_fileNameQueue.AddTail(tmpFileItem);
	LeaveCriticalSection(&m_section);
}
