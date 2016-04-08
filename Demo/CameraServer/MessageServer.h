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
//#include "TmtSocket.h"
//#include "VisionStruct.h"
//#include "CameraServer.h"
#include "CommonInclude.h"
//==============================================================================
///</header_info>

///<class_info>
//==============================================================================
//程序管理类
class CameraServer;
class MessageServer :public TmtSocketServer
{
public:
	enum {MAXCAMNUM = 128 };
	MessageServer(int bufferSize = MAXBUFFERSIZE);
	~MessageServer();
	static MessageServer& GetState()
	{
		static MessageServer obj;
		return obj;
	}
public://相机、算法操作命令
	vector<CameraServer*> m_CameraServerVector;
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
	//停止相机
	bool SetCamera(Tmtv_CameraInfo& cameraInfo);
	//停止相机
	bool GetCamera(Tmtv_CameraInfo& cameraInfo);
	//打开相机算法
	bool StartAlgorithm(Tmtv_CameraInfo& cameraInfo);
	//停止相机算法
	bool StopAlgorithm(Tmtv_CameraInfo& cameraInfo);
	//停止相机算法
	bool SetAlgorithm(Tmtv_CameraInfo& cameraInfo);
public://返回消息
	int SendMsgInfo(Tmtv_MsgInfo& msgInfo);
	bool SendImage(Tmtv_ImageInfo& imgInfo);
	
	void ServerProcess(int revLen);

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
