///<algorithm_info>
//==============================================================================
//功能描述:相机+算法状态空间: * means TMTV or TMTV_****_
//         必须按照顺序启动, 不能跳过中间过程和中间状态,
//         切换状态的命令只在其箭头后面一个或若干状态下有效,
//         否则返回TMTV_FAIL非法的命令通知  
//
//                              ┏  TMTV_NOCAM   ┓ ◀──────────┐
//                        ┏━▶ ┗  TMTV_NOWARN  ┛─┬▶*_STOPALGO┐     │
//                        ┃      ┃           ▲    ├▶*_STOPCAM ┼▶*_INVALID
//                        ┃      ┃           ┃    └▶*_DELCAM  ┘
//                        ┃      ▼           ┃
//                        ┃   *_ADDCAM      *_OK / *_FAIL
//                      *_FAIL / *_OK       *_DELCAM   ┃
//                                ┃           ▲      ┃
//                                ┃           ┃      ┃
//                                ▼           ┃      ┃
//     *_INVALID◀─*_ADDCAM◀──┏TMTV_STOPEDCAM ┓ ◀━┹────────┐
//          └──────┲━▶ ┗ TMTV_NOWARN   ┛─┬▶*_STOPALGO┬▶*_INVALID
//                        ┃      ┃           ▲    └▶*_STOPCAM ┘
//                        ┃      ┃           ┃
//                        ┃      ▼           ┃
//                        ┃ *_STERTCAM      *_OK / *_FAIL
//                      *_FAIL / *_OK     *_STOPCAM    ┃
//                                ┃           ▲      ┃
//                                ┃           ┃      ┃
//             ┌*_STERTCAM◀┐    ▼           ┃      ┃
//   *_INVALID◀┴ *_ADDCAM ◀┴─┏TMTV_RUNNINGCAM┓ ◀━┹────────┐
//        └───────┲━▶ ┗ TMTV_NOWARN   ┛──▶*_STOPALGO─▶*_INVALID
//                        ┃      ┃           ▲
//                        ┃      ┃           ┃
//                        ┃      ▼           ┃
//                        ┃ *_STERTALGO     *_OK / *_FAIL
//                        ┃      ┃           ▲      ┃
//                        ┃      ▼           ┃      ┃
//                        ┃    ┏TMTV_RUNNINGCAM┓    ┃
//                        ┃    ┗  TMTV_PREWARN ┛    ┃
//                        ┃      ┃           ▲      ┃
//                        ┃      ▼           ┃      ┃
//                      *_FAIL / *_OK      *_STOPALGO  ┃
//                                ┃           ▲      ┃
//            ┌*_STERTALGO◀┐    ┃           ┃      ┃
//            ├*_STERTCAM ◀┤    ▼           ┃      ┃
//  *_INVALID◀┴ *_ADDCAM  ◀┴─┏TMTV_RUNNINGCAM┓ ◀━┛
//          └────────▶ ┗TMTV_STARTWARN ┛
//
//==============================================================================
///</algorithm_info>

///<header_info>
//==============================================================================
#pragma once
#include "CommonInclude.h"
#include "CameraObject.h"
#include "SendServer.h"
#include "ReceiveServer.h"
#include <queue>
//==============================================================================
///</header_info>

///<datastruct_info>
//==============================================================================
//Xml配置文件按照此结构存储, 读取后再逐步启动程序
class CameraManagerSetting
{
public:
	long m_SleepTime=0;
	SendServerSetting m_SendServerSetting;
	ReceiveServerSetting m_ReceiveServerSetting;
public:
	CameraManagerSetting() {}
	CameraManagerSetting(const CameraManagerSetting& cameraManagerSetting)
	{
		m_SendServerSetting = cameraManagerSetting.m_SendServerSetting;
		m_ReceiveServerSetting = cameraManagerSetting.m_ReceiveServerSetting;
		m_SleepTime = cameraManagerSetting.m_SleepTime;
	}
	CameraManagerSetting& operator= (const CameraManagerSetting& cameraManagerSetting)
	{
		m_SendServerSetting = cameraManagerSetting.m_SendServerSetting;
		m_ReceiveServerSetting = cameraManagerSetting.m_ReceiveServerSetting;
		m_SleepTime = cameraManagerSetting.m_SleepTime;
	}
	bool LoadSetting(PATHWSTR xmlFilePath);
	bool SaveSetting(PATHWSTR xmlFilePath);
};
//==============================================================================
///</datastruct_info>


///<class_info>
//==============================================================================
//程序管理类
class CameraManager: public Thread
{
public:
	CameraManager(int maxCameraNum = MAXCAMNUM);
	~CameraManager();
	CameraManagerSetting m_CameraManagerSetting;
	void Initial(CameraManagerSetting cameraManagerSetting);
	void Initial();
	void Unitial();
public://Socket异步消息收发对象
	SendServer m_SendServer;
    ReceiveServer m_ReceiveServer;
public://相机、算法操作命令
	enum { MAXCAMNUM = 128 };
	int m_MaxCameraNum;
	vector<CameraObject*> m_CameraObjectVector;
	CameraObject* GetCamServer(int CamIndex);
	CameraObject* GetCamServer(Tmtv_CameraInfo& cameraInfo);
	CameraObject* GetCamServer(Tmtv_CameraInfo& cameraInfo);
	//载入参数
	bool LoadSetting(PATHWSTR xmlFilePath);
	//保存参数
	bool SaveSetting(PATHWSTR xmlFilePath);
	//添加相机
	bool AddCamera(Tmtv_CameraInfo& cameraInfo);
	//删除相机
	bool DelCamera(Tmtv_CameraInfo& cameraInfo);
	//打开相机
	bool StartCamera(Tmtv_CameraInfo& cameraInfo);
	//停止相机
	bool StopCamera(Tmtv_CameraInfo& cameraInfo);
	//设置相机
	bool SetCamera(Tmtv_CameraInfo& cameraInfo);
	//查询相机
	bool GetCamera(Tmtv_CameraInfo& cameraInfo);
	//打开相机算法
	bool StartAlgorithm(Tmtv_CameraInfo& cameraInfo);
	//停止相机算法
	bool StopAlgorithm(Tmtv_CameraInfo& cameraInfo);
	//停止相机算法
	bool SetAlgorithm(Tmtv_CameraInfo& cameraInfo);

public://发送消息交由SendServer处理
	int SendMsgInfo(Tmtv_MsgInfo& msgInfo);
	bool SendImage(Tmtv_ImageInfo& imgInfo);
	//由ReceiveServer接受消息的本地处理
	void Task();
};
//==============================================================================
///</class_info>