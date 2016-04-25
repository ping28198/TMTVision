#pragma once
#include "stdafx.h"
#include "CameraObject.h"
#include "CameraManager.h"

CameraObject::CameraObject(void *pParent, HANDLE hParent):m_DirWatchServer(this)
{
	//m_CameraObjectID++;
	//m_ImageInfo.mCameraInfo.Indexnum = /*m_CameraObjectID*/;
	m_CameraInfo.CameraPath[0] = 0;
	m_CameraInfo.Status = Tmtv_CameraInfo::TMTV_NOCAM;
	m_CameraInfo.AlgorithmInfo.DstImgPath[0] = 0;
	m_CameraInfo.AlgorithmInfo.MaskImgPath[0] = 0;
	m_CameraInfo.AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_NOWARN;
	m_Detector.m_imageWidth = 0;
	m_Detector.m_imageHeight = 0;
	m_ImageInfo.ImagePath[0] = 0;
	m_ImageInfo.GrabTime[0] = 0;
	p_Parent = pParent;
}

CameraObject::~CameraObject()
{
	m_DirWatchServer.ForceEnd();
	if (m_Detector.m_imageWidth != 0 && m_Detector.m_imageHeight != 0)
	{ 
		m_Detector.Unitial();
	}
}

//�����ļ��ж�ȡ���, ��ʼ��ReadDirectoryChangesW��ز���
//ִ��ǰǿ��ֹͣ�߳�, ��Ҫ��Create()�ٴ�����
bool CameraObject::RegPath(PATHWSTR path, DWORD action)
{
	EnterCriticalSection(&m_section);
	m_DirWatchServer.RegPath(path, action);
	m_DirWatchServer.m_hParent = this->m_hThread;
	LeaveCriticalSection(&m_section);
	return true;
}

//�����߳�
void CameraObject::Create(int times, long waiteTime, bool includeTaskTime)
{
	EnterCriticalSection(&m_section);
	Thread::Create(-1, waiteTime, includeTaskTime);
	m_DirWatchServer.Create(-1,MAX(0, waiteTime*2),true);
	m_DirWatchServer.m_hParent = this->m_hThread;
	LeaveCriticalSection(&m_section);
}
//����ִ�й�����߳�
void CameraObject::Resume(void)
{
	EnterCriticalSection(&m_section);
	m_DirWatchServer.Resume();
	Thread::Resume();
	LeaveCriticalSection(&m_section);
}
//�����߳�
void CameraObject::Suspend(void)
{
	EnterCriticalSection(&m_section);
	m_DirWatchServer.Suspend();
	Thread::Suspend();
	LeaveCriticalSection(&m_section);
}
//���ò���ʹ�������˳��������߳�
void  CameraObject::Destroy(void)
{
	EnterCriticalSection(&m_section);
	m_DirWatchServer.Destroy();
	Thread::Destroy();
	LeaveCriticalSection(&m_section);
}
//����WindowsAPIǿ�ƽ�����ǰ�߳�
void CameraObject::ForceEnd(void)
{
	EnterCriticalSection(&m_section);
	m_DirWatchServer.ForceEnd();
	Thread::ForceEnd();
	LeaveCriticalSection(&m_section);
}


//����m_DirWatchServer�е�ͼ�����
void CameraObject::Task()
{
	FileItem tmpFileItem;
	EnterCriticalSection(&m_section);
	if (m_CameraInfo.Status==Tmtv_CameraInfo::TMTV_RUNNINGCAM &&
		m_CameraInfo.CameraPath[0]!=0)
	{	
		{
			while(!m_DirWatchServer.m_fileNameQueue.IsEmpty())
			{
				if (!m_DirWatchServer.m_fileNameQueue.GetHead(tmpFileItem))
				{
					break;
				}
				CCommonFunc::UnicodeToAnsi(tmpFileItem.m_fileName,m_ImageInfo.ImagePath, TMTV_PATHSTRLEN);
				switch (m_CameraInfo.AlgorithmInfo.WarnningLevel)
				{
				case Tmtv_AlgorithmInfo::TMTV_NOWARN://������ͼ��
					m_ImageInfo.mDefectInfo.DefectNum = 0;
					if (!((CameraManager*)p_Parent)->SendImage(m_ImageInfo))
					{
						OutputDebugString(L"<CameraObject::Task() SendImage failed.>\n");
					}
					m_DirWatchServer.m_fileNameQueue.DelHead();
					break;
				case Tmtv_AlgorithmInfo::TMTV_PREWARN://Ԥִ���㷨������ͼ��
					m_ImageInfo.mDefectInfo.DefectNum = 0;
					if (!tmpFileItem.m_fileProcessed &&
						((tmpFileItem.m_fileAction & FILE_ACTION_ADDED) == FILE_ACTION_ADDED ||
							(tmpFileItem.m_fileAction & FILE_ACTION_MODIFIED) == FILE_ACTION_MODIFIED))
					{
						try
						{
							m_Detector.Detect(m_ImageInfo.ImagePath,
								m_CameraInfo.AlgorithmInfo.DstImgPath,
								m_ImageInfo.mDefectInfo);
						}
						catch (...)
						{
							OutputDebugString(L"<CameraObject::Task() Detect failed.>\n");
						}

					}
					m_ImageInfo.mDefectInfo.DefectNum = 0;
					if (!((CameraManager*)p_Parent)->SendImage(m_ImageInfo))
					{
						OutputDebugString(L"<CameraObject::Task() SendImage failed.>\n");
					}
					m_DirWatchServer.m_fileNameQueue.DelHead();
					break;
				case Tmtv_AlgorithmInfo::TMTV_STARTWARN://ִ���㷨������ͼ���ȱ��
					m_ImageInfo.mDefectInfo.DefectNum = 0;
					if (!tmpFileItem.m_fileProcessed &&
						((tmpFileItem.m_fileAction & FILE_ACTION_ADDED) == FILE_ACTION_ADDED ||
							(tmpFileItem.m_fileAction & FILE_ACTION_MODIFIED) == FILE_ACTION_MODIFIED))
					{
						try
						{
							m_Detector.Detect(m_ImageInfo.ImagePath,
								m_CameraInfo.AlgorithmInfo.DstImgPath,
								m_ImageInfo.mDefectInfo);
						}
						catch (...)
						{
							OutputDebugString(L"<CameraObject::Task() Detect failed.>\n");
						}
					}
					if (!((CameraManager*)p_Parent)->SendImage(m_ImageInfo))
					{
						OutputDebugString(L"<CameraObject::Task() SendImage failed.>\n");
					}
					m_DirWatchServer.m_fileNameQueue.DelHead();
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


//������,����CameraObject����
bool CameraObject::AddCamera(Tmtv_CameraInfo& cameraInfo)
{
	EnterCriticalSection(&m_section);
	if (m_CameraInfo.CameraPath[0] == 0 &&
		m_CameraInfo.Status == Tmtv_CameraInfo::TMTV_NOCAM)
	{
		if (cameraInfo.CameraPath[0]!=0 && 
			cameraInfo.CameraName[0]!=0)
		{
			{
				PATHWSTR cameraPathW = { 0 };
				CCommonFunc::AnsiToUnicode(cameraInfo.CameraPath, cameraPathW, TMTV_PATHSTRLEN);
				//m_DirWatchServer->RegPath(cameraPathW,FILE_NOTIFY_CHANGE_LAST_WRITE);
				RegPath(cameraPathW, FILE_NOTIFY_CHANGE_LAST_WRITE);
				Create(-1, MAX(0,cameraInfo.WaiteTime),true);
			}
			m_CameraInfo = cameraInfo;
			m_CameraInfo.Status = Tmtv_CameraInfo::TMTV_STOPEDCAM;
			m_ImageInfo.ImagePath[0] = 0;
			SetAlgorithm(cameraInfo.AlgorithmInfo);
			LeaveCriticalSection(&m_section);
			return true;
		}
	}
	LeaveCriticalSection(&m_section);
	OutputDebugString(L"<CameraObject::AddCamera() failed.>\n");
	return false;
}
//ɾ�����,����CameraObject����
bool CameraObject::DelCamera()
{
	EnterCriticalSection(&m_section);
	if (m_CameraInfo.CameraPath[0] != 0 &&
		m_CameraInfo.Status != Tmtv_CameraInfo::TMTV_NOCAM)
	{
		if (m_CameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM)
		{
			Tmtv_CameraInfo tmpCameraInfo;
			tmpCameraInfo.Status = Tmtv_CameraInfo::TMTV_STOPEDCAM;
			StopCamera();
		}
		ForceEnd();
		{
			m_DirWatchServer.FreePath();
		}
		m_Detector.Unitial();
		m_CameraInfo.CameraPath[0] = 0;
		m_CameraInfo.Status = Tmtv_CameraInfo::TMTV_NOCAM;
		m_ImageInfo.ImagePath[0] = 0;
		LeaveCriticalSection(&m_section);
		return true;
	}
	LeaveCriticalSection(&m_section);
	OutputDebugString(L"<CameraObject::AddCamera() failed.>\n");
	return false;
}
//�����,����m_DirWatchServer����
bool CameraObject::StartCamera()
{
	EnterCriticalSection(&m_section);
	if (m_CameraInfo.CameraPath[0] != 0 &&
		m_CameraInfo.Status == Tmtv_CameraInfo::TMTV_STOPEDCAM)
	{
		{
			m_DirWatchServer.Resume();
		}
		m_CameraInfo.Status = Tmtv_CameraInfo::TMTV_RUNNINGCAM;
		LeaveCriticalSection(&m_section);
		return true;
	}
	LeaveCriticalSection(&m_section);
	OutputDebugString(L"<CameraObject::StartCamera() failed.>\n");
	return false;
}
//ֹͣ���,����m_DirWatchServer����
bool CameraObject::StopCamera()
{
	EnterCriticalSection(&m_section);
	if (m_CameraInfo.CameraPath[0] != 0 &&
		m_CameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM)
	{
		m_DirWatchServer.Suspend();
		m_CameraInfo.Status = Tmtv_CameraInfo::TMTV_STOPEDCAM;
		LeaveCriticalSection(&m_section);
		return true;
	}
	LeaveCriticalSection(&m_section);
	OutputDebugString(L"<CameraObject::StopCamera() failed.>\n");
	return false;
}
//�������,����m_DirWatchServer����
bool CameraObject::SetCamera(Tmtv_CameraInfo& cameraInfo)
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
	OutputDebugString(L"<CameraObject::SetCamera() failed.>\n");
	return false;
}
//������㷨,����m_Detector����
bool CameraObject::StartAlgorithm(Tmtv_AlgorithmInfo& algorithmInfo)
{
	EnterCriticalSection(&m_section);
	if (m_CameraInfo.CameraPath[0] != 0 &&
		m_CameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM)
	{
		if (algorithmInfo.WarnningLevel== Tmtv_AlgorithmInfo::TMTV_STARTWARN)
		{
			m_Detector.Reset(algorithmInfo);
			m_CameraInfo.AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_PREWARN;
			LeaveCriticalSection(&m_section);
			return true;
		}
		LeaveCriticalSection(&m_section);
		return true;
	}
	LeaveCriticalSection(&m_section);
	OutputDebugString(L"<CameraObject::StartAlgorithm() failed.>\n");
	return false;
}
//ֹͣ����㷨,����m_Detector����
bool CameraObject::StopAlgorithm()
{
	EnterCriticalSection(&m_section);
	if (m_CameraInfo.CameraPath[0] != 0 &&
		m_CameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM)
	{
		m_Detector.Unitial();
		m_CameraInfo.AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_NOWARN;
		LeaveCriticalSection(&m_section);
		return true;
	}
	LeaveCriticalSection(&m_section);
	OutputDebugString(L"<CameraObject::StopAlgorithm() failed.>\n");
	return false;
}
//ֹͣ����㷨,����m_Detector����
bool CameraObject::SetAlgorithm(Tmtv_AlgorithmInfo& algorithmInfo)
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
	OutputDebugString(L"<CameraObject::SetAlgorithm() failed.>\n");
	return false;
}

bool CameraObject::GetImgInfo(Tmtv_ImageInfo* pImgInfo)
{
	if (pImgInfo == NULL) return false;
	EnterCriticalSection(&m_section);
	memcpy(pImgInfo, &m_ImageInfo, sizeof(Tmtv_ImageInfo));
	LeaveCriticalSection(&m_section);
	return true;
}

int CameraObject::GetCamIndex()
{
	int mIndex=0;
	EnterCriticalSection(&m_section);
	mIndex = m_CameraInfo.Indexnum;
	LeaveCriticalSection(&m_section);
	return mIndex;
}

bool CameraObject::GetCamInfo(Tmtv_CameraInfo* pCamInfo)
{
	if (pCamInfo == NULL) return false;
	EnterCriticalSection(&m_section);
	memcpy(pCamInfo, &(m_CameraInfo), sizeof(Tmtv_CameraInfo));
	LeaveCriticalSection(&m_section);
	return true;
}

//int CameraObject::m_CameraObjectID = 0;

void CameraObject::ToString(MEGAWSTR & string, int method, int color)
{
	string[0] = 0;
	MEGAWSTR tmpStr1 = { 0 };
	{
		m_DirWatchServer.ToString(tmpStr1, method, 0);
	}
	MEGAWSTR tmpStr2 = { 0 };
	ObjToString::ToString(tmpStr2, m_ImageInfo, method, 0);
	if (method >= 0 && method <= 2)
	{
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<CameraObject>\n");
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s%s", string, tmpStr1);
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s%s", string, tmpStr2);
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s</CameraObject>\n", string);
	}
	if (color >= 30 && color <= 39)
	{
		MEGAWSTR testString = { 0 };
		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, string);
		CCommonFunc::SafeWStringCpy(string, TMTV_HUGESTRLEN, testString);
	}
}