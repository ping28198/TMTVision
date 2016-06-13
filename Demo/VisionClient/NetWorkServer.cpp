
#pragma once
#include "stdafx.h"
#include "NetWorkServer.h"
#include "VisionClientDlg.h"

CNetWorkServer::CNetWorkServer(void* pParam)
{
	pRcData = new char[40960];
	pSdData = new char[40960];
	pParent = pParam;
	mNetWork.SetOption(TmtSocket::ADDR_REUSE);
	NetIP mIp;
	TmtSocket::GetAvailableNetIP(mIp);
	bool a = mNetWork.SetSendAddr(5620, mIp,5007,NULL);//5610 127.0.0.1 5620 222.28.39.16 5621 222.28.39.16
	if (a==true)
	{
		VisionPublicSet::mLogger.TraceInfo("����socket��ַ���óɹ�,ip��ַΪ��%s",mIp);
	}
	bool b = mNetWork.SetRecvAddr(5610, mIp);
	if (b == true)
	{
		VisionPublicSet::mLogger.TraceInfo("����socket��ַ���óɹ�,ip��ַΪ��%s", mIp);
	}
	
	InitializeCriticalSection(&cs);
	//����ʱ��������

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
	VisionPublicSet::mLogger.TraceInfo("���������Ϣ�߳�");
	int recvnum = mNetWork.RecvMsg(pRcData, 40960);
	VisionPublicSet::mLogger.TraceInfo("�յ���Ϣ");
	CVisionClientDlg *pDlg = (CVisionClientDlg*)pParent;
	if (recvnum == sizeof(Tmtv_MsgInfo))
	{
		Tmtv_MsgInfo* pInfo = (Tmtv_MsgInfo*)pRcData;
		if (pInfo->CheckCode != TMTV_CHECKCODE) return false;
		if (pInfo->MsgType == Tmtv_MsgInfo::TMTV_SNAPED)
		{
			VisionPublicSet::mLogger.TraceInfo("������µ�ͼƬ");
			pDlg->pRunState->AddNewImg(&(pInfo->ImgInfo));
		}
		else
		{
			Tmtv_MsgInfo* pPstMsg = new Tmtv_MsgInfo;
			memcpy(pPstMsg, pInfo, sizeof(Tmtv_MsgInfo));
			HWND hParent = ((CVisionClientDlg*)pParent)->GetSafeHwnd();
			::PostMessage(hParent, WM_ASK_RESULT, 0, (LPARAM)pPstMsg);
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
	memcpy_s(pSdData, 40960, (void *)&AskInfo, sizeof(Tmtv_AskInfo));
	bool b = this->SendMsg(sizeof(Tmtv_AskInfo));

	AskInfo.Asktype = Tmtv_AskInfo::TMTV_STARTCAM;
	memcpy_s(pSdData, 40960, (void *)&AskInfo, sizeof(Tmtv_AskInfo));
	b = this->SendMsg(sizeof(Tmtv_AskInfo));

	return b;
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
	int sendcount = mNetWork.SendMsg(pSdData, msglength);//sendto(sock_send, (char*)pSdData, msglength, 0, (SOCKADDR*)&recvaddr, sizeof(recvaddr));
	if (sendcount == msglength)
	{
		VisionPublicSet::mLogger.TraceInfo("����������Ϣ�ɹ���");
		return true;
	}
	return false;
}

