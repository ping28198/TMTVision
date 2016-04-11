#pragma once
#include "MessageServer.h"
#include "CameraServer.h"
MessageServer::MessageServer(int bufferSize)
{
	m_CameraServerVector.clear();
}

MessageServer::~MessageServer()
{
}

bool MessageServer::LoadSetting(PATHWSTR xmlFilePath)
{
	return false;
}

bool MessageServer::SaveSetting(PATHWSTR xmlFilePath)
{
	return false;
}

bool MessageServer::AddCamera(Tmtv_CameraInfo& cameraInfo)
{
	bool addedOK = true;
	CameraServer *newCameraServer = new CameraServer(this,this->m_hThread);
	addedOK &= newCameraServer->AddCamera(cameraInfo);
	newCameraServer->Resume();
	/*addedOK &=*/ newCameraServer->StartAlgorithm(cameraInfo.AlgorithmInfo);
	addedOK &= newCameraServer->StartCamera();
	m_CameraServerVector.push_back(newCameraServer);
	return addedOK;
}

bool MessageServer::DelCamera(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraServer*>::iterator it;
	for (it = m_CameraServerVector.begin(); it != m_CameraServerVector.end();it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			(*it)->DelCamera();
			m_CameraServerVector.erase(it);
			return true;
		}
	}
	return false;
}

bool MessageServer::StartCamera(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraServer*>::iterator it;
	for (it = m_CameraServerVector.begin(); it != m_CameraServerVector.end(); it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			(*it)->StartCamera();
			return true;
		}
	}
	return false;
}

bool MessageServer::StopCamera(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraServer*>::iterator it;
	for (it = m_CameraServerVector.begin(); it != m_CameraServerVector.end(); it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			(*it)->StopCamera();
			return true;
		}
	}
	return false;
}

bool MessageServer::SetCamera(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraServer*>::iterator it;
	for (it = m_CameraServerVector.begin(); it != m_CameraServerVector.end(); it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			(*it)->SetCamera(cameraInfo);
			if (cameraInfo.Status==Tmtv_CameraInfo::TMTV_NOCAM)
			{
				m_CameraServerVector.erase(it);
			}
			return true;
		}
	}
	return false;
}

bool MessageServer::GetCamera(Tmtv_CameraInfo & cameraInfo)
{
	vector<CameraServer*>::iterator it;
	for (it = m_CameraServerVector.begin(); it != m_CameraServerVector.end(); it++)
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

bool MessageServer::StopAlgorithm(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraServer*>::iterator it;
	for (it = m_CameraServerVector.begin(); it != m_CameraServerVector.end(); it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			(*it)->StopAlgorithm();
			return true;
		}
	}
	return false;
}

bool MessageServer::SetAlgorithm(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraServer*>::iterator it;
	for (it = m_CameraServerVector.begin(); it != m_CameraServerVector.end(); it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			(*it)->SetAlgorithm(cameraInfo.AlgorithmInfo);
			return true;
		}
	}
	return false;
}

bool MessageServer::StartAlgorithm(Tmtv_CameraInfo& cameraInfo)
{
	vector<CameraServer*>::iterator it;
	for (it = m_CameraServerVector.begin(); it != m_CameraServerVector.end(); it++)
	{
		if ((*it)->m_ImageInfo.mCameraInfo.Indexnum == cameraInfo.Indexnum)
		{
			(*it)->StartAlgorithm(cameraInfo.AlgorithmInfo);
			return true;
		}
	}
	return false;
}

int MessageServer::SendMsgInfo(Tmtv_MsgInfo& msgInfo)
{
	return SendMsg((void *)& msgInfo, msgInfo.structSize);
}

bool MessageServer::SendImage(Tmtv_ImageInfo& imgInfo)
{
	Tmtv_MsgInfo msgInfo;
	msgInfo.MsgType = Tmtv_MsgInfo::TMTV_SNAPED;
	memcpy(&msgInfo.ImgInfo, &imgInfo, sizeof(Tmtv_ImageInfo));
	return (SendMsgInfo(msgInfo) > 0);
}


void MessageServer::ServerProcess(int revLen)
{
	if (revLen>0)
	{
		//Tmtv_AskInfo msgData;
		//::EnterCriticalSection(&m_section);
		//::LeaveCriticalSection(&m_section);
		Tmtv_AskInfo* pAskData = (Tmtv_AskInfo*)pBuffer;
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

//µ÷ÊÔº¯Êý,ÏÔÊ¾¶ÔÏóÐÅÏ¢
//×Ö±³¾°ÑÕÉ«·¶Î§: 40--49        ×ÖÑÕÉ«: 30--39
//	40 : ºÚ                           30 : ºÚ
//	41 : ºì                           31 : ºì
//	42 : ÂÌ                           32 : ÂÌ
//	43 : »Æ                           33 : »Æ
//	44 : À¶                           34 : À¶
//	45 : ×Ï                           35 : ×Ï
//	46 : ÉîÂÌ                         36 : ÉîÂÌ
//	47 : °×É«                         37 : °×É«
void MessageServer::ToString(MEGAWSTR & string, int method, int color)
{
	string[0] = 0;
	MEGAWSTR tmpWStr1 = {0};
	TmtSocketServer::ToString(tmpWStr1, method, 0);
	MEGAWSTR tmpWStr2 = { 0 };
	if (method >= 0 && method <=2)
	{
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<MessageServer cameraNum=%d>\n", 
			m_CameraServerVector.size());
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s%s", string, tmpWStr1);

		vector<CameraServer*>::iterator it;
		for (it = m_CameraServerVector.begin(); it != m_CameraServerVector.end(); it++)
		{
			(*it)->ToString(tmpWStr2,1,31);
			CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s%s", string, tmpWStr2);
			break;
		}
		//CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s</MessageServer>\n", string);
	}
	if (color >= 30 && color <= 39)
	{
		MEGAWSTR testString = { 0 };
		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, string);
		CCommonFunc::SafeWStringCpy(string, TMTV_HUGESTRLEN, testString);
	}
}