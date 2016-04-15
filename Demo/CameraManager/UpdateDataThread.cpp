#include "UpdateDataThread.h"
#include "CameraManagerDlg.h"
CUpdateDataThread::CUpdateDataThread(void* pParent)
{
	this->pParent = (CCameraManagerDlg*)pParent;
	pCamManager = this->pParent->pCamManager;
}
CUpdateDataThread::~CUpdateDataThread()
{
	ForceEnd();
}

void CUpdateDataThread::Initial()
{
	if (pCamManager==NULL)
	{
		LoggerServer::mLogger.TraceError("��ȡCameraManagerָ��Ϊ�գ�");
		return;
	}
	m_NetCount = 0;
	Create(-1, 500);
}

void CUpdateDataThread::Task()
{
	CListCtrl *pList = &(pParent->pCamListDlg->m_CamList);
	pList->DeleteAllItems();
	vector<CameraObject*>::iterator it;
	int mindex = 0;
	Tmtv_CameraInfo mCamInfo;
	wchar_t wstr[512];
	for (it = pCamManager->m_CameraObjectVector.begin(); it != pCamManager->m_CameraObjectVector.end(); it++)
	{
		(*it)->GetCamInfo(&mCamInfo);
		wsprintf(wstr, L"%d", mCamInfo.Indexnum);
		int nRow = pList->InsertItem(mindex, wstr);
		switch (mCamInfo.Status)
		{
		case Tmtv_CameraInfo::TMTV_RUNNINGCAM:
			wsprintf(wstr, L"����");
			break;
		case Tmtv_CameraInfo::TMTV_STOPEDCAM:
			wsprintf(wstr, L"ֹͣ");
			break;
		default:
			wsprintf(wstr, L"δ֪");
			break;
		}
		pList->SetItemText(nRow, 1, wstr);
		switch (mCamInfo.AlgorithmInfo.WarnningLevel)
		{
		case Tmtv_AlgorithmInfo::TMTV_NOWARN :
			wsprintf(wstr, L"ֹͣ");
			break;
		case Tmtv_AlgorithmInfo::TMTV_PREWARN:
			wsprintf(wstr, L"����");
			break;
		case Tmtv_AlgorithmInfo::TMTV_STARTWARN:
			wsprintf(wstr, L"����");
			break;
		default:
			wsprintf(wstr, L"δ֪");
			break;
		}
		pList->SetItemText(nRow, 2, wstr);
		wsprintf(wstr, L"%d", mCamInfo.AlgorithmInfo.WarnningLevel);
		pList->SetItemText(nRow, 3, wstr);
		CCommonFunc::AnsiToUnicode(mCamInfo.CameraName, wstr, sizeof(wstr));
		pList->SetItemText(nRow, 4, wstr);
		CCommonFunc::AnsiToUnicode(mCamInfo.CameraPath, wstr, sizeof(wstr));
		pList->SetItemText(nRow, 5, wstr);
		mindex++;
	}
	//////////////////////////////////////////////////////////////////////////
	//����״̬
	CListBox* pListBox = &(pParent->pNetWorkDlg->mNetStatusList);
	ReceiveServerSetting mRecvSetting;
	SendServerSetting mSendSetting;
	pCamManager->m_SendServer.GetSetting(mSendSetting);
	pCamManager->m_ReceiveServer.GetSetting(mRecvSetting);
	pListBox->ResetContent();
	pListBox->AddString(L"����״̬��");
	DWORD mstatus;
	mstatus = pCamManager->m_SendServer.GetSendServerStatus();
	if ((mstatus&TmtSocket::enSendOK) == TmtSocket::enSendOK)
	{
		wsprintf(wstr, L"  ����״̬������");
	}
	else
	{
		wsprintf(wstr, L"  ����״̬���쳣");
	}

	wsprintf(wstr, L"  ���ض˿ںţ�%d", mSendSetting.m_LocalSendPort);
	





	m_NetCount++;
	if (m_NetCount>=4)
	{
		if ((pCamManager->m_SendServer.GetSendServerStatus() & SendServer::enSendOK) != SendServer::enSendOK)
		{
			pCamManager->m_SendServer.ResetSocket();
		}
		if ((pCamManager->m_ReceiveServer.GetReceiveStatus() & SendServer::enRecvOK) != SendServer::enRecvOK)
		{
			pCamManager->m_ReceiveServer.ResetSocket();
		}
		m_NetCount = 0;
	}


}
