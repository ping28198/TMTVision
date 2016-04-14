#include "CameraManager.h"
#include "XMLOperator.h"
bool CameraManagerSetting::LoadSetting(PATHWSTR xmlFilePath)
{

	return false;
}

bool CameraManagerSetting::SaveSetting(PATHWSTR xmlFilePath)
{
	return false;
}

CameraManager::CameraManager(int maxCameraNum):
	m_MaxCameraNum(maxCameraNum), m_SendServer(this), m_ReceiveServer(this)
{

}

CameraManager::~CameraManager()
{
}

void CameraManager::Initial(CameraManagerSetting cameraManagerSetting)
{
	m_CameraManagerSetting = cameraManagerSetting;
	Initial();
}

void CameraManager::Initial()
{
	//this->LoadSetting(L"");
	this->Create(-1, m_CameraManagerSetting.m_SleepTime, true);
	this->Resume();
	m_SendServer.Initial(m_CameraManagerSetting.m_SendServerSetting);
	m_SendServer.Create();
	m_SendServer.Resume();
	m_ReceiveServer.Initial(m_CameraManagerSetting.m_ReceiveServerSetting);
	m_ReceiveServer.Create();
	m_ReceiveServer.Resume();
}

void CameraManager::Unitial()
{
	m_ReceiveServer.ForceEnd();
	m_SendServer.ForceEnd();
	this->ForceEnd();
	m_ReceiveServer.Unitial();
	m_SendServer.Unitial();
	this->Unitial();
}


CameraObject* CameraManager::GetCamServer(int CamIndex)
{
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end(); it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			(*it)->StopCamera();
			return true;
		}
	}
	return false;

	return NULL;
}

bool CameraManager::LoadSetting(PATHWSTR xmlFilePath)
{
	return m_CameraManagerSetting.LoadSetting(xmlFilePath);
}

bool CameraManager::SaveSetting(PATHWSTR xmlFilePath)
{
	return m_CameraManagerSetting.SaveSetting(xmlFilePath);
}

bool CameraManager::AddCamera(Tmtv_CameraInfo& cameraInfo)
{
	bool addedOK = true;
	CameraObject *newCameraObject = new CameraObject(this, this->m_hThread);
	addedOK &= newCameraObject->AddCamera(cameraInfo);
	newCameraObject->Resume();
	/*addedOK &=*/ newCameraObject->StartAlgorithm(cameraInfo.AlgorithmInfo);
	addedOK &= newCameraObject->StartCamera();
	m_CameraObjectVector.push_back(newCameraObject);
	return addedOK;
}

bool CameraManager::DelCamera(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end(); it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			(*it)->DelCamera();
			m_CameraObjectVector.erase(it);
			return true;
		}
	}
	return false;
}

bool CameraManager::StartCamera(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end(); it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			(*it)->StartCamera();
			return true;
		}
	}
	return false;
}

bool CameraManager::StopCamera(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end(); it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			(*it)->StopCamera();
			return true;
		}
	}
	return false;
}

bool CameraManager::SetCamera(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end(); it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			(*it)->SetCamera(cameraInfo);
			if (cameraInfo.Status == Tmtv_CameraInfo::TMTV_NOCAM)
			{
				m_CameraObjectVector.erase(it);
			}
			return true;
		}
	}
	return false;
}

bool CameraManager::GetCamera(Tmtv_CameraInfo & cameraInfo)
{
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end(); it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			memcpy((void*)&cameraInfo, (void*)&(*it)->m_ImageInfo.mCameraInfo,
				sizeof(Tmtv_CameraInfo));
			return true;
		}
	}
	return false;
}

bool CameraManager::StopAlgorithm(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end(); it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			(*it)->StopAlgorithm();
			return true;
		}
	}
	return false;
}

bool CameraManager::SetAlgorithm(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end(); it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			(*it)->SetAlgorithm(cameraInfo.AlgorithmInfo);
			return true;
		}
	}
	return false;
}

bool CameraManager::StartAlgorithm(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end(); it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			(*it)->StartAlgorithm(cameraInfo.AlgorithmInfo);
			return true;
		}
	}
	return false;
}

int CameraManager::SendMsgInfo(Tmtv_MsgInfo& msgInfo)
{
	return m_SendServer.PushMsg((void *)& msgInfo, msgInfo.structSize);
}

bool CameraManager::SendImage(Tmtv_ImageInfo& imgInfo)
{
	Tmtv_MsgInfo msgInfo;
	msgInfo.MsgType = Tmtv_MsgInfo::TMTV_SNAPED;
	memcpy(&msgInfo.ImgInfo, &imgInfo, sizeof(Tmtv_ImageInfo));
	return (SendMsgInfo(msgInfo) > 0);
}

void CameraManager::Task()
{
	MessageItem tmpMsgItem;
	EnterCriticalSection(&m_section);
	bool isOK = m_ReceiveServer.PullMsg(tmpMsgItem);
	LeaveCriticalSection(&m_section);
	if (!isOK)
	{
		return;
	}
	else
	{
		Tmtv_AskInfo* pAskData = (Tmtv_AskInfo*)tmpMsgItem.m_BufferSize;
		Tmtv_MsgInfo msgData;
		if (pAskData->CheckCode == TMTV_CHECKCODE)
		{
			switch (pAskData->Asktype)
			{
			case Tmtv_AskInfo::TMTV_ADDCAM:
				if (AddCamera(pAskData->CameraInfo))
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_ADDCAM_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_ADDCAM_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_DELCAM:
				if (DelCamera(pAskData->CameraInfo))
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_DELCAM_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_DELCAM_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_STARTCAM:
				if (StartCamera(pAskData->CameraInfo))
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_STARTCAM_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_STARTCAM_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_STOPCAM:
				if (StopCamera(pAskData->CameraInfo))
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_STOPCAM_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_STOPCAM_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_GETCAM:
				if (GetCamera(pAskData->CameraInfo))
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_GETCAM_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_GETCAM_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_SETCAM:
				if (SetCamera(pAskData->CameraInfo))
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_SETCAM_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_SETCAM_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_STARTALGO:
				if (StartAlgorithm(pAskData->CameraInfo))
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_STARTALGO_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_STARTALGO_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_STOPCALGO:
				if (StopAlgorithm(pAskData->CameraInfo))
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_STOPCAM_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_STOPCAM_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_SETALGO:
				if (SetAlgorithm(pAskData->CameraInfo))
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_SETALGO_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_SETALGO_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			default:
				break;
			}
		}
	}
}


