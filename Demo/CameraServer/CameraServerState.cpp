#include "stdafx.h"
#include "CameraServerState.h"

CameraServerState::CameraServerState()
{
	pCameraInfo=0;
	pDirWatcher=0;
	cameraNum=0;
}


CameraServerState::~CameraServerState()
{
	if (pCameraInfo!=0)
	{
		delete[]pCameraInfo;
	}	
	if (pDirWatcher != 0)
	{
		delete[]pDirWatcher;
	}
	cameraNum = 0;
}

bool CameraServerState::LoadSetting(LONGSTR xmlFilePath)
{
	return false;
}

bool CameraServerState::SaveSetting(LONGSTR xmlFilePath)
{
	return false;
}

bool CameraServerState::AddCamera(Tmtv_CameraInfo cameraInfo)
{
	return false;
}

bool CameraServerState::StartCamera(Tmtv_CameraInfo cameraInfo)
{
	return false;
}

bool CameraServerState::StartAlgorithm(Tmtv_CameraInfo cameraInfo)
{
	return false;
}

bool CameraServerState::StopAlgorithm(Tmtv_CameraInfo cameraInfo)
{
	return false;
}

bool CameraServerState::StopCamera(Tmtv_CameraInfo cameraInfo)
{
	return false;
}

bool CameraServerState::DelCamera(Tmtv_CameraInfo cameraInfo)
{
	return false;
}

bool CameraServerState::RevMsg(Tmtv_MsgInfo msgInfo)
{
	return false;
}
