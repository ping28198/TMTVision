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
#include"CommonInclude.h"
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
	CameraServer(void *pParam=NULL,HANDLE  hParent = 0);
	~CameraServer();
//文件监控
public:
	DirWatchServer *pDirWatchServer;
	//Tmtv_CameraInfo m_ImageInfo.mCameraInfo;
	BackgroundDetector m_Detector;
	//static int m_CameraServerID;
	Tmtv_ImageInfo m_ImageInfo;//仅保存短算法结构

	void *pParent;//父类指针

//线程功能,内部调用,禁止外部调用
public:
	//创建文件夹读取句柄, 初始化ReadDirectoryChangesW相关参数
	//执行前强制停止线程, 需要用Create()再次启动,具有线程保护
	bool RegPath(PATHWSTR path, DWORD action = FILE_NOTIFY_CHANGE_LAST_WRITE);
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
	bool AddCamera(Tmtv_CameraInfo& cameraInfo);
	//删除相机,操作CameraServer对象, 准备pDirWatchServer对象
	bool DelCamera();
	//打开相机,操作pDirWatchServer对象
	bool StartCamera();
	//停止相机,操作pDirWatchServer对象
	bool StopCamera();
	//设置相机,操作pDirWatchServer对象
	bool SetCamera(Tmtv_CameraInfo& cameraInfo);
	//打开相机算法,操作m_Detector对象
	bool StartAlgorithm(Tmtv_AlgorithmInfo& algorithmInfo);
	//停止相机算法,操作m_Detector对象
	bool StopAlgorithm();
	//停止相机算法,操作m_Detector对象
	bool SetAlgorithm(Tmtv_AlgorithmInfo& algorithmInfo);

public:
	//调试函数,显示对象信息
	//method=0 xml 完整格式输出, 带换行
	//1 最短格式输出,只输出队列数量
	//2 更新格式输出,输出队列最新元素
	//字背景颜色范围: 40--49        字颜色: 30--39
	//	40 : 黑                           30 : 黑
	//	41 : 红                           31 : 红
	//	42 : 绿                           32 : 绿
	//	43 : 黄                           33 : 黄
	//	44 : 蓝                           34 : 蓝
	//	45 : 紫                           35 : 紫
	//	46 : 深绿                         36 : 深绿
	//	47 : 白色                         37 : 白色
	void ToString(MEGAWSTR &string, int method = 0, int color = 32);
};

//==============================================================================
///</class_info>





