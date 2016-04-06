#pragma once
#include "MessageServer.h"
#include "CameraServer.h"
MessageServer::MessageServer(int bufferSize)
{
	pCameraInfo=0;
	cameraNum=0;
}

MessageServer::~MessageServer()
{
	if (pCameraInfo!=0)
	{
		delete[]pCameraInfo;
	}
	cameraNum = 0;
}

bool MessageServer::LoadSetting(LONGWSTR xmlFilePath)
{
	return false;
}

bool MessageServer::SaveSetting(LONGWSTR xmlFilePath)
{
	return false;
}

bool MessageServer::AddCamera(Tmtv_CameraInfo& cameraInfo)
{
	return false;
}

bool MessageServer::DelCamera(Tmtv_CameraInfo& cameraInfo)
{
	return false;
}

bool MessageServer::StartCamera(Tmtv_CameraInfo& cameraInfo)
{
	return false;
}

bool MessageServer::StopCamera(Tmtv_CameraInfo& cameraInfo)
{
	return false;
}

bool MessageServer::SetCamera(Tmtv_CameraInfo& cameraInfo)
{
	return false;
}

bool MessageServer::GetCamera(Tmtv_CameraInfo & cameraInfo)
{
	return false;
}

bool MessageServer::StopAlgorithm(Tmtv_CameraInfo& cameraInfo)
{
	return false;
}

bool MessageServer::SetAlgorithm(Tmtv_CameraInfo& cameraInfo)
{
	return false;
}

bool MessageServer::StartAlgorithm(Tmtv_CameraInfo& cameraInfo)
{
	return false;
}

bool MessageServer::SendMsgInfo(Tmtv_MsgInfo& msgInfo)
{
	return SendMsg((void *)& msgInfo, msgInfo.structSize);
}

bool MessageServer::SendImage(Tmtv_ImageInfo& imgInfo)
{

	return false;
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
					msgData.MsgType == Tmtv_MsgInfo::TMTV_ADDCAM_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_ADDCAM_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_DELCAM:
				if (DelCamera(pAskData->CameraInfo))
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_DELCAM_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_DELCAM_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_STARTCAM:
				if (StartCamera(pAskData->CameraInfo))
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_STARTCAM_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_STARTCAM_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_STOPCAM:
				if (StopCamera(pAskData->CameraInfo))
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_STOPCAM_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_STOPCAM_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_GETCAM:
				if (GetCamera(pAskData->CameraInfo))
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_GETCAM_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_GETCAM_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_SETCAM:
				if (SetCamera(pAskData->CameraInfo))
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_SETCAM_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_SETCAM_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_STARTALGO:
				if (StartAlgorithm(pAskData->CameraInfo))
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_STARTALGO_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_STARTALGO_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_STOPCALGO:
				if (StopAlgorithm(pAskData->CameraInfo))
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_STOPCAM_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_STOPCAM_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_SETALGO:
				if (SetAlgorithm(pAskData->CameraInfo))
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_SETALGO_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_SETALGO_FAIL;
					SendMsgInfo(msgData);
				}
				break;
			case Tmtv_AskInfo::TMTV_SETALGO:
				if (SetAlgorithm(pAskData->CameraInfo))
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_SETALGO_OK;
					SendMsgInfo(msgData);
				}
				else
				{
					msgData.MsgType == Tmtv_MsgInfo::TMTV_SETALGO_FAIL;
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
	MEGAWSTR tmpWStr = {0};
	TmtSocketServer::ToString(tmpWStr, method, 0);
	if (method >= 0 && method <=2)
	{
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<MessageServer cameraNum=%d>\n", cameraNum);
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s%s", string, tmpWStr);
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s</MessageServer>\n", string);
	}
	if (color >= 30 && color <= 39)
	{
		MEGAWSTR testString = { 0 };
		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, string);
		CCommonFunc::SafeWStringCpy(string, TMTV_HUGESTRLEN, testString);
	}
}