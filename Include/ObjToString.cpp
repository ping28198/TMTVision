#include "ObjToString.h"
#include <string>
///<function_info>
//==============================================================================
//��������:�ṹ��ת��Ϊ�ַ���
//method=0 xml ������ʽ���, ������
//1 ��̸�ʽ���,ֻ�����������
//2 ���¸�ʽ���,�����������Ԫ��
//�ֱ�����ɫ��Χ: 40--49        ����ɫ: 30--39
//	40 : ��                           30 : ��
//	41 : ��                           31 : ��
//	42 : ��                           32 : ��
//	43 : ��                           33 : ��
//	44 : ��                           34 : ��
//	45 : ��                           35 : ��
//	46 : ����                         36 : ����
//	47 : ��ɫ                         37 : ��ɫ
void ObjToString::ToString(MEGAWSTR wstring, Tmtv_AlgorithmInfo algorithmInfo, int method, int color)
{
	wstring[0] = 0;
	MEGAWSTR tmpStr = { 0 };
	LONGWSTR tmpWStr = { 0 };
	if (method == 0)
	{
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"<Tmtv_AlgorithmInfo structSize =%d>\n", algorithmInfo.structSize);
		CCommonFunc::AnsiToUnicode(algorithmInfo.MaskImgPath, tmpWStr, TMTV_LONGSTRLEN);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s  <MaskImgPath=\"%s\">\n", wstring, tmpWStr);
		CCommonFunc::AnsiToUnicode(algorithmInfo.DstImgPath, tmpWStr, TMTV_LONGSTRLEN);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s  <DstImgPath=\"%s\">\n", wstring, tmpWStr);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s  <WarnningLevel=%d >\n", wstring, algorithmInfo.WarnningLevel);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s</Tmtv_AlgorithmInfo>\n", wstring);
	}
	else if (method == 1)
	{
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"<Tmtv_AlgorithmInfo WarnningLevel =%d>\n", algorithmInfo.WarnningLevel);
	}
	else if (method == 2)
	{
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"<Tmtv_AlgorithmInfo WarnningLevel =%d>\n", algorithmInfo.WarnningLevel);
	}
	if (color >= 30 && color <= 39)
	{
		MEGAWSTR testString = { 0 };
		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, wstring);
		CCommonFunc::SafeWStringCpy(wstring, TMTV_HUGESTRLEN, testString);
	}
}
void ObjToString::ToString(MEGAWSTR wstring, Tmtv_CameraInfo camraInfo, int method, int color)
{
	wstring[0] = 0;
	MEGAWSTR tmpWStrMega = { 0 };
	ObjToString::ToString(tmpWStrMega, camraInfo.AlgorithmInfo, method, 0);
	LONGWSTR tmpWStr = { 0 };
	if (method == 0)
	{
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"<Tmtv_CameraInfo Indexnum=%d>\n", camraInfo.Indexnum);
		CCommonFunc::AnsiToUnicode(camraInfo.CameraName, tmpWStr, TMTV_LONGSTRLEN);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s  <CameraName=\"%s\">\n", wstring, tmpWStr);
		CCommonFunc::AnsiToUnicode(camraInfo.CameraPath, tmpWStr, TMTV_LONGSTRLEN);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s  <CameraPath=\"%s\">\n", wstring, tmpWStr);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s  <CameraPos=[%d %d %d %d %d %d %d %d]>\n", wstring,
			camraInfo.CameraPos[0], camraInfo.CameraPos[1], camraInfo.CameraPos[2], camraInfo.CameraPos[3],
			camraInfo.CameraPos[4], camraInfo.CameraPos[5], camraInfo.CameraPos[6], camraInfo.CameraPos[7]);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s  <CameraWidth=%d CameraHeight=%d>\n", wstring,
			camraInfo.CameraWidth, camraInfo.CameraHeight);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s  <Status=%d>\n", wstring, camraInfo.Status);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s%s", wstring, tmpWStrMega);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s</Tmtv_CameraInfo>\n", wstring);
	}
	else if (method == 1)
	{
		CCommonFunc::AnsiToUnicode(camraInfo.CameraName, tmpWStr, TMTV_LONGSTRLEN);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"<Tmtv_CameraInfo Indexnum=%d CameraName=\"%s\" Status=%d>\n",
			camraInfo.Indexnum, tmpWStr, camraInfo.Status);
	}
	else if (method == 2)
	{
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"<Tmtv_CameraInfo Indexnum=%d Status=%d",
			camraInfo.Indexnum, camraInfo.Status);
		CCommonFunc::AnsiToUnicode(camraInfo.CameraName, tmpWStr, TMTV_LONGSTRLEN);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s CameraName=\"%s\" ", wstring, tmpWStr);
		CCommonFunc::AnsiToUnicode(camraInfo.CameraPath, tmpWStr, TMTV_LONGSTRLEN);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s CameraPath=\"%s\">\n", wstring, tmpWStr);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s%s", wstring, tmpWStrMega);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s</Tmtv_CameraInfo>\n", wstring);
	}
	if (color >= 30 && color <= 39)
	{
		MEGAWSTR testString = { 0 };
		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, wstring);
		CCommonFunc::SafeWStringCpy(wstring, TMTV_HUGESTRLEN, testString);
	}
}
void ObjToString::ToString(MEGAWSTR wstring, Tmtv_DefectInfo defectInfo, int method, int color)
{
	wstring[0] = 0;
	LONGWSTR tmpWStr = { 0 };
	if (method == 0)
	{
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"<Tmtv_DefectInfo DefectNum=%d>\n", defectInfo.DefectNum);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s  <ImgWidth=%d ImgHeight=%d>\n", wstring,
			defectInfo.ImgWidth, defectInfo.ImgHeight);
		for (int i = 0; i < defectInfo.DefectNum - 1; i++)
		{
			CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s  <DefectPos=[%d %d %d %d %d %d %d %d]>\n", wstring,
				defectInfo.DefectPos[0], defectInfo.DefectPos[1], defectInfo.DefectPos[2], defectInfo.DefectPos[3],
				defectInfo.DefectPos[4], defectInfo.DefectPos[5], defectInfo.DefectPos[6], defectInfo.DefectPos[7]);
		}
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s</Tmtv_DefectInfo>\n", wstring);
	}
	else if (method == 1)
	{
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"<Tmtv_DefectInfo DefectNum=%d>\n", defectInfo.DefectNum);
	}
	else if (method == 2)
	{
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"<Tmtv_DefectInfo DefectNum=%d>\n", defectInfo.DefectNum);
	}
	if (color >= 30 && color <= 39)
	{
		MEGAWSTR testString = { 0 };
		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, wstring);
		CCommonFunc::SafeWStringCpy(wstring, TMTV_HUGESTRLEN, testString);
	}
}
void ObjToString::ToString(MEGAWSTR wstring, Tmtv_ImageInfo imageInfo, int method, int color)
{
	wstring[0] = 0;
	MEGAWSTR tmpWStrMega1 = { 0 };
	ObjToString::ToString(tmpWStrMega1, imageInfo.mCameraInfo, method, 0);
	MEGAWSTR tmpWStrMega2 = { 0 };
	ObjToString::ToString(tmpWStrMega2, imageInfo.mDefectInfo, method, 0);
	LONGWSTR tmpWStr1 = { 0 };
	CCommonFunc::AnsiToUnicode(imageInfo.ImagePath, tmpWStr1, TMTV_LONGSTRLEN);
	TINYWSTR tmpWStr2 = { 0 };
	CCommonFunc::AnsiToUnicode(imageInfo.GrabTime, tmpWStr2, TMTV_TINYSTRLEN);

	if (method >= 0 && method <= 2)
	{
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"<Tmtv_ImageInfo ImagePath=\"%s\" GrabTime=\"%s\" IsWarnning=%d IsVIP=%d>\n",
			tmpWStr1, tmpWStr2, imageInfo.IsWarnning, imageInfo.IsVIP);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s%s", wstring, tmpWStrMega1);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s%s", wstring, tmpWStrMega2);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s</Tmtv_ImageInfo>\n", wstring);
	}
	if (color >= 30 && color <= 39)
	{
		MEGAWSTR testString = { 0 };
		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, wstring);
		CCommonFunc::SafeWStringCpy(wstring, TMTV_HUGESTRLEN, testString);
	}
}
void ObjToString::ToString(MEGAWSTR wstring, Tmtv_AskInfo askInfo, int method, int color)
{
	wstring[0] = 0;
	MEGAWSTR tmpWStrMega1 = { 0 };
	ObjToString::ToString(tmpWStrMega1, askInfo.CameraInfo, method, 0);
	LONGWSTR tmpWStr = { 0 };
	if (method >= 0 && method <= 2)
	{
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"<Tmtv_AskInfo structSize=%d CheckCode=%d Asktype=%d>\n",
			askInfo.structSize, askInfo.CheckCode, askInfo.Asktype);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s  <hAskHandle=%d hAnswerHandle=%d>\n",
			wstring, askInfo.hAskHandle, askInfo.hAnswerHandle);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s%s", wstring, tmpWStrMega1);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s</Tmtv_AskInfo>\n", wstring);
	}
	if (color >= 30 && color <= 39)
	{
		MEGAWSTR testString = { 0 };
		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, wstring);
		CCommonFunc::SafeWStringCpy(wstring, TMTV_HUGESTRLEN, testString);
	}
}
void ObjToString::ToString(MEGAWSTR wstring, Tmtv_MsgInfo msgInfo, int method, int color)
{
	wstring[0] = 0;
	MEGAWSTR tmpWStrMega1 = { 0 };
	ObjToString::ToString(tmpWStrMega1, msgInfo.ImgInfo, method, 0);
	LONGWSTR tmpWStr = { 0 };
	if (method >= 0 && method <= 2)
	{
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"<Tmtv_MsgInfo structSize=%d CheckCode=%d MsgType=%d>\n",
			msgInfo.structSize, msgInfo.CheckCode, msgInfo.MsgType);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s  <hAskHandle=%d hAnswerHandle=%d>\n",
			wstring, msgInfo.hAskHandle, msgInfo.hAnswerHandle);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s%s", wstring, tmpWStrMega1);
		CCommonFunc::SafeWStringPrintf(wstring, TMTV_HUGESTRLEN, L"%s</Tmtv_MsgInfo>\n", wstring);
	}
	if (color >= 30 && color <= 39)
	{
		MEGAWSTR testString = { 0 };
		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, wstring);
		CCommonFunc::SafeWStringCpy(wstring, TMTV_HUGESTRLEN, testString);
	}
}

//��������:��Ϣ���,�Ƿ���Ϣ����-1,0ΪTmtv_AskInfo,1ΪTmtv_MsgInfo
int ObjToString::InfoCheck(void* pBuffer, long  bufferSize)
{
	Tmtv_AskInfo * testInfo = (Tmtv_AskInfo*)pBuffer;
	if (bufferSize < sizeof(Tmtv_AskInfo) && bufferSize < sizeof(Tmtv_MsgInfo))
	{
		return -1;
	}
	if (testInfo->CheckCode != TMTV_CHECKCODE)
	{
		return -1;
	}
	if (testInfo->Asktype < 200 && testInfo->Asktype >= 100)
	{
		return 0;
	}
	if (testInfo->Asktype < 300 && testInfo->Asktype >= 200)
	{
		return 1;
	}
}

int ObjToString::ToString(MEGAWSTR wstring, void * pBuffer, long bufferSize, int method, int color)
{
	int infoType = InfoCheck(pBuffer, bufferSize);
	Tmtv_AskInfo tmpAskInfo;
	Tmtv_MsgInfo tmpMsgInfo;
	switch (infoType)
	{
	case 1:
		memcpy((void*)&tmpAskInfo, pBuffer, sizeof(Tmtv_AskInfo));
		ObjToString::ToString(wstring, tmpAskInfo, method, 0);
		break;
	case 2:
		memcpy((void*)&tmpMsgInfo, pBuffer, sizeof(Tmtv_MsgInfo));
		ObjToString::ToString(wstring, tmpMsgInfo, method, 0);
		break;
	default:
		return -1;
		break;
	}
	return infoType;
}
//==============================================================================
///</function_info>