///<proj_info>
//==============================================================================
// 项目名 ：智能监控
// 文件名 ：VisionStruct.h
// 作  者 ：任威平
// 用  途 ：主要结构体
// 版  权 ：霍比特人
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间      描述
//1.0     任威平      2016.3.20  整合旧版本
//1.1     王磊        2016.3.29  增加相机分辨率
//                               增加消息枚举
//==============================================================================
///</ver_info>

#pragma once
#include "CommonDefine.h" 


struct Tmtv_CameraInfo //相机信息
{
	int Indexnum=0;//相机序号
	char CameraName[TMTV_MAXSTRLEN];//相机名称等信息
	char CameraPath[TMTV_MAXSTRLEN];//相机根目录路径
	char MaskImgPath[TMTV_MAXSTRLEN];//掩码图像位置
	int CameraPos[8];//x,y,z坐标，方向
	int CameraWidth = 1920;//1.1 //相机图片宽度
	int CameraHeight = 1080;//1.1 //相机图片高度
	int WarnningLevel;//警告等级
	char Reservechar[TMTV_MAXSTRLEN];//保留
};

struct Tmtv_DefectInfo	//缺陷信息
{
	int DefectNum=0;//缺陷数量
	int DefectPos[TMTV_MAXDEFECTNUM][8];//left,top,width,height,type,level,..
	int ImgWidth=1920;
	int ImgHeight=1080;//指定图片大小，便于图片尺寸调整之后绘制缺陷信息
};

struct Tmtv_ImageInfo	//图像信息，相机服务端发往主程序端
{
	Tmtv_CameraInfo mCameraInfo;
	Tmtv_DefectInfo mDefectInfo;
	char ImagePath[TMTV_MAXSTRLEN];//文件路径
	char GrabTime[64];//采集时间 格式：年-月-日-时:分：秒。例：2016-03-22-12:00:00
	int IsWarnning=0;//是否有警告
	int IsVIP=0;//设置是否是重点
	char Reservechar[TMTV_MAXSTRLEN];//保留
};

struct Tmtv_AskInfo//指令信息，主程序端发往相机服务端
{
	int CheckCode = TMTV_CHECKCODE;//验证码，防止通信干扰
    //请求类型，101：关闭CameraInfo指定的相机，102：请求发送CameraInfo指定的相机最新数据，103：添加CameraInfo指定的相机
	//104：修改CameraInfo相机号指定的相机信息
	enum { 
		   TMTV_ADDCAM = 100,        //添加CameraInfo指定的未加载相机
		   TMTV_DELCAM = 101,        //删除CameraInfo指定的未加载相机
		   TMTV_STERTCAM = 102,      //启动CameraInfo指定的已加载相机
		   TMTV_STOPCAM = 103,       //暂定CameraInfo指定的已加载相机
		   TMTV_GETCAM = 104,        //查询CameraInfo指定的已加载相机的指定参数
		   TMTV_SETCAM = 105,        //设置CameraInfo指定的已加载相机的指定参数
	};
	int Asktype;
	Tmtv_CameraInfo CameraInfo;
	DWORD hAskHandle;   //请求线程句柄,=0无效,用于调试,暂时程序不调用
	DWORD hAnswerHandle;//应答线程句柄,=0无效,用于调试,暂时程序不调用
};
struct Tmtv_MsgInfo//指令信息，相机服务端发往主程序端
{
	int CheckCode = TMTV_CHECKCODE;//验证码，防止通信干扰
	enum {
		   TMTV_ADDCAM_OK = 200,        //添加CameraInfo指定的未加载相机成功
		   TMTV_DELCAM_OK = 201,        //删除CameraInfo指定的未加载相机成功
		   TMTV_STERTCAM_OK = 202,      //启动CameraInfo指定的已加载相机成功
		   TMTV_STOPCAM_OK = 203,       //暂定CameraInfo指定的已加载相机成功
		   TMTV_GETCAM_OK = 204,        //查询CameraInfo指定的已加载相机的指定参数成功
		   TMTV_SETCAM_OK = 205,        //设置CameraInfo指定的已加载相机的指定参数成功
		   TMTV_SNAPED= 298,            //新检测到的mImgInfo图片
	};
	int MsgType;
	//Tmtv_CameraInfo CameraInfo;//冗余的数据
	Tmtv_ImageInfo ImgInfo;
	DWORD hAskHandle;               //请求线程句柄,=0无效,用于调试,暂时程序不调用
	DWORD hAnswerHandle;            //应答线程句柄,=0无效,用于调试,暂时程序不调用
};