#include "stdafx.h"
#include "CameraServer.h"

CameraServer::CameraServer(HANDLE hParent)
{
	pDirWatchServer = 0;
}

CameraServer::~CameraServer()
{
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Destroy();
		delete pDirWatchServer;
		pDirWatchServer = 0;
	}
}
//创建线程
void CameraServer::Create(int times, long waiteTime, bool includeTaskTime)
{
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Create(-1,0,false);
	}
	Thread::Create(-1, waiteTime, includeTaskTime);
}
//设置参数使主函数退出以销毁线程
void  CameraServer::Destroy(void)
{
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Destroy();
		delete pDirWatchServer;
		pDirWatchServer = 0;
	}
	Thread::Destroy();
}
void CameraServer::Task()
{

}