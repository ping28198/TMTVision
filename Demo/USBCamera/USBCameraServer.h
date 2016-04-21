#pragma once
#include "Thread.h"
#include "Queue.h"
#include "VisionStruct.h"
#include "CommonDefine.h"

#define WM_CAPTUREDNOTIFY (WM_USER+11)
class USBCameraServer :
	public Thread
{
public:
	USBCameraServer();
	~USBCameraServer();

	Tmtv_CameraInfo m_CameraInfo;
	Queue<CImage> m_CImageList;
	void InitialCamera(Tmtv_CameraInfo cameraInfo);
	void UnitialCamera();
	void SetCamera(Tmtv_CameraInfo cameraInfo)
	{
		UnitialCamera();
		InitialCamera(cameraInfo);
	}
	void Create()
	{
		Thread::Create(-1, MIN(0, m_CameraInfo.WaiteTime), true);
	}
	void Task();
};

