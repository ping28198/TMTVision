///<proj_info>
//==============================================================================
// 项目名 : 智能监控
// 文件名 : CameraServer.h
// 作  者 : 王磊
// 用  途 : 相机服务类CameraServer,内含线程任务Task()处理相机消息和图像
//          内含处理文件夹监控子线程DirWatchServer对象
// 版  权 : 霍比特人
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间      描述
//1.0     王磊        2016.3.20  整合旧版本
//==============================================================================
///</ver_info>

///<header_info>
//==============================================================================
//添加所需的头文件
#pragma once
#include "DirWatcher.h"
#include "Thread.h"
#include "CommonDefine.h"
#include "VisionStruct.h"
#include "Detector.h"
//==============================================================================
///</header_info>

///<class_info>
//==============================================================================
//功能描述: 相机服务类CameraServer,内含线程任务Task()处理相机消息和图像
//          内含处理文件夹监控子线程DirWatchServer对象
class CameraServer:public Thread
{
//类功能
public:
	CameraServer(HANDLE  hParent = 0);
	~CameraServer();
//文件监控
public:
	DirWatchServer *pDirWatchServer;
	//Tmtv_CameraInfo m_ImageInfo.mCameraInfo;
	BackgroundDetector m_Detector;
	static int m_CameraServerID;
	Tmtv_ImageInfo m_ImageInfo;//仅保存短算法结构
//线程功能,内部调用,禁止外部调用
public:
    //创建线程
	void  Create(int times = -1, long waiteTime = 0, bool includeTaskTime = true);
	//继续执行挂起的线程
	void  Resume(void);
	//挂起线程
	void  Suspend(void);
	//设置参数使主函数退出以销毁线程
	void  Destroy(void);
	//调用WindowsAPI强制结束当前线程
	void  ForceEnd(void);
//任务功能
public:
	//处理pDirWatchServer中的图像队列
	void Task();

//消息功能,由父线程调用或返回给父线程
	//添加相机,操作CameraServer对象, 准备pDirWatchServer对象
	bool AddCamera(Tmtv_CameraInfo cameraInfo);
	//删除相机,操作CameraServer对象, 准备pDirWatchServer对象
	bool DelCamera(Tmtv_CameraInfo cameraInfo);
	//打开相机,操作pDirWatchServer对象
	bool StartCamera(Tmtv_CameraInfo cameraInfo);
	//停止相机,操作pDirWatchServer对象
	bool StopCamera(Tmtv_CameraInfo cameraInfo);
	//设置相机,操作pDirWatchServer对象
	bool SetCamera(Tmtv_CameraInfo cameraInfo);
	//打开相机算法,操作m_Detector对象
	bool StartAlgorithm(Tmtv_CameraInfo cameraInfo);
	//停止相机算法,操作m_Detector对象
	bool StopAlgorithm(Tmtv_CameraInfo cameraInfo);
	//停止相机算法,操作m_Detector对象
	bool SetAlgorithm(Tmtv_CameraInfo cameraInfo);
};
int CameraServer::m_CameraServerID=0;
//==============================================================================
///</class_info>





