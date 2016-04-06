#pragma once
#include "CommonInclude.h"
#include "Thread.h"

class CNetWorkServer : public Thread
{
public:
	CRITICAL_SECTION cs;//本地线程锁
	CNetWorkServer(void* pParam);
	~CNetWorkServer();

	//请求类型，100：启动CameraInfo指定的相机，101：关闭CameraInfo指定的相机，102：请求发送CameraInfo指定的相机最新数据，103：添加CameraInfo指定的相机
	//104：修改CameraInfo相机号指定的相机信息

	bool WatchingNetwork();

	bool AddNewCam(Tmtv_CameraInfo *camInfo);
	bool CloseCam(int CamIndex);
	bool OpenCam(int CamIndex);



private:
	void Task(void);
	bool SendMsg(size_t msglength);

private:
	void *pParent;
	TmtSocket mNetWork;
	char*pRcData;
	char*pSdData;
};

