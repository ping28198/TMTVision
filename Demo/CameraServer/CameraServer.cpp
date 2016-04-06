#pragma once
#include "CameraServer.h"
#include "MessageServer.h"

CameraServer::CameraServer(HANDLE hParent, void* hParentObj)
{
	pDirWatchServer = 0;
	pDirWatchServer = new DirWatchServer();
	m_CameraServerID++;
	m_ImageInfo.mCameraInfo.Indexnum = m_CameraServerID;
	m_ImageInfo.mCameraInfo.CameraPath[0] = 0;
	m_ImageInfo.mCameraInfo.Status = Tmtv_CameraInfo::TMTV_NOCAM;
	m_ImageInfo.mCameraInfo.AlgorithmInfo.DstImgPath[0] = 0;
	m_ImageInfo.mCameraInfo.AlgorithmInfo.MaskImgPath[0] = 0;
	m_ImageInfo.mCameraInfo.AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_NOWARN;
	m_Detector.m_imageWidth = 0;
	m_Detector.m_imageHeight = 0;
	m_ImageInfo.ImagePath[0] = 0;
	m_hParentObj = hParentObj;
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
//�����߳�
void CameraServer::Create(int times, long waiteTime, bool includeTaskTime)
{
	Thread::Create(-1, waiteTime, includeTaskTime);
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Create();
		pDirWatchServer->m_hParent = this->m_hThread;
	}
}
//����ִ�й�����߳�
void CameraServer::Resume(void)
{
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Resume();
	}
	Thread::Resume();
}
//�����߳�
void CameraServer::Suspend(void)
{
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Suspend();
	}
	Thread::Suspend();
}
//���ò���ʹ�������˳��������߳�
void  CameraServer::Destroy(void)
{
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Destroy();
	}
	Thread::Destroy();
}
//����WindowsAPIǿ�ƽ�����ǰ�߳�
void CameraServer::ForceEnd(void)
{
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->ForceEnd();
	}
	Thread::ForceEnd();
}


//����pDirWatchServer�е�ͼ�����
void CameraServer::Task()
{
	EnterCriticalSection(&m_section);
	FileItem tmpFileItem;
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
				CCommonFunc::UnicodeToAnsi(tmpFileItem.m_fileName,m_ImageInfo.ImagePath,TMTV_LONGSTRLEN);
				switch (m_ImageInfo.mCameraInfo.AlgorithmInfo.WarnningLevel)
				{
				case Tmtv_AlgorithmInfo::TMTV_NOWARN://������ͼ��
					m_ImageInfo.IsWarnning = 0;
					m_ImageInfo.mDefectInfo.DefectNum = 0;
					//MessageServer::GetState().SendImage(m_ImageInfo,m_hThread);
					break;
				case Tmtv_AlgorithmInfo::TMTV_PREWARN://Ԥִ���㷨������ͼ��
					m_ImageInfo.IsWarnning = 0;
					m_ImageInfo.mDefectInfo.DefectNum = 0;
					if (!tmpFileItem.m_fileProcessed &&
						(tmpFileItem.m_fileAction & FILE_ACTION_ADDED == FILE_ACTION_ADDED ||
							tmpFileItem.m_fileAction & FILE_ACTION_MODIFIED == FILE_ACTION_MODIFIED))
					{
						m_Detector.Detect(m_ImageInfo.ImagePath,
							m_ImageInfo.mCameraInfo.AlgorithmInfo.DstImgPath,
							m_ImageInfo.mDefectInfo);
					}
					m_ImageInfo.mDefectInfo.DefectNum = 0;
					//MessageServer::GetState().SendImage(m_ImageInfo, m_hThread);
					break;
				case Tmtv_AlgorithmInfo::TMTV_STARTWARN://ִ���㷨������ͼ���ȱ��
					m_ImageInfo.IsWarnning = 1;
					m_ImageInfo.mDefectInfo.DefectNum = 0;
					if (!tmpFileItem.m_fileProcessed &&
						(tmpFileItem.m_fileAction & FILE_ACTION_ADDED == FILE_ACTION_ADDED ||
							tmpFileItem.m_fileAction & FILE_ACTION_MODIFIED == FILE_ACTION_MODIFIED))
					{
						m_Detector.Detect(m_ImageInfo.ImagePath,
							m_ImageInfo.mCameraInfo.AlgorithmInfo.DstImgPath,
							m_ImageInfo.mDefectInfo);
					}
					//MessageServer::GetState().SendImage(m_ImageInfo, m_hThread);
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
bool CameraServer::AddCamera(Tmtv_CameraInfo cameraInfo)
{
	if (m_ImageInfo.mCameraInfo.CameraPath[0] == 0 &&
		m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_NOCAM)
	{
		if (cameraInfo.CameraPath[0]!=0 && 
			cameraInfo.CameraName[0]!=0)
		{
			Create();
			if (pDirWatchServer != 0)
			{
				LONGWSTR cameraPathW = { 0 };
				CCommonFunc::AnsiToUnicode(cameraInfo.CameraPath, cameraPathW, TMTV_LONGSTRLEN);
				pDirWatchServer->RegPath(cameraPathW,FILE_NOTIFY_CHANGE_LAST_WRITE);
			}
			else
			{
				OutputDebugString(L"<CameraServer::AddCamera() failed.>\n");
			}
			m_ImageInfo.mCameraInfo = cameraInfo;
			m_ImageInfo.mCameraInfo.Status = Tmtv_CameraInfo::TMTV_STOPEDCAM;
			m_ImageInfo.ImagePath[0] = 0;
			return true;
		}
	}
	OutputDebugString(L"<CameraServer::AddCamera() failed.>\n");
	return false;
}
//ɾ�����,����CameraServer����
bool CameraServer::DelCamera(Tmtv_CameraInfo cameraInfo)
{
	if (m_ImageInfo.mCameraInfo.CameraPath[0] == 0 &&
		m_ImageInfo.mCameraInfo.Status != Tmtv_CameraInfo::TMTV_NOCAM)
	{
		if (cameraInfo.CameraPath[0] != 0 &&
			cameraInfo.CameraName[0] != 0)
		{
			if (strcmp(cameraInfo.CameraName, m_ImageInfo.mCameraInfo.CameraName))
			{
				if (m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM)
				{   
					Tmtv_CameraInfo tmpCameraInfo;
					tmpCameraInfo.Status = Tmtv_CameraInfo::TMTV_STOPEDCAM;
					StopCamera(tmpCameraInfo);
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
				m_ImageInfo.mCameraInfo = cameraInfo;
				m_ImageInfo.mCameraInfo.CameraPath[0] = 0;
				m_ImageInfo.mCameraInfo.Status = Tmtv_CameraInfo::TMTV_NOCAM;
				m_ImageInfo.ImagePath[0] = 0;
				return true;
			}
		}
	}
	OutputDebugString(L"<CameraServer::AddCamera() failed.>\n");
	return false;
}
//�����,����pDirWatchServer����
bool CameraServer::StartCamera(Tmtv_CameraInfo cameraInfo)
{
	if (m_ImageInfo.mCameraInfo.CameraPath[0] != 0 &&
		m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_STOPEDCAM &&
		cameraInfo.Status== Tmtv_CameraInfo::TMTV_RUNNINGCAM)
	{
		if (strcmp(cameraInfo.CameraName, m_ImageInfo.mCameraInfo.CameraName))
		{
			if (pDirWatchServer != 0)
			{
				pDirWatchServer->Resume();
			}
			else
			{
				OutputDebugString(L"<CameraServer::AddCamera() failed.>\n");
				return false;
			}
			m_ImageInfo.mCameraInfo.Status = Tmtv_CameraInfo::TMTV_RUNNINGCAM;
			return true;
		}
	}
	OutputDebugString(L"<CameraServer::StartCamera() failed.>\n");
	return false;
}
//ֹͣ���,����pDirWatchServer����
bool CameraServer::StopCamera(Tmtv_CameraInfo cameraInfo)
{
	if (m_ImageInfo.mCameraInfo.CameraPath[0] != 0 &&
		m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM &&
		cameraInfo.Status == Tmtv_CameraInfo::TMTV_STOPEDCAM)
	{
		if (strcmp(cameraInfo.CameraName, m_ImageInfo.mCameraInfo.CameraName))
		{
			pDirWatchServer->Suspend();
			m_ImageInfo.mCameraInfo.Status = Tmtv_CameraInfo::TMTV_STOPEDCAM;
			return true;
		}
	}
	OutputDebugString(L"<CameraServer::StopCamera() failed.>\n");
	return false;
}
//�������,����pDirWatchServer����
bool CameraServer::SetCamera(Tmtv_CameraInfo cameraInfo)
{
	switch (cameraInfo.Status)
	{
	case Tmtv_CameraInfo::TMTV_STOPEDCAM:
		return StopCamera(cameraInfo);
		break;
	case Tmtv_CameraInfo::TMTV_RUNNINGCAM:
		return StartCamera(cameraInfo);
		break;
	case Tmtv_CameraInfo::TMTV_NOCAM:
		break;
	default:
		break;
	}
	OutputDebugString(L"<CameraServer::SetCamera() failed.>\n");
	return false;
}
//������㷨,����m_Detector����
bool CameraServer::StartAlgorithm(Tmtv_CameraInfo cameraInfo)
{
	if (m_ImageInfo.mCameraInfo.CameraPath[0] != 0 &&
		m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM &&
		cameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM)
	{
		if (strcmp(cameraInfo.CameraName,m_ImageInfo.mCameraInfo.CameraName) &&
			cameraInfo.AlgorithmInfo.WarnningLevel== Tmtv_AlgorithmInfo::TMTV_STARTWARN)
		{
			m_Detector.Reset(&cameraInfo.AlgorithmInfo);
			m_ImageInfo.mCameraInfo.AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_PREWARN;
			return true;
		}
	}
	OutputDebugString(L"<CameraServer::StartAlgorithm() failed.>\n");
	return false;
}
//ֹͣ����㷨,����m_Detector����
bool CameraServer::StopAlgorithm(Tmtv_CameraInfo cameraInfo)
{
	if (m_ImageInfo.mCameraInfo.CameraPath[0] != 0 &&
		m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM &&
		cameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM)
	{
		if (strcmp(cameraInfo.CameraName, m_ImageInfo.mCameraInfo.CameraName) &&
			cameraInfo.AlgorithmInfo.WarnningLevel == Tmtv_AlgorithmInfo::TMTV_NOWARN)
		{
			m_Detector.Reset(&cameraInfo.AlgorithmInfo);
			m_ImageInfo.mCameraInfo.AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_NOWARN;
			return true;
		}
	}
	OutputDebugString(L"<CameraServer::StopAlgorithm() failed.>\n");
	return false;
}
//ֹͣ����㷨,����m_Detector����
bool CameraServer::SetAlgorithm(Tmtv_CameraInfo cameraInfo)
{
	switch (cameraInfo.AlgorithmInfo.WarnningLevel)
	{
	case Tmtv_AlgorithmInfo::TMTV_NOWARN:
		return StartAlgorithm(cameraInfo);
		break;
	case Tmtv_AlgorithmInfo::TMTV_PREWARN:
		break;
	case Tmtv_AlgorithmInfo::TMTV_STARTWARN:
		return StopAlgorithm(cameraInfo);
		break;
	default:
		break;
	}
	OutputDebugString(L"<CameraServer::SetAlgorithm() failed.>\n");
	return false;
}
int CameraServer::m_CameraServerID = 0;