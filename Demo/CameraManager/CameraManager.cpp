#include "CameraManager.h"
#include "XMLOperator.h"
bool CameraManagerSetting::LoadSetting(PATHWSTR xmlFilePath)
{
	CMemoryFile mMemFile;
	bool isOK = mMemFile.OpenFile_R(xmlFilePath);
	if (!isOK)
	{
		LoggerServer::mLogger.TraceError("��ȡ���������ļ�ʧ�ܣ�");
		return false;
	}
	CameraManagerSetting msetting;
	mMemFile.ReadMemoryFromFile(&msetting, sizeof(CameraManagerSetting));
	this->m_ReceiveServerSetting = msetting.m_ReceiveServerSetting;
	this->m_SendServerSetting = msetting.m_SendServerSetting;
	this->m_SleepTime = msetting.m_SleepTime;
	mMemFile.CloseFile();
	return true;
}

bool CameraManagerSetting::SaveSetting(PATHWSTR xmlFilePath)
{
	CMemoryFile mMemFile;
	bool isOK = mMemFile.OpenFile_W(xmlFilePath);
	if (!isOK)
	{
		LoggerServer::mLogger.TraceError("��ȡ���������ļ�ʧ�ܣ�");
		return false;
	}
	mMemFile.WriteMemoryToFile(this, sizeof(CameraManagerSetting));
	mMemFile.CloseFile();
	return true;


	return false;
}

CameraManager::CameraManager(int maxCameraNum):
	m_MaxCameraNum(maxCameraNum), m_SendServer(this), m_ReceiveServer(this)
{
	swprintf_s(m_CamObjFilePath, L"setting\\CamObjInfo.data");
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
	bool IsOk = this->LoadSetting();
	if (IsOk)
	{
		m_SendServer.Initial(m_CameraManagerSetting.m_SendServerSetting);
		m_SendServer.Create();
		m_SendServer.Resume();
		m_ReceiveServer.Initial(m_CameraManagerSetting.m_ReceiveServerSetting);
		m_ReceiveServer.Create();
		m_ReceiveServer.Resume();
		LoggerServer::mLogger.TraceInfo("�����������ò����ɹ���");
	}
	else
	{
		LoggerServer::mLogger.TraceInfo("�����������ò���ʧ�ܣ�");
	}
	this->Create(-1, 0, true);
	this->Resume();
	ReadAndSetCamObj();
}

void CameraManager::Unitial()
{
	m_ReceiveServer.ForceEnd();
	m_SendServer.ForceEnd();
	this->ForceEnd();
	m_ReceiveServer.Unitial();
	m_SendServer.Unitial();
	//this->Unitial();
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end();it++)
	{
		(*it)->DelCamera();
		Sleep(10);//�ȴ��߳̽���
		delete (*it);
	}
	m_CameraObjectVector.clear();
}
bool CameraManager::WriteCamObjsToFile()
{
	CMemoryFile mMemFile;
	bool isOK = mMemFile.OpenFile_W(m_CamObjFilePath);
	if (!isOK)
	{
		LoggerServer::mLogger.TraceError("д����������ļ�ʧ�ܣ�");
		return false;
	}
	vector<CameraObject*>::iterator it;
	Tmtv_CameraInfo mCamInfo;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end();it++)
	{
		(*it)->GetCamInfo(&mCamInfo);
		mMemFile.WriteMemoryToFile(&mCamInfo, sizeof(Tmtv_CameraInfo));
	}
	mMemFile.CloseFile();
	return true;
}

bool CameraManager::ReadAndSetCamObj()
{
	vector<CameraObject*>::iterator it;
	m_CameraObjectVector.clear();
	Tmtv_CameraInfo mCamInfo;
	bool IsOK = true;
	CMemoryFile mMemFile;
	IsOK &= mMemFile.OpenFile_R(m_CamObjFilePath);
	if (IsOK)
	{
		LoggerServer::mLogger.TraceError("������������ļ�ʧ��");
		return false;
	}
	while (mMemFile.ReadMemoryFromFile(&mCamInfo,sizeof(Tmtv_CameraInfo)))
	{
		//����������У�飬��֤��ȫ
		//
		//
		this->AddCamera(mCamInfo);
	}
	mMemFile.CloseFile();
	if (IsOK) LoggerServer::mLogger.TraceInfo("������������ɹ�");
	return true;
}

CameraObject* CameraManager::GetCamObject(int CamIndex)
{
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end(); it++)
	{
		if ((*it)->GetCamIndex() == CamIndex)
		{
			return *it;
		}
	}
	return NULL;
}

CameraObject* CameraManager::GetCamObject(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end(); it++)
	{
		if ((*it)->GetCamIndex() == cameraInfo.Indexnum)
		{
			return *it;
		}
	}
	return NULL;
}

bool CameraManager::LoadSetting()
{
	return m_CameraManagerSetting.LoadSetting(L"setting\\NetworkInfo.data");
}

bool CameraManager::SaveSetting()
{
	return m_CameraManagerSetting.SaveSetting(L"setting\\NetworkInfo.data");
}

bool CameraManager::AddCamera(Tmtv_CameraInfo& cameraInfo)
{
	bool addedOK = true;
	CameraObject *newCameraObject = new CameraObject(this, this->m_hThread);
	addedOK &= newCameraObject->AddCamera(cameraInfo);
	newCameraObject->Resume();
	/*addedOK &=*/ newCameraObject->SetAlgorithm(cameraInfo.AlgorithmInfo);
	addedOK &= newCameraObject->SetCamera(cameraInfo);
	m_CameraObjectVector.push_back(newCameraObject);
	LoggerServer::mLogger.TraceInfo("���%d������ɹ�", cameraInfo.Indexnum);
	WriteCamObjsToFile();
	return addedOK;
}

bool CameraManager::DelCamera(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end(); it++)
	{
		if ((*it)->GetCamIndex() == cameraInfo.Indexnum)
		{
			(*it)->DelCamera();
			delete (*it);
			m_CameraObjectVector.erase(it);
			LoggerServer::mLogger.TraceInfo("ɾ��%d������ɹ�", cameraInfo.Indexnum);
			WriteCamObjsToFile();
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
		if ((*it)->GetCamIndex() == cameraInfo.Indexnum)
		{
			(*it)->StartCamera();
			LoggerServer::mLogger.TraceInfo("��%d������ɹ�", cameraInfo.Indexnum);
			WriteCamObjsToFile();
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
		if ((*it)->GetCamIndex() == cameraInfo.Indexnum)
		{
			(*it)->StopCamera();
			LoggerServer::mLogger.TraceInfo("ֹͣ%d������ɹ�", cameraInfo.Indexnum);
			WriteCamObjsToFile();
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
		if ((*it)->GetCamIndex() == cameraInfo.Indexnum)
		{
			(*it)->SetCamera(cameraInfo);
			if (cameraInfo.Status == Tmtv_CameraInfo::TMTV_NOCAM)
			{
				(*it)->DelCamera();
				delete (*it);
				m_CameraObjectVector.erase(it);
			}
			LoggerServer::mLogger.TraceInfo("����%d������ɹ�", cameraInfo.Indexnum);
			WriteCamObjsToFile();
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
		if ((*it)->GetCamIndex() == cameraInfo.Indexnum)
		{
			(*it)->GetCamInfo(&cameraInfo);
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
		if ((*it)->GetCamIndex() == cameraInfo.Indexnum)
		{
			(*it)->StopAlgorithm();
			LoggerServer::mLogger.TraceInfo("ֹͣ%d������㷨�ɹ�", cameraInfo.Indexnum);
			WriteCamObjsToFile();
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
		if ((*it)->GetCamIndex() == cameraInfo.Indexnum)
		{
			(*it)->SetAlgorithm(cameraInfo.AlgorithmInfo);
			LoggerServer::mLogger.TraceInfo("����%d������㷨�ɹ�", cameraInfo.Indexnum);
			WriteCamObjsToFile();
			return true;
		}
	}
	return false;
}

bool CameraManager::GetManagerSetting(CameraManagerSetting &camManagerSetting)
{
	camManagerSetting = m_CameraManagerSetting;
	return true;
}

bool CameraManager::GetSendServerSetting(SendServerSetting &sendServerSetting)
{
	
	return m_SendServer.GetSetting(sendServerSetting);
}

bool CameraManager::GetReciveServerSeting(ReceiveServerSetting &receiveServerSetting)
{
	return m_ReceiveServer.GetSetting(receiveServerSetting);
}

bool CameraManager::SetManagerSetting(const CameraManagerSetting &camManagerSetting)
{
	bool isOK = true;
	isOK &= SetSendServerSetting(camManagerSetting.m_SendServerSetting);
	isOK &= SetReciveServerSeting(camManagerSetting.m_ReceiveServerSetting);
	return isOK;
}

bool CameraManager::SetSendServerSetting(const SendServerSetting &sendServerSetting)
{
	m_CameraManagerSetting.m_SendServerSetting = sendServerSetting;
	SaveSetting();
	bool IsOK = m_SendServer.ReSetSetting(m_CameraManagerSetting.m_SendServerSetting);
	if (IsOK)
	{
		LoggerServer::mLogger.TraceKeyInfo("���跢���������Ŀ��IP��%s:%d ����IP��%s:%d�ɹ���", sendServerSetting.m_RemoteRecvIp, 
			sendServerSetting.m_RemoteRecvPort, sendServerSetting.m_LocalSendIP, sendServerSetting.m_LocalSendPort);
	}
	else
	{
		LoggerServer::mLogger.TraceKeyInfo("���跢���������Ŀ��IP��%s:%d ����IP��%s:%dʧ�ܣ�", sendServerSetting.m_RemoteRecvIp,
			sendServerSetting.m_RemoteRecvPort, sendServerSetting.m_LocalSendIP, sendServerSetting.m_LocalSendPort);
	}
	return IsOK;
}
	

bool CameraManager::SetReciveServerSeting(const ReceiveServerSetting &receiveServerSetting)
{
	m_CameraManagerSetting.m_ReceiveServerSetting = receiveServerSetting;
	SaveSetting();
	bool IsOK = m_ReceiveServer.ReSetSetting(m_CameraManagerSetting.m_ReceiveServerSetting);
	if (IsOK)
	{
		LoggerServer::mLogger.TraceKeyInfo("�����������IP��%s�˿ڣ�%d�ɹ���",receiveServerSetting.m_LocalRecvIP, receiveServerSetting.m_LocalRecvPort);
	}
	else
	{
		LoggerServer::mLogger.TraceKeyInfo("�����������IP��%s�˿ڣ�%dʧ�ܣ�",receiveServerSetting.m_LocalRecvIP, receiveServerSetting.m_LocalRecvPort);
	}
	return IsOK;
}

bool CameraManager::StartAlgorithm(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end(); it++)
	{
		if ((*it)->GetCamIndex() == cameraInfo.Indexnum)
		{
			(*it)->StartAlgorithm(cameraInfo.AlgorithmInfo);
			LoggerServer::mLogger.TraceInfo("����%d������㷨�ɹ�", cameraInfo.Indexnum);
			WriteCamObjsToFile();
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

bool CameraManager::SendAllCamInfo()
{
	Tmtv_MsgInfo msgInfo;
	msgInfo.MsgType = Tmtv_MsgInfo::TMTV_CAMINFO;
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end(); it++)
	{
		(*it)->GetImgInfo(&msgInfo.ImgInfo);
		m_SendServer.PushMsg((void *)& msgInfo, msgInfo.structSize);
	}
	return true;
}

bool CameraManager::SendAllImgInfo()
{
	Tmtv_MsgInfo msgInfo;
	msgInfo.MsgType = Tmtv_MsgInfo::TMTV_SNAPED;
	vector<CameraObject*>::iterator it;
	for (it = m_CameraObjectVector.begin(); it != m_CameraObjectVector.end(); it++)
	{
		(*it)->GetImgInfo(&msgInfo.ImgInfo);
		m_SendServer.PushMsg((void *)& msgInfo, msgInfo.structSize);
	}
	return true;
}

void CameraManager::Task()
{
	MessageItem tmpMsgItem;
	EnterCriticalSection(&(m_ReceiveServer.m_section));
	bool isOK = m_ReceiveServer.PullMsg(tmpMsgItem);
	LeaveCriticalSection(&(m_ReceiveServer.m_section));
	if (!isOK)
	{
		return;
	}
	else
	{
		Tmtv_AskInfo* pAskData = (Tmtv_AskInfo*)tmpMsgItem.p_Buffer;
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
			case Tmtv_AskInfo::TMTV_GETALLCAM:
				if (SendAllCamInfo())
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_SENDALLCAMOK;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_GETALLIMG:
				if (SendAllImgInfo())
				{
					msgData.MsgType = Tmtv_MsgInfo::TMTV_SENDALLIMGOK;
					SendMsgInfo(msgData);
				}
				break;
			default:
				break;
			}
		}
	}
}


Logger LoggerServer::mLogger("serverlog\\");

