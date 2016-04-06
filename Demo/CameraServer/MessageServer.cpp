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

//���Ժ���,��ʾ������Ϣ
//�ֱ�����ɫ��Χ: 40--49        ����ɫ: 30--39
//	40 : ��                           30 : ��
//	41 : ��                           31 : ��
//	42 : ��                           32 : ��
//	43 : ��                           33 : ��
//	44 : ��                           34 : ��
//	45 : ��                           35 : ��
//	46 : ����                         36 : ����
//	47 : ��ɫ                         37 : ��ɫ
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