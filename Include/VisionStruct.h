///<proj_info>
//==============================================================================
// 项目名 : 智能监控
// 文件名 : VisionStruct.h
// 作  者 : 任威平
// 用  途 : 主要结构体
// 版  权 : 霍比特人
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间      描述
//1.0     任威平      2016.3.20  整合旧版本
//1.1     王磊        2016.3.29  增加相机分辨率
//                               增加消息枚举
//                               增加句柄供输出调试信息
//1.2     王磊        2016.3.29  增加相机状态空间管理,对应于相机管理消息
//2.0     王磊        2016.3.29  *待增加算法状态空间管理
//2.1     王磊        2016.3.29  实现算法状态空间管理
//2.2	  任威平		 2016.4.18   添加主机地址到相机信息
//==============================================================================
///</ver_info>

///<header_info>
//==============================================================================
#pragma once
#include "CommonDefine.h" 
#include "WinSock2.h"
#include <iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"WS2_32.lib")
//==============================================================================
///</header_info>

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


//struct sockaddr_in {
//	short   sin_family;
//	u_short sin_port;
//	struct  in_addr sin_addr;
//	char    sin_zero[8];
//};



///<datastruct_info>
//==============================================================================
struct Tmtv_AlgorithmInfo	//算法信息
{
public:
	long structSize = sizeof(Tmtv_AlgorithmInfo);//若含有较大数据, 修改此大小
	PATHSTR MaskImgPath = "";//掩码图像位置
	PATHSTR DstImgPath = "";//目标图像位置
	enum {//2.0
		TMTV_NOWARN = 0,     //不启动算法
		TMTV_PREWARN = 1,    //启动算法过程中
		TMTV_STARTWARN = 2,  //已启动的算法
	};
	int WarnningLevel = 0;//警告等级,设置算法启动状态
	int mAlgoStatus = TMTV_NOWARN;
	LONGSTR Reservechar="";//保留
public:
	Tmtv_AlgorithmInfo()
	{
		structSize = sizeof(Tmtv_AlgorithmInfo);
	}
	Tmtv_AlgorithmInfo(const Tmtv_AlgorithmInfo& algorithmInfo)
	{
		structSize = sizeof(Tmtv_AlgorithmInfo);
		strcpy_s(MaskImgPath, TMTV_PATHSTRLEN, algorithmInfo.MaskImgPath);
		strcpy_s(DstImgPath, TMTV_PATHSTRLEN, algorithmInfo.DstImgPath);
		WarnningLevel = algorithmInfo.WarnningLevel;
		mAlgoStatus = algorithmInfo.mAlgoStatus;
		strcpy_s(Reservechar, TMTV_LONGSTRLEN, algorithmInfo.Reservechar);
	}
	Tmtv_AlgorithmInfo& operator= (const Tmtv_AlgorithmInfo& algorithmInfo)
	{
		structSize = sizeof(Tmtv_AlgorithmInfo);
		strcpy_s(MaskImgPath, TMTV_PATHSTRLEN, algorithmInfo.MaskImgPath);
		strcpy_s(DstImgPath, TMTV_PATHSTRLEN, algorithmInfo.DstImgPath);
		WarnningLevel = algorithmInfo.WarnningLevel;
		mAlgoStatus = algorithmInfo.mAlgoStatus;
		strcpy_s(Reservechar, TMTV_LONGSTRLEN, algorithmInfo.Reservechar);
		return *this;
	}
};
//==============================================================================
///</datastruct_info>

///<datastruct_info>
//==============================================================================
struct Tmtv_CameraInfo //相机信息
{
	int Indexnum = 0; //相机序号
	SHORTSTR CameraName = "";//相机名称等信息
	PATHSTR CameraPath = "";//相机根目录路径
	NetIP CameraHost = "";//相机所在主机
	double CameraPos[8];//x,y,z坐标, 方向
	int CameraWidth = 1920;//1.1 //相机图片宽度
	int CameraHeight = 1080;//1.1 //相机图片高度	
	enum {//1.2
		TMTV_NOCAM = 300,      //未加载的相机
		TMTV_STOPEDCAM = 301,  //已加载的暂停相机
		TMTV_RUNNINGCAM = 302, //已加载的运行相机
	};
	int Status= TMTV_NOCAM;//1.2//相机状态	
	int WaiteTime=1000;//1.2//相机状态
	Tmtv_AlgorithmInfo AlgorithmInfo;
public:
	Tmtv_CameraInfo(){}
	Tmtv_CameraInfo(const Tmtv_CameraInfo& cameraInfo)
	{
		Indexnum = cameraInfo.Indexnum;
		strcpy_s(CameraName, TMTV_SHORTSTRLEN, cameraInfo.CameraName);
		strcpy_s(CameraPath, TMTV_PATHSTRLEN, cameraInfo.CameraPath);
		strcpy_s(CameraHost, TMTV_IPSTRLEN, cameraInfo.CameraHost);
		for (int i = 0; i < 8;i++)
		{
			CameraPos[i] = cameraInfo.CameraPos[i];
		}
		CameraWidth = cameraInfo.CameraWidth;
		CameraHeight = cameraInfo.CameraHeight;
		Status = cameraInfo.Status;
		WaiteTime = cameraInfo.WaiteTime;
		AlgorithmInfo = cameraInfo.AlgorithmInfo;
	}
	Tmtv_CameraInfo& operator= (const Tmtv_CameraInfo& cameraInfo)
	{
		Indexnum = cameraInfo.Indexnum;
		strcpy_s(CameraName, TMTV_SHORTSTRLEN, cameraInfo.CameraName);
		strcpy_s(CameraPath, TMTV_PATHSTRLEN, cameraInfo.CameraPath);
		strcpy_s(CameraHost, TMTV_IPSTRLEN, cameraInfo.CameraHost);
		for (int i = 0; i < 8; i++)
		{
			CameraPos[i] = cameraInfo.CameraPos[i];
		}
		CameraWidth = cameraInfo.CameraWidth;
		CameraHeight = cameraInfo.CameraHeight;
		Status = cameraInfo.Status;
		WaiteTime = cameraInfo.WaiteTime;
		AlgorithmInfo = cameraInfo.AlgorithmInfo;
		return *this;
	}
};
//==============================================================================
///</datastruct_info>

///<datastruct_info>
//==============================================================================
struct Tmtv_DefectInfo	//缺陷信息
{
	int ImgWidth = 1920;
	int ImgHeight = 1080;//指定图片大小, 便于图片尺寸调整之后绘制缺陷信息
	int DefectNum = 0;   //缺陷数量
	int DefectPos[TMTV_MAXDEFECTNUM][8];//left,top,width,height,type,level,..
};
//==============================================================================
///</datastruct_info>

///<datastruct_info>
//==============================================================================
//struct Tmtv_ImageInfo	//图像信息, 相机服务端发往主程序端
//{
//	Tmtv_CameraInfo mCameraInfo;
//	Tmtv_DefectInfo mDefectInfo;
//	PATHSTR ImagePath = "";//文件路径
//	TINYSTR GrabTime = "";//采集时间 格式: 年-月-日-时:分: 秒. 例: 2016-03-22-12:00:00
//	int IsWarnning = 0;//是否有警告
//	int IsVIP = 0;//设置是否是重点
//	LONGSTR Reservechar;//保留
//};
struct Tmtv_ImageInfo	//图像信息, 相机服务端发往主程序端//2.0
{
	int mCamId = 0;
	Tmtv_DefectInfo mDefectInfo;
	PATHSTR ImagePath = "";//文件路径
	TINYSTR GrabTime = "";//采集时间 格式: 年-月-日-时:分: 秒. 例: 2016-03-22-12:00:00
	LONGSTR Reservechar;//保留
};
//==============================================================================
///</datastruct_info>

struct Tmtv_BaseNetMessage
{
	unsigned long structSize = sizeof(Tmtv_BaseNetMessage);
	int CheckCode = TMTV_CHECKCODE;//验证码, 防止通信干扰
	enum {
		TMTV_ADDCAM = 100,        //添加CameraInfo指定的未加载相机
		TMTV_DELCAM = 101,        //删除CameraInfo指定的未加载相机
		TMTV_STARTCAM = 102,      //启动CameraInfo指定的已加载相机
		TMTV_STOPCAM = 103,       //暂定CameraInfo指定的已加载相机
		TMTV_GETCAM = 104,        //查询CameraInfo指定的已加载相机的指定参数
		TMTV_SETCAM = 105,        //设置CameraInfo指定的已加载相机的指定参数
		TMTV_STARTALGO = 106,     //启动CameraInfo指定的算法
		TMTV_STOPCALGO = 107,      //暂定CameraInfo指定的算法
		TMTV_SETALGO = 108,      //暂定CameraInfo指定的算法
		TMTV_GETALLCAM = 150,		//获取所有相机状态
		TMTV_GETALLIMG = 151,		//获取所有相机最新图片

		TMTV_ADDCAM_OK = 200,        //添加CameraInfo指定的未加载相机成功
		TMTV_DELCAM_OK = 201,        //删除CameraInfo指定的未加载相机成功
		TMTV_STARTCAM_OK = 202,      //启动CameraInfo指定的已加载相机成功
		TMTV_STOPCAM_OK = 203,       //暂定CameraInfo指定的已加载相机成功
		TMTV_GETCAM_OK = 204,        //查询CameraInfo指定的已加载相机的指定参数成功
		TMTV_SETCAM_OK = 205,        //设置CameraInfo指定的已加载相机的指定参数成功
		TMTV_STARTALGO_OK = 206,     //启动算法成功
		TMTV_STOPALGO_OK = 207,      //暂定算法成功
		TMTV_SETALGO_OK = 208,       //设定算法成功
		TMTV_SENDALLCAMOK = 209,
		TMTV_SENDALLIMGOK = 210,

		TMTV_ADDCAM_FAIL = 240,      //添加CameraInfo指定的未加载相机未成功
		TMTV_DELCAM_FAIL,      //删除CameraInfo指定的未加载相机未成功
		TMTV_STARTCAM_FAIL,    //启动CameraInfo指定的已加载相机未成功
		TMTV_STOPCAM_FAIL,     //暂定CameraInfo指定的已加载相机未成功
		TMTV_GETCAM_FAIL,      //查询CameraInfo指定的已加载相机的指定参数未成功
		TMTV_SETCAM_FAIL,      //设置CameraInfo指定的已加载相机的指定参数未成功
		TMTV_STARTALGO_FAIL,   //启动算法未成功
		TMTV_STOPALGO_FAIL,    //暂定算法未成功
		TMTV_SETALGO_FAIL,       //设定算法成功

		TMTV_INVALID = 260,          //返回非法的命令
		TMTV_SNAPED = 298,           //推送实时mImgInfo图片
		TMTV_DETECTED = 299,         //推送检测到的的mImgInfo图片+缺陷
		TMTV_CAMINFO = 300,
	};
	int MsgType = 0;
	HANDLE hAskHandle;   //1.1//请求线程句柄,=0无效,用于调试,暂时程序不调用
	HANDLE hAnswerHandle;//1.1//应答线程句柄,=0无效,用于调试,暂时程序不调用	
	sockaddr_in mAddr;		//告知接收方，我方的接收地址
	sockaddr_in dstAddr;		//消息发送的目标地址
	unsigned long ElementCount = 0;		//跟随的消息元素数量
	unsigned long ElementLength = 0;   //跟随的消息单个元素长度
};





///<datastruct_info>
//==============================================================================
struct Tmtv_AskInfo//指令信息, 主程序端发往相机服务端
{
	long structSize = sizeof(Tmtv_AskInfo);//若含有较大数据, 修改此大小
	int CheckCode = TMTV_CHECKCODE;//验证码, 防止通信干扰
	enum {
		TMTV_ADDCAM = 100,        //添加CameraInfo指定的未加载相机
		TMTV_DELCAM = 101,        //删除CameraInfo指定的未加载相机
		TMTV_STARTCAM = 102,      //启动CameraInfo指定的已加载相机
		TMTV_STOPCAM = 103,       //暂定CameraInfo指定的已加载相机
		TMTV_GETCAM = 104,        //查询CameraInfo指定的已加载相机的指定参数
		TMTV_SETCAM = 105,        //设置CameraInfo指定的已加载相机的指定参数
		TMTV_STARTALGO = 106,     //启动CameraInfo指定的算法
		TMTV_STOPCALGO = 107,      //暂定CameraInfo指定的算法
		TMTV_SETALGO = 108,      //暂定CameraInfo指定的算法
		TMTV_GETALLCAM = 150,		//获取所有相机状态
		TMTV_GETALLIMG = 151,		//获取所有相机最新图片
	};
	int Asktype=0;
	HANDLE hAskHandle;   //1.1//请求线程句柄,=0无效,用于调试,暂时程序不调用
	HANDLE hAnswerHandle;//1.1//应答线程句柄,=0无效,用于调试,暂时程序不调用	
	Tmtv_CameraInfo CameraInfo;
};
//==============================================================================
///</datastruct_info>

///<datastruct_info>
//==============================================================================
struct Tmtv_MsgInfo//指令信息, 相机服务端发往主程序端
{
	long structSize = sizeof(Tmtv_MsgInfo);//若含有较大数据, 修改此大小
	int CheckCode = TMTV_CHECKCODE;//验证码, 防止通信干扰
	enum {
		TMTV_ADDCAM_OK = 200,        //添加CameraInfo指定的未加载相机成功
		TMTV_DELCAM_OK = 201,        //删除CameraInfo指定的未加载相机成功
		TMTV_STARTCAM_OK = 202,      //启动CameraInfo指定的已加载相机成功
		TMTV_STOPCAM_OK = 203,       //暂定CameraInfo指定的已加载相机成功
		TMTV_GETCAM_OK = 204,        //查询CameraInfo指定的已加载相机的指定参数成功
		TMTV_SETCAM_OK = 205,        //设置CameraInfo指定的已加载相机的指定参数成功
		TMTV_STARTALGO_OK = 206,     //启动算法成功
		TMTV_STOPALGO_OK = 207,      //暂定算法成功
		TMTV_SETALGO_OK = 208,       //设定算法成功
		TMTV_SENDALLCAMOK=209,
		TMTV_SENDALLIMGOK=210,

		TMTV_ADDCAM_FAIL = 240,      //添加CameraInfo指定的未加载相机未成功
		TMTV_DELCAM_FAIL,      //删除CameraInfo指定的未加载相机未成功
		TMTV_STARTCAM_FAIL,    //启动CameraInfo指定的已加载相机未成功
		TMTV_STOPCAM_FAIL,     //暂定CameraInfo指定的已加载相机未成功
		TMTV_GETCAM_FAIL,      //查询CameraInfo指定的已加载相机的指定参数未成功
		TMTV_SETCAM_FAIL,      //设置CameraInfo指定的已加载相机的指定参数未成功
		TMTV_STARTALGO_FAIL,   //启动算法未成功
		TMTV_STOPALGO_FAIL,    //暂定算法未成功
		TMTV_SETALGO_FAIL,       //设定算法成功

		TMTV_INVALID = 260,          //返回非法的命令
		TMTV_SNAPED = 298,           //推送实时mImgInfo图片
		TMTV_DETECTED = 299,         //推送检测到的的mImgInfo图片+缺陷
		TMTV_CAMINFO = 300,
	};
	int MsgType = 0;
	int MsgNum = 1;					//消息数量，用于一次传输多个消息
	HANDLE hAskHandle;            //1.1//请求线程句柄,=0无效,用于调试,暂时程序不调用
	HANDLE hAnswerHandle;         //1.1//应答线程句柄,=0无效,用于调试,暂时程序不调用
	//Tmtv_CameraInfo CameraInfo;//1.1//冗余的数据
	Tmtv_ImageInfo ImgInfo;
};
//==============================================================================
///</datastruct_info>


///<datastruct_info>
//==============================================================================
struct Tmt_UserInfo
{
	LONGSTR UserName;		//用户名
	LONGSTR PassWord;		//密码
	LONGSTR mCamWatch;
	LONGSTR mWeChat_id;
	int AuthorityLevel = 0;	//权限等级
};
//==============================================================================
///</datastruct_info>

///<datastruct_info>
//==============================================================================
struct Tmt_ClientInfo //客户端信息
{
	NetIP mIpAddr;		//IP
	int mport;		//port
	int status;		//工作状态
	enum {
		TMT_CLIENT_RUNNING = 1,
		TMT_CLIENT_CLOSE,
	};
};
//==============================================================================
///</datastruct_info>