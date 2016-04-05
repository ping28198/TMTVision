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
#include "VisionStruct.h"
#include "CameraServer.h"
#include "TmtSocket.h"
//==============================================================================
///</header_info>

///<class_info>
//==============================================================================
//程序管理类
class MessageServer :public TmtSocketServer
{
public:
	MessageServer();
	~MessageServer();
	static MessageServer& GetState()
	{
		static MessageServer obj;
		return obj;
	}
public://相机、算法操作命令
	Tmtv_CameraInfo *pCameraInfo;
	CameraServer *pCameraServer;
	int cameraNum;
	//载入参数
	bool LoadSetting(LONGWSTR xmlFilePath);
	//保存参数
	bool SaveSetting(LONGWSTR xmlFilePath);
	//添加相机
	bool AddCamera(Tmtv_CameraInfo cameraInfo);
	//删除相机
	bool DelCamera(Tmtv_CameraInfo cameraInfo);
	//打开相机
	bool StartCamera(Tmtv_CameraInfo cameraInfo);
	//停止相机
	bool StopCamera(Tmtv_CameraInfo cameraInfo);
	//停止相机
	bool SetCamera(Tmtv_CameraInfo cameraInfo);
	//打开相机算法
	bool StartAlgorithm(Tmtv_CameraInfo cameraInfo);
	//停止相机算法
	bool StopAlgorithm(Tmtv_CameraInfo cameraInfo);
	//停止相机算法
	bool SetAlgorithm(Tmtv_CameraInfo cameraInfo);
public://返回消息
	bool RevMsg(Tmtv_MsgInfo msgInfo);
	bool RevImage(Tmtv_ImageInfo imgInfo, HANDLE hAnswerHandle);

	void ServerProcess(int revLen);
};

