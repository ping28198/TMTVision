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

bool MessageServer::AddCamera(Tmtv_CameraInfo cameraInfo)
{
	return false;
}

bool MessageServer::DelCamera(Tmtv_CameraInfo cameraInfo)
{
	return false;
}

bool MessageServer::StartCamera(Tmtv_CameraInfo cameraInfo)
{
	return false;
}

bool MessageServer::StopCamera(Tmtv_CameraInfo cameraInfo)
{
	return false;
}

bool MessageServer::SetCamera(Tmtv_CameraInfo cameraInfo)
{
	return false;
}

bool MessageServer::StopAlgorithm(Tmtv_CameraInfo cameraInfo)
{
	return false;
}

bool MessageServer::SetAlgorithm(Tmtv_CameraInfo cameraInfo)
{
	return false;
}

bool MessageServer::StartAlgorithm(Tmtv_CameraInfo cameraInfo)
{
	return false;
}

bool MessageServer::SendMsgInfo(Tmtv_MsgInfo msgInfo)
{
	return SendMsg((void *)& msgInfo, msgInfo.structSize);
}

bool MessageServer::SendImage(Tmtv_ImageInfo imgInfo, HANDLE hAnswerHandle)
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
		Tmtv_AskInfo* pMsgData = (Tmtv_AskInfo*)pBuffer;
		if (pMsgData->CheckCode == TMTV_CHECKCODE)
		{
			switch (pMsgData->Asktype)
			{
			case Tmtv_AskInfo::TMTV_ADDCAM:
				break;
			case Tmtv_AskInfo::TMTV_DELCAM:
				break;
			case Tmtv_AskInfo::TMTV_STARTCAM:
				break;
			case Tmtv_AskInfo::TMTV_STOPCAM:
				break;
			case Tmtv_AskInfo::TMTV_GETCAM:
				break;
			case Tmtv_AskInfo::TMTV_SETCAM:
				break;
			case Tmtv_AskInfo::TMTV_STARTALGO:
				break;
			case Tmtv_AskInfo::TMTV_STOPCALGO:
				break;
			case Tmtv_AskInfo::TMTV_SETALGO:
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