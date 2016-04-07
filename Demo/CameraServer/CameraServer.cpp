#pragma once
#include "CameraServer.h"
#include "MessageServer.h"

CameraServer::CameraServer(HANDLE hParent)
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

//创建文件夹读取句柄, 初始化ReadDirectoryChangesW相关参数
//执行前强制停止线程, 需要用Create()再次启动
bool CameraServer::RegPath(LONGWSTR path, DWORD action)
{
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->RegPath(path, action);
		pDirWatchServer->m_hParent = this->m_hThread;
		return true;
	}
	return false;
}

//创建线程
void CameraServer::Create(int times, long waiteTime, bool includeTaskTime)
{
	Thread::Create(-1, waiteTime, includeTaskTime);
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Create(-1,MAX(0, waiteTime*2),true);
		pDirWatchServer->m_hParent = this->m_hThread;
	}
}
//继续执行挂起的线程
void CameraServer::Resume(void)
{
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Resume();
	}
	Thread::Resume();
}
//挂起线程
void CameraServer::Suspend(void)
{
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Suspend();
	}
	Thread::Suspend();
}
//设置参数使主函数退出以销毁线程
void  CameraServer::Destroy(void)
{
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->Destroy();
	}
	Thread::Destroy();
}
//调用WindowsAPI强制结束当前线程
void CameraServer::ForceEnd(void)
{
	if (pDirWatchServer != 0)
	{
		pDirWatchServer->ForceEnd();
	}
	Thread::ForceEnd();
}


//处理pDirWatchServer中的图像队列
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
				CCommonFunc::UnicodeToAnsi(tmpFileItem.m_fileName,m_ImageInfo.ImagePath, TMTV_PATHSTRLEN);
				switch (m_ImageInfo.mCameraInfo.AlgorithmInfo.WarnningLevel)
				{
				case Tmtv_AlgorithmInfo::TMTV_NOWARN://仅返回图像
					m_ImageInfo.IsWarnning = 0;
					m_ImageInfo.mDefectInfo.DefectNum = 0;
					if (!MessageServer::GetState().SendImage(m_ImageInfo))
					{
						OutputDebugString(L"<CameraServer::Task() SendImage failed.>\n");
					}
					pDirWatchServer->m_fileNameQueue.DelHead();
					break;
				case Tmtv_AlgorithmInfo::TMTV_PREWARN://预执行算法并返回图像
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
					if (!MessageServer::GetState().SendImage(m_ImageInfo))
					{
						OutputDebugString(L"<CameraServer::Task() SendImage failed.>\n");
					}
					pDirWatchServer->m_fileNameQueue.DelHead();
					break;
				case Tmtv_AlgorithmInfo::TMTV_STARTWARN://执行算法并返回图像和缺陷
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
					if (!MessageServer::GetState().SendImage(m_ImageInfo))
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


//添加相机,操作CameraServer对象
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
			return true;
		}
	}
	OutputDebugString(L"<CameraServer::AddCamera() failed.>\n");
	return false;
}
//删除相机,操作CameraServer对象
bool CameraServer::DelCamera()
{
	if (m_ImageInfo.mCameraInfo.CameraPath[0] == 0 &&
		m_ImageInfo.mCameraInfo.Status != Tmtv_CameraInfo::TMTV_NOCAM)
	{
		if (1)
		{
			if (1)
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
				return true;
			}
		}
	}
	OutputDebugString(L"<CameraServer::AddCamera() failed.>\n");
	return false;
}
//打开相机,操作pDirWatchServer对象
bool CameraServer::StartCamera()
{
	if (m_ImageInfo.mCameraInfo.CameraPath[0] != 0 &&
		m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_STOPEDCAM)
	{
		if (1)
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
//停止相机,操作pDirWatchServer对象
bool CameraServer::StopCamera()
{
	if (m_ImageInfo.mCameraInfo.CameraPath[0] != 0 &&
		m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM)
	{
		if (1)
		{
			pDirWatchServer->Suspend();
			m_ImageInfo.mCameraInfo.Status = Tmtv_CameraInfo::TMTV_STOPEDCAM;
			return true;
		}
	}
	OutputDebugString(L"<CameraServer::StopCamera() failed.>\n");
	return false;
}
//设置相机,操作pDirWatchServer对象
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
//打开相机算法,操作m_Detector对象
bool CameraServer::StartAlgorithm(Tmtv_AlgorithmInfo& algorithmInfo)
{
	if (m_ImageInfo.mCameraInfo.CameraPath[0] != 0 &&
		m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM)
	{
		if (algorithmInfo.WarnningLevel== Tmtv_AlgorithmInfo::TMTV_STARTWARN)
		{
			m_Detector.Reset(&algorithmInfo);
			m_ImageInfo.mCameraInfo.AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_PREWARN;
			return true;
		}
		return true;
	}
	OutputDebugString(L"<CameraServer::StartAlgorithm() failed.>\n");
	return false;
}
//停止相机算法,操作m_Detector对象
bool CameraServer::StopAlgorithm()
{
	if (m_ImageInfo.mCameraInfo.CameraPath[0] != 0 &&
		m_ImageInfo.mCameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM)
	{
		if (1)
		{
			m_Detector.Unitial();
			m_ImageInfo.mCameraInfo.AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_NOWARN;
			return true;
		}
	}
	OutputDebugString(L"<CameraServer::StopAlgorithm() failed.>\n");
	return false;
}
//停止相机算法,操作m_Detector对象
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