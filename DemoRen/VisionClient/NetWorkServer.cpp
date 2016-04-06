
#pragma once
#include "stdafx.h"
#include "NetWorkServer.h"
#include "VisionClientDlg.h"

CNetWorkServer::CNetWorkServer(void* pParam)
{
	pRcData = new char[40960];
	pSdData = new char[40960];

	mNetWork.SetSendAddr(3307,"127.0.0.1");
	mNetWork.SetRecvAddr(3308);

	InitializeCriticalSection(&cs);
	//创建时启动进程

	Create();
	Suspend();
}

CNetWorkServer::~CNetWorkServer()
{
	DeleteCriticalSection(&cs);
	Suspend();
	Destroy();
	delete[] pRcData;
	delete[] pSdData;
}


bool CNetWorkServer::WatchingNetwork()
{
	VisionPublicSet::mLogger.TraceInfo("进入接收消息线程");
	int recvnum = mNetWork.RecvMsg(pRcData, 40960);
	VisionPublicSet::mLogger.TraceInfo("收到消息");
	CVisionClientDlg *pDlg = (CVisionClientDlg*)pParent;
	if (recvnum>=sizeof(Tmtv_MsgInfo))
	{
		Tmtv_MsgInfo* pInfo = (Tmtv_MsgInfo*)pRcData;
		if (pInfo->CheckCode != TMTV_CHECKCODE) return false;
		if (pInfo->MsgType == Tmtv_MsgInfo::TMTV_SNAPED)
		{
			VisionPublicSet::mLogger.TraceInfo("添加新的图片");
			pDlg->pRunState->AddNewImg(&(pInfo->ImgInfo));
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool CNetWorkServer::AddNewCam(Tmtv_CameraInfo *camInfo)
{
	Tmtv_AskInfo AskInfo;
	AskInfo.Asktype = Tmtv_AskInfo::TMTV_ADDCAM;
	AskInfo.CameraInfo = *camInfo;
	this->SendMsg(sizeof(Tmtv_AskInfo));
	return true;
}

bool CNetWorkServer::CloseCam(int CamIndex)
{
	Tmtv_AskInfo AskInfo;
	AskInfo.CameraInfo.Indexnum = CamIndex;
	AskInfo.Asktype = Tmtv_AskInfo::TMTV_STOPCAM;
	this->SendMsg(sizeof(Tmtv_AskInfo));
	return true;
}

bool CNetWorkServer::OpenCam(int CamIndex)
{
	Tmtv_AskInfo AskInfo;
	AskInfo.CameraInfo.Indexnum = CamIndex;
	AskInfo.Asktype = Tmtv_AskInfo::TMTV_STARTCAM;
	this->SendMsg(sizeof(Tmtv_AskInfo));
	return true;
}

void CNetWorkServer::Task(void)
{
	this->WatchingNetwork();
}

bool CNetWorkServer::SendMsg(size_t msglength)
{
	int sendcount = mNetWork.SendMsg(pSdData, 40960);//sendto(sock_send, (char*)pSdData, msglength, 0, (SOCKADDR*)&recvaddr, sizeof(recvaddr));
	if (sendcount>=msglength)
	{
		VisionPublicSet::mLogger.TraceInfo("发送请求消息成功！");
		return true;
	}
	return false;
}

