#include "CameraManager.h"
#include "XMLOperator.h"
bool CameraManagerSetting::LoadSetting(PATHWSTR xmlFilePath)
{
	Config mCfg;
	try
	{
		mCfg.ReadFile("setting\\NetworkInfo.ini");
	}
	catch (Config::File_not_found m)
	{
		LoggerServer::mLogger.TraceWarning("打开配置文件%s失败，将使用默认配置！", m.filename.c_str());
	}
	NetIP netip;
	TmtSocket::GetAvailableNetIP(netip);
	string mstr = mCfg.Read("LocalSendIP", string(netip));
	strcpy_s(m_SendServerSetting.m_LocalSendIP, TMTV_IPSTRLEN, mstr.c_str());
	mstr = mCfg.Read("RemoteRecvIp", string(netip));
	strcpy_s(m_SendServerSetting.m_RemoteRecvIp, TMTV_IPSTRLEN, mstr.c_str());
	mstr = mCfg.Read("LocalRecvIP", string(netip));
	strcpy_s(m_ReceiveServerSetting.m_LocalRecvIP, TMTV_IPSTRLEN, mstr.c_str());
	m_SendServerSetting.m_LocalSendPort = mCfg.Read("LocalSendPort", TMT_CAMSERVER_SENDPORT);
	m_SendServerSetting.m_RemoteRecvPort= mCfg.Read("RemoteRecvPort", TMT_DBSERVER_RECVPORT);
	m_ReceiveServerSetting.m_LocalRecvPort= mCfg.Read("LocalRecvPort", TMT_CAMSERVER_RECVPORT);
	m_SendServerSetting.m_SleepTime = mCfg.Read("SendSleepTime", 100);
	m_ReceiveServerSetting.m_SleepTime = mCfg.Read("RecvSleepTime", 100);
	m_SleepTime = mCfg.Read("CamManagerSleepTime", 100);
	SaveSetting(L"a");
	return true;
}

bool CameraManagerSetting::SaveSetting(PATHWSTR xmlFilePath)
{
	Config mCfg;
	try
	{
		mCfg.ReadFile("setting\\NetworkInfo.ini");
	}
	catch (Config::File_not_found m)
	{
		LoggerServer::mLogger.TraceWarning("打开配置文件%s失败，将使用默认配置！", m.filename.c_str());
	}
	mCfg.Add("LocalSendIP", string(m_SendServerSetting.m_LocalSendIP));
	mCfg.Add("RemoteRecvIp", string(m_SendServerSetting.m_RemoteRecvIp));
	mCfg.Add("LocalRecvIP", string(m_ReceiveServerSetting.m_LocalRecvIP));
	mCfg.Add("LocalSendPort", m_SendServerSetting.m_LocalSendPort);
	mCfg.Add("RemoteRecvPort", m_SendServerSetting.m_RemoteRecvPort);
	mCfg.Add("LocalRecvPort", m_ReceiveServerSetting.m_LocalRecvPort);
	mCfg.Add("SendSleepTime", m_SendServerSetting.m_SleepTime);
	mCfg.Add("RecvSleepTime", m_ReceiveServerSetting.m_SleepTime);
	mCfg.Add("CamManagerSleepTime", m_SleepTime);
	return true;
}

CameraManager::CameraManager(int maxCameraNum):
	m_MaxCameraNum(maxCameraNum), m_SendServer(this), m_ReceiveServer(this)
{
	swprintf_s(m_CamObjFilePath, L"setting\\CamObjInfo.data");
}

CameraManager::~CameraManager()
{
	Unitial();
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
		LoggerServer::mLogger.TraceInfo("加载网络配置参数成功！");
	}
	else
	{
		LoggerServer::mLogger.TraceInfo("加载网络配置参数失败！");
	}
	NetWorkResetCount = 0;
	this->Create(-1, 100, true);
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
		Sleep(10);//等待线程结束
		delete (*it);
	}
	SaveSetting();
	m_CameraObjectVector.clear();
}
bool CameraManager::WriteCamObjsToFile()
{
	CMemoryFile mMemFile;
	bool isOK = mMemFile.OpenFile_W(m_CamObjFilePath);
	if (!isOK)
	{
		LoggerServer::mLogger.TraceError("写入相机对象文件失败！");
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
	IsOK = mMemFile.OpenFile_R(m_CamObjFilePath);
	if (!IsOK)
	{
		LoggerServer::mLogger.TraceError("读取相机对象文件失败！");
		mMemFile.CloseFile();
		return false;
	}
	while (mMemFile.ReadMemoryFromFile(&mCamInfo,sizeof(Tmtv_CameraInfo)))
	{
		//需增加数据校验，保证安全
		//
		//
		this->AddCamera(mCamInfo);
	}
	mMemFile.CloseFile();
	LoggerServer::mLogger.TraceInfo("加载所有相机成功！");
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
	LoggerServer::mLogger.TraceInfo("添加%d号相机成功", cameraInfo.Indexnum);
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
			LoggerServer::mLogger.TraceInfo("删除%d号相机成功", cameraInfo.Indexnum);
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
			LoggerServer::mLogger.TraceInfo("打开%d号相机成功", cameraInfo.Indexnum);
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
			LoggerServer::mLogger.TraceInfo("停止%d号相机成功", cameraInfo.Indexnum);
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
			LoggerServer::mLogger.TraceInfo("设置%d号相机成功", cameraInfo.Indexnum);
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
			LoggerServer::mLogger.TraceInfo("停止%d号相机算法成功", cameraInfo.Indexnum);
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
			LoggerServer::mLogger.TraceInfo("设置%d号相机算法成功", cameraInfo.Indexnum);
			WriteCamObjsToFile();
			return true;
		}
	}
	return false;
}

bool CameraManager::AskAddCam(Tmtv_CameraInfo& cameraInfo)
{
	Tmtv_BaseNetMessage mBaseMsg;
	mBaseMsg.MsgType = Tmtv_BaseNetMessage::TMTV_ADDCAM;
	strcpy_s(mBaseMsg.dstAddr, m_CameraManagerSetting.m_SendServerSetting.m_RemoteRecvIp);
	mBaseMsg.dstPort = m_CameraManagerSetting.m_SendServerSetting.m_RemoteRecvPort;
	strcpy_s(mBaseMsg.mAddr, m_CameraManagerSetting.m_ReceiveServerSetting.m_LocalRecvIP);
	mBaseMsg.mPort = m_CameraManagerSetting.m_ReceiveServerSetting.m_LocalRecvPort;
	mBaseMsg.ElementCount = 1;
	mBaseMsg.ElementLength = sizeof(Tmtv_CameraInfo);
	strcpy_s(cameraInfo.CameraHost, m_CameraManagerSetting.m_ReceiveServerSetting.m_LocalRecvIP);
	MessageItem mMsgItem;
	memcpy(mMsgItem.p_Buffer, &mBaseMsg, sizeof(mBaseMsg));
	memcpy(mMsgItem.p_Buffer + sizeof(mBaseMsg), &cameraInfo, sizeof(cameraInfo));
	return (m_SendServer.PushMsg(mMsgItem) > 0);
}

bool CameraManager::GetManagerSetting(CameraManagerSetting &camManagerSetting)
{
	GetSendServerSetting(m_CameraManagerSetting.m_SendServerSetting);
	GetReciveServerSeting(m_CameraManagerSetting.m_ReceiveServerSetting);
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
		LoggerServer::mLogger.TraceKeyInfo("重设发送网络服务：目标IP：%s:%d 本地IP：%s:%d成功！", sendServerSetting.m_RemoteRecvIp, 
			sendServerSetting.m_RemoteRecvPort, sendServerSetting.m_LocalSendIP, sendServerSetting.m_LocalSendPort);
	}
	else
	{
		LoggerServer::mLogger.TraceKeyInfo("重设发送网络服务：目标IP：%s:%d 本地IP：%s:%d失败！", sendServerSetting.m_RemoteRecvIp,
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
		LoggerServer::mLogger.TraceKeyInfo("重设接收网络IP：%s端口：%d成功！",receiveServerSetting.m_LocalRecvIP, receiveServerSetting.m_LocalRecvPort);
	}
	else
	{
		LoggerServer::mLogger.TraceKeyInfo("重设接收网络IP：%s端口：%d失败！",receiveServerSetting.m_LocalRecvIP, receiveServerSetting.m_LocalRecvPort);
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
			LoggerServer::mLogger.TraceInfo("启动%d号相机算法成功", cameraInfo.Indexnum);
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
	Tmtv_BaseNetMessage mBaseMsg;
	mBaseMsg.MsgType = Tmtv_BaseNetMessage::TMTV_SNAPED;
	strcpy_s(mBaseMsg.dstAddr, m_CameraManagerSetting.m_SendServerSetting.m_RemoteRecvIp);
	mBaseMsg.dstPort = m_CameraManagerSetting.m_SendServerSetting.m_RemoteRecvPort;
	strcpy_s(mBaseMsg.mAddr, m_CameraManagerSetting.m_ReceiveServerSetting.m_LocalRecvIP);
	mBaseMsg.mPort = m_CameraManagerSetting.m_ReceiveServerSetting.m_LocalRecvPort;
	mBaseMsg.ElementCount = 1;
	mBaseMsg.ElementLength = sizeof(Tmtv_ImageInfo);
	MessageItem mMsgItem;
	memcpy(mMsgItem.p_Buffer, &mBaseMsg, sizeof(mBaseMsg));
	memcpy(mMsgItem.p_Buffer + sizeof(mBaseMsg), &imgInfo, sizeof(imgInfo));
	return (m_SendServer.PushMsg(mMsgItem) > 0);
}

void CameraManager::Task()
{
	MessageItem tmpMsgItem;
	MessageItem tmpSendMsgItem;
	bool isOK = m_ReceiveServer.PullMsg(tmpMsgItem);
	if (!isOK)
	{
		Sleep(100);
		return;
	}
	Tmtv_BaseNetMessage* pBaseMsg = (Tmtv_BaseNetMessage*)(tmpMsgItem.p_Buffer);
	Tmtv_CameraInfo* pCam;
	//Tmtv_ImageInfo* pImg;
	//Tmt_ClientInfo* pClient;
	//Tmtv_AlgorithmInfo* pAlgo;
	//int b;
	//vector<Tmtv_ImageInfo> mImgVec;
	//vector<Tmtv_ImageInfo>::iterator itimg;
	//vector<Tmtv_CameraInfo> mCamVec;
	//vector<Tmtv_CameraInfo>::iterator itcam;
	//Tmtv_BaseNetMessage mBaseMsg;
	switch (pBaseMsg->MsgType)
	{
	case Tmtv_BaseNetMessage::TMTV_ADDCAM_OK:
		LoggerServer::mLogger.TraceKeyInfo("添加相机返回成功！");
		if (pBaseMsg->ElementCount != 1 || pBaseMsg->ElementLength != sizeof(Tmtv_CameraInfo)) return;
		pCam = (Tmtv_CameraInfo*)(tmpMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage));
		if (AddCamera(*pCam))
		{
		}
		break;
	default:
		break;
	}
}


Logger LoggerServer::mLogger("serverlog\\");

