#pragma once
#include "stdafx.h"
#include "CameraObject.h"
#include "CameraManager.h"

CameraObject::CameraObject(void *pParent, HANDLE hParent):m_UsbCamServer(this)
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
	m_UsbCamServer.ForceEnd();
	if (m_Detector.m_imageWidth != 0 && m_Detector.m_imageHeight != 0)
	{ 
		m_Detector.Unitial();
	}
}

//创建文件夹读取句柄, 初始化ReadDirectoryChangesW相关参数
//执行前强制停止线程, 需要用Create()再次启动
bool CameraObject::RegPath(PATHSTR path)
{
	EnterCriticalSection(&m_section);
	m_UsbCamServer.RegPath(path);
	m_UsbCamServer.m_hParent = this->m_hThread;
	LeaveCriticalSection(&m_section);
	return true;
}

//创建线程
void CameraObject::Create(int times, long waiteTime, bool includeTaskTime)
{
	EnterCriticalSection(&m_section);
	Thread::Create(-1, waiteTime, includeTaskTime);
	m_UsbCamServer.Create(-1,waiteTime*2);
	LeaveCriticalSection(&m_section);
}
//继续执行挂起的线程
void CameraObject::Resume(void)
{
	EnterCriticalSection(&m_section);
	m_UsbCamServer.Resume();
	Thread::Resume();
	LeaveCriticalSection(&m_section);
}
//挂起线程
void CameraObject::Suspend(void)
{
	EnterCriticalSection(&m_section);
	m_UsbCamServer.Suspend();
	Thread::Suspend();
	LeaveCriticalSection(&m_section);
}
//设置参数使主函数退出以销毁线程
void  CameraObject::Destroy(void)
{
	EnterCriticalSection(&m_section);
	m_UsbCamServer.Destroy();
	Thread::Destroy();
	LeaveCriticalSection(&m_section);
}
//调用WindowsAPI强制结束当前线程
void CameraObject::ForceEnd(void)
{
	EnterCriticalSection(&m_section);
	m_UsbCamServer.ForceEnd();
	Thread::ForceEnd();
	LeaveCriticalSection(&m_section);
}


//处理m_DirWatchServer中的图像队列
void CameraObject::Task()
{
	FileItem tmpFileItem;
	EnterCriticalSection(&m_section);
	if (m_CameraInfo.Status==Tmtv_CameraInfo::TMTV_RUNNINGCAM &&
		m_CameraInfo.CameraPath[0]!=0)
	{	
		{
			while(!m_UsbCamServer.m_fileNameQueue.IsEmpty())
			{
				if (!m_UsbCamServer.m_fileNameQueue.GetHead(tmpFileItem))
				{
					break;
				}
				CCommonFunc::UnicodeToAnsi(tmpFileItem.m_fileName,m_ImageInfo.ImagePath, TMTV_PATHSTRLEN);
				m_ImageInfo.mCamId = m_CameraInfo.Indexnum;
				CCommonFunc::UnicodeToAnsi(tmpFileItem.m_fileTime, m_ImageInfo.GrabTime, STR_LEN(m_ImageInfo.GrabTime));
				m_ImageInfo.mDefectInfo.ImgWidth = tmpFileItem.m_fileWidth;
				m_ImageInfo.mDefectInfo.ImgHeight = tmpFileItem.m_fileHeiht;
				switch (m_CameraInfo.AlgorithmInfo.WarnningLevel)
				{
				case Tmtv_AlgorithmInfo::TMTV_NOWARN://仅返回图像
					m_ImageInfo.mDefectInfo.DefectNum = 0;
					if (!((CameraManager*)p_Parent)->SendImage(m_ImageInfo))
					{
						OutputDebugString(L"<CameraObject::Task() SendImage failed.>\n");
					}
					m_UsbCamServer.m_fileNameQueue.DelHead();
					break;
				case Tmtv_AlgorithmInfo::TMTV_PREWARN://预执行算法并返回图像
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
					m_UsbCamServer.m_fileNameQueue.DelHead();
					break;
				case Tmtv_AlgorithmInfo::TMTV_STARTWARN://执行算法并返回图像和缺陷
					m_ImageInfo.mDefectInfo.DefectNum = 0;
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
					
					if (!((CameraManager*)p_Parent)->SendImage(m_ImageInfo))
					{
						OutputDebugString(L"<CameraObject::Task() SendImage failed.>\n");
					}
					m_UsbCamServer.m_fileNameQueue.DelHead();
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


//添加相机,操作CameraObject对象
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
				//PATHWSTR cameraPathW = { 0 };
				//CCommonFunc::AnsiToUnicode(cameraInfo.CameraPath, cameraPathW, TMTV_PATHSTRLEN);
				//m_DirWatchServer->RegPath(cameraPathW,FILE_NOTIFY_CHANGE_LAST_WRITE);
				RegPath(cameraInfo.CameraPath);
				Create(-1, MAX(0,cameraInfo.WaiteTime),true);
				m_UsbCamServer.OpenAvailableUsbCam();
			}
			m_CameraInfo = cameraInfo;
			m_CameraInfo.Status = Tmtv_CameraInfo::TMTV_STOPEDCAM;
			m_ImageInfo.ImagePath[0] = 0;
			sprintf_s(cameraInfo.AlgorithmInfo.Reservechar, "haarcascade_frontalface_alt.xml");
			
			SetAlgorithm(cameraInfo.AlgorithmInfo);
			LeaveCriticalSection(&m_section);
			return true;
		}
	}
	LeaveCriticalSection(&m_section);
	OutputDebugString(L"<CameraObject::AddCamera() failed.>\n");
	return false;
}
//删除相机,操作CameraObject对象
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
			m_UsbCamServer.CloseUsbCam();
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
//打开相机,操作m_DirWatchServer对象
bool CameraObject::StartCamera()
{
	EnterCriticalSection(&m_section);
	if (m_CameraInfo.CameraPath[0] != 0 &&
		m_CameraInfo.Status == Tmtv_CameraInfo::TMTV_STOPEDCAM)
	{
		{
			m_UsbCamServer.Resume();
		}
		m_CameraInfo.Status = Tmtv_CameraInfo::TMTV_RUNNINGCAM;
		LeaveCriticalSection(&m_section);
		return true;
	}
	LeaveCriticalSection(&m_section);
	OutputDebugString(L"<CameraObject::StartCamera() failed.>\n");
	return false;
}
//停止相机,操作m_DirWatchServer对象
bool CameraObject::StopCamera()
{
	EnterCriticalSection(&m_section);
	if (m_CameraInfo.CameraPath[0] != 0 &&
		m_CameraInfo.Status == Tmtv_CameraInfo::TMTV_RUNNINGCAM)
	{
		m_UsbCamServer.Suspend();
		m_CameraInfo.Status = Tmtv_CameraInfo::TMTV_STOPEDCAM;
		LeaveCriticalSection(&m_section);
		return true;
	}
	LeaveCriticalSection(&m_section);
	OutputDebugString(L"<CameraObject::StopCamera() failed.>\n");
	return false;
}
//设置相机,操作m_DirWatchServer对象
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
//打开相机算法,操作m_Detector对象
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
//停止相机算法,操作m_Detector对象
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
//停止相机算法,操作m_Detector对象
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

