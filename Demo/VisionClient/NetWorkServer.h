#pragma once
#include "CommonInclude.h"
#include "Thread.h"

class CNetWorkServer : public Thread
{
public:
	CRITICAL_SECTION cs;//�����߳���
	CNetWorkServer(void* pParam);
	~CNetWorkServer();

	//�������ͣ�100������CameraInfoָ���������101���ر�CameraInfoָ���������102��������CameraInfoָ��������������ݣ�103�����CameraInfoָ�������
	//104���޸�CameraInfo�����ָ���������Ϣ

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

