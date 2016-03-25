#pragma once
#include "CommonDefine.h"


struct Tmtv_CameraInfo //相机信息
{
	int Indexnum=0;//相机序号
	char CameraName[TMTV_MAXSTRLEN];//相机名称等信息
	char CameraPath[TMTV_MAXSTRLEN];//相机根目录路径
	char MaskImgPath[TMTV_MAXSTRLEN];//掩码图像位置
	int CameraPos[8];//x,y,z坐标，方向
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
	int CheckCode = TMTV_CHECKCODE;//验证码，防止通信干扰
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
	int Asktype;//请求类型，100：启动CameraIndex相机，101：关闭CameraIndex相机，102：请求发送CameraIndex相机最新数据，103：
	int CameraIndex;//相机索引号
	int VerifyCode;//校验码，以上所有数据之和
};
