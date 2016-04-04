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
//�����߳�
void CameraServer::Create(int times, long waiteTime, bool includeTaskTime)
{
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Create(-1,0,false);
	}
	Thread::Create(-1, waiteTime, includeTaskTime);
}
//���ò���ʹ�������˳��������߳�
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