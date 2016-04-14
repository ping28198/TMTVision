#pragma once
#include "CameraServer.h"
#include "MessageServer.h"

CameraServer::CameraServer(void *pParent, HANDLE hParent)
{
	pDirWatchServer = 0;
	pDirWatchServer = new DirWatchServer();
	//m_CameraServerID++;
	//m_ImageInfo.mCameraInfo.Indexnum = /*m_CameraServerID*/;
	m_ImageInfo.mCameraInfo.CameraPath[0] = 0;
	m_ImageInfo.mCameraInfo.Status = Tmtv_CameraInfo::TMTV_NOCAM;
	m_ImageInfo.mCameraInfo.AlgorithmInfo.DstImgPath[0] = 0;
	m_ImageInfo.mCameraInfo.AlgorithmInfo.MaskImgPath[0] = 0;
	m_ImageInfo.mCameraInfo.AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_NOWARN;
	m_Detector.m_imageWidth = 0;
	m_Detector.m_imageHeight = 0;
	m_ImageInfo.ImagePath[0] = 0;
	m_ImageInfo.GrabTime[0] = 0;
	p_Parent = pParent;
}

CameraServer::~CameraServer()
{
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Destroy();
		delete pDirWatchServer;
		pDirWatchServer = 0;
	}
	if (m_Detector.m_imageWidth != 0 && m_Detector.m_imageHeight != 0)
	{ 
		m_Detector.Unitial();
	}
}

//�����ļ��ж�ȡ���, ��ʼ��ReadDirectoryChangesW��ز���
//ִ��ǰǿ��ֹͣ�߳�, ��Ҫ��Create()�ٴ�����
bool CameraServer::RegPath(PATHWSTR path, DWORD action)
{
	EnterCriticalSection(&m_section);
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->RegPath(path, action);
		pDirWatchServer->m_hParent = this->m_hThread;
		LeaveCriticalSection(&m_section);
		return true;
	}
	LeaveCriticalSection(&m_section);
	return false;
}

//�����߳�
void CameraServer::Create(int times, long waiteTime, bool includeTaskTime)
{
	EnterCriticalSection(&m_section);
	Thread::Create(-1, waiteTime, includeTaskTime);
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Create(-1,MAX(0, waiteTime*2),true);
		pDirWatchServer->m_hParent = this->m_hThread;
	}
	LeaveCriticalSection(&m_section);
}
//����ִ�й�����߳�
void CameraServer::Resume(void)
{
	EnterCriticalSection(&m_section);
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Resume();
	}
	Thread::Resume();
	LeaveCriticalSection(&m_section);
}
//�����߳�
void CameraServer::Suspend(void)
{
	EnterCriticalSection(&m_section);
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Suspend();
	}
	Thread::Suspend();
	LeaveCriticalSection(&m_section);
}
//���ò���ʹ�������˳��������߳�
void  CameraServer::Destroy(void)
{
	EnterCriticalSection(&m_section);
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Destroy();
	}
	Thread::Destroy();
	LeaveCriticalSection(&m_section);
}
//����WindowsAPIǿ�ƽ�����ǰ�߳�
void CameraServer::ForceEnd(void)
{
	EnterCriticalSection(&m_section);
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->ForceEnd();
	}
	Thread::ForceEnd();
	LeaveCriticalSection(&m_section);
}


//����pDirWatchServer�е�ͼ�����
void CameraServer::Task()
{
	FileItem tmpFileItem;
	EnterCriticalSection(&m_section);
	if (m_ImageInfo.mCameraInfo.Status==Tmtv_CameraInfo::TMTV_RUNNINGCAM &&
		m_ImageInfo.mCameraInfo.CameraPath[0]!=0)
	{	
		if (pDirWatchServer != 0)
		{
			while(!pDirWatchServer->m_fileNameQueue.IsEmpty())
			{
				if (!pDirWatchServer->m_fileNameQueue.GetHead(tmpFileItem))
				{
					break;
				}
				CCommonFunc::UnicodeToAnsi(tmpFileItem.m_fileName,m_ImageInfo.ImagePath, TMTV_PATHSTRLEN);
				switch (m_ImageInfo.mCameraInfo.AlgorithmInfo.WarnningLevel)
				{
				case Tmtv_AlgorithmInfo::TMTV_NOWARN://������ͼ��
					m_ImageInfo.IsWarnning = 0;
					m_ImageInfo.mDefectInfo.DefectNum = 0;
					if (!((MessageServer*)p_Parent)->SendImage(m_ImageInfo))
					{
						OutputDebugString(L"<CameraServer::Task() SendImage failed.>\n");
					}
					pDirWatchServer->m_fileNameQueue.DelHead();
					break;
				case Tmtv_AlgorithmInfo::TMTV_PREWARN://Ԥִ���㷨������ͼ��
					m_ImageInfo.IsWarnning = 0;
					m_ImageInfo.mDefectInfo.DefectNum = 0;
					if (!tmpFileItem.m_fileProcessed &&
						((tmpFileItem.m_fileAction & FILE_ACTION_ADDED) == FILE_ACTION_ADDED ||
							(tmpFileItem.m_fileAction & FILE_ACTION_MODIFIED) == FILE_ACTION_MODIFIED))
					{
						m_Detector.Detect(m_ImageInfo.ImagePath,
							m_ImageInfo.mCameraInfo.AlgorithmInfo.DstImgPath,
							m_ImageInfo.mDefectInfo);
					}
					m_ImageInfo.mDefectInfo.DefectNum = 0;
					if (!((MessageServer*)p_Parent)->SendImage(m_ImageInfo))
					{
						OutputDebugString(L"<CameraServer::Task() SendImage failed.>\n");
					}
					pDirWatchServer->m_fileNameQueue.DelHead();
					break;
				case Tmtv_AlgorithmInfo::TMTV_STARTWARN://ִ���㷨������ͼ���ȱ��
					m_ImageInfo.IsWarnning = 1;
					m_ImageInfo.mDefectInfo.DefectNum = 0;
					if (!tmpFileItem.m_fileProcessed &&
						((tmpFileItem.m_fileAction & FILE_ACTION_ADDED) == FILE_ACTION_ADDED ||
							(tmpFileItem.m_fileAction & FILE_ACTION_MODIFIED) == FILE_ACTION_MODIFIED))
					{
						m_Detector.Detect(m_ImageInfo.ImagePath,
							m_ImageInfo.mCameraInfo.AlgorithmInfo.DstImgPath,
							m_ImageInfo.mDefectInfo);
					}
					if (!((MessageServer*)p_Parent)->SendImage(m_ImageInfo))
					{
						OutputDebugString(L"<CameraServer::Task() SendImage failed.>\n");
					}
					pDirWatchServer->m_fileNameQueue.DelHead();
					break;
				default:
					break;
				}
			}
		}
	}
	//m_fileNameQueue.ForcTail(tmpFileItem);
	LeaveCriticalSection(&m_section);
}


//������,����CameraServer����
bool CameraServer::AddCamera(Tmtv_CameraInfo& cameraInfo)
{
	if (m_ImageInfo.mCameraInfo.CameraPath[0] == 0 &&
		m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_NOCAM)
	{
		if (cameraInfo.CameraPath[0]!=0 && 
			cameraInfo.CameraName[0]!=0)
		{
			if (pDirWatchServer != 0)
			{
				PATHWSTR cameraPathW = { 0 };
				CCommonFunc::AnsiToUnicode(cameraInfo.CameraPath, cameraPathW, TMTV_PATHSTRLEN);
				//pDirWatchServer->RegPath(cameraPathW,FILE_NOTIFY_CHANGE_LAST_WRITE);
				RegPath(cameraPathW, FILE_NOTIFY_CHANGE_LAST_WRITE);
				Create(-1, MAX(0,cameraInfo.WaiteTime),true);
			}
			else
			{
				OutputDebugString(L"<CameraServer::AddCamera() failed.>\n");
			}

			m_ImageInfo.mCameraInfo = cameraInfo;
			m_ImageInfo.mCameraInfo.Status = Tmtv_CameraInfo::TMTV_STOPEDCAM;
			m_ImageInfo.ImagePath[0] = 0;
			SetAlgorithm(cameraInfo.AlgorithmInfo);
			LeaveCriticalSection(&m_section);
			return true;
		}
	}
	LeaveCriticalSection(&m_section);
	OutputDebugString(L"<CameraServer::AddCamera() failed.>\n");
	return false;
}
//ɾ�����,����CameraServer����
bool CameraServer::DelCamera()
{
	EnterCriticalSection(&m_section);
	if (m_ImageInfo.mCameraInfo.CameraPath[0] == 0 &&
		m_ImageInfo.mCameraInfo.Status != Tmtv_CameraInfo::TMTV_NOCAM)
	{
		if (m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM)
		{
			Tmtv_CameraInfo tmpCameraInfo;
			tmpCameraInfo.Status = Tmtv_CameraInfo::TMTV_STOPEDCAM;
			StopCamera();
		}
		ForceEnd();
		if (pDirWatchServer != 0)
		{
			pDirWatchServer->FreePath();
		}
		else
		{
			OutputDebugString(L"<CameraServer::DelCamera() failed.>\n");
		}
		m_Detector.Unitial();
		m_ImageInfo.mCameraInfo.CameraPath[0] = 0;
		m_ImageInfo.mCameraInfo.Status = Tmtv_CameraInfo::TMTV_NOCAM;
		m_ImageInfo.ImagePath[0] = 0;
		LeaveCriticalSection(&m_section);
		return true;
	}
	LeaveCriticalSection(&m_section);
	OutputDebugString(L"<CameraServer::AddCamera() failed.>\n");
	return false;
}
//�����,����pDirWatchServer����
bool CameraServer::StartCamera()
{
	EnterCriticalSection(&m_section);
	if (m_ImageInfo.mCameraInfo.CameraPath[0] != 0 &&
		m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_STOPEDCAM)
	{
		if (pDirWatchServer != 0)
		{
			pDirWatchServer->Resume();
		}
		else
		{
			LeaveCriticalSection(&m_section);
			OutputDebugString(L"<CameraServer::AddCamera() failed.>\n");
			return false;
		}
		m_ImageInfo.mCameraInfo.Status = Tmtv_CameraInfo::TMTV_RUNNINGCAM;
		LeaveCriticalSection(&m_section);
		return true;
	}
	LeaveCriticalSection(&m_section);
	OutputDebugString(L"<CameraServer::StartCamera() failed.>\n");
	return false;
}
//ֹͣ���,����pDirWatchServer����
bool CameraServer::StopCamera()
{
	EnterCriticalSection(&m_section);
	if (m_ImageInfo.mCameraInfo.CameraPath[0] != 0 &&
		m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM)
	{
		pDirWatchServer->Suspend();
		m_ImageInfo.mCameraInfo.Status = Tmtv_CameraInfo::TMTV_STOPEDCAM;
		LeaveCriticalSection(&m_section);
		return true;
	}
	LeaveCriticalSection(&m_section);
	OutputDebugString(L"<CameraServer::StopCamera() failed.>\n");
	return false;
}
//�������,����pDirWatchServer����
bool CameraServer::SetCamera(Tmtv_CameraInfo& cameraInfo)
{
	switch (cameraInfo.Status)
	{
	case Tmtv_CameraInfo::TMTV_STOPEDCAM:
		return StopCamera();
		break;
	case Tmtv_CameraInfo::TMTV_RUNNINGCAM:
		return StartCamera();
		break;
	case Tmtv_CameraInfo::TMTV_NOCAM:
		StopCamera();
		return DelCamera();
		break;
	default:
		break;
	}
	OutputDebugString(L"<CameraServer::SetCamera() failed.>\n");
	return false;
}
//������㷨,����m_Detector����
bool CameraServer::StartAlgorithm(Tmtv_AlgorithmInfo& algorithmInfo)
{
	EnterCriticalSection(&m_section);
	if (m_ImageInfo.mCameraInfo.CameraPath[0] != 0 &&
		m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM)
	{
		if (algorithmInfo.WarnningLevel== Tmtv_AlgorithmInfo::TMTV_STARTWARN)
		{
			m_Detector.Reset(&algorithmInfo);
			m_ImageInfo.mCameraInfo.AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_PREWARN;
			LeaveCriticalSection(&m_section);
			return true;
		}
		LeaveCriticalSection(&m_section);
		return true;
	}
	LeaveCriticalSection(&m_section);
	OutputDebugString(L"<CameraServer::StartAlgorithm() failed.>\n");
	return false;
}
//ֹͣ����㷨,����m_Detector����
bool CameraServer::StopAlgorithm()
{
	EnterCriticalSection(&m_section);
	if (m_ImageInfo.mCameraInfo.CameraPath[0] != 0 &&
		m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM)
	{
		m_Detector.Unitial();
		m_ImageInfo.mCameraInfo.AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_NOWARN;
		LeaveCriticalSection(&m_section);
		return true;
	}
	LeaveCriticalSection(&m_section);
	OutputDebugString(L"<CameraServer::StopAlgorithm() failed.>\n");
	return false;
}
//ֹͣ����㷨,����m_Detector����
bool CameraServer::SetAlgorithm(Tmtv_AlgorithmInfo& algorithmInfo)
{
	switch (algorithmInfo.WarnningLevel)
	{
	case Tmtv_AlgorithmInfo::TMTV_STARTWARN:
		return StartAlgorithm(algorithmInfo);
		break;
	case Tmtv_AlgorithmInfo::TMTV_PREWARN:
		break;
	case Tmtv_AlgorithmInfo::TMTV_NOWARN:
		return StopAlgorithm();
		break;
	default:
		break;
	}
	OutputDebugString(L"<CameraServer::SetAlgorithm() failed.>\n");
	return false;
}
//int CameraServer::m_CameraServerID = 0;

void CameraServer::ToString(MEGAWSTR & string, int method, int color)
{
	string[0] = 0;
	MEGAWSTR tmpStr1 = { 0 };
	if (pDirWatchServer!=0)
	{
		pDirWatchServer->ToString(tmpStr1, method, 0);
	}
	MEGAWSTR tmpStr2 = { 0 };
	ObjToString::ToString(tmpStr2, m_ImageInfo, method, 0);
	if (method >= 0 && method <= 2)
	{
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<CameraServer>\n");
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s%s", string, tmpStr1);
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s%s", string, tmpStr2);
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s</CameraServer>\n", string);
	}
	if (color >= 30 && color <= 39)
	{
		MEGAWSTR testString = { 0 };
		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, string);
		CCommonFunc::SafeWStringCpy(string, TMTV_HUGESTRLEN, testString);
	}
}