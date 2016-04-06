///<proj_info>
//==============================================================================
// 项目名 : 智能监控
// 文件名 : ObjToString.h
// 作  者 : 王磊
// 用  途 : 主要结构体转为字符串用于显示
// 版  权 : 霍比特人
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间      描述
//1.0     王磊        2016.4.6    创建
//==============================================================================
///</ver_info>

///<header_info>
//==============================================================================
#pragma once
#include "VisionStruct.h"
#include "CommonFunc.h"
//==============================================================================
///</header_info>

///<class_info>
//==============================================================================
//功能描述:结构体转换为字符串
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
class ObjToString
{
public:
	static void ToString(MEGAWSTR wstring, Tmtv_AlgorithmInfo algorithmInfo, int method = 0, int color = 37);
	static void ToString(MEGAWSTR wstring, Tmtv_CameraInfo  camraInfo, int method = 0, int color = 37);
	static void ToString(MEGAWSTR wstring, Tmtv_DefectInfo  defectInfo, int method = 0, int color = 37);
	static void ToString(MEGAWSTR wstring, Tmtv_ImageInfo  imageInfo, int method = 0, int color = 37);
	static void ToString(MEGAWSTR wstring, Tmtv_AskInfo  askInfo, int method = 0, int color = 37);
	static void ToString(MEGAWSTR wstring, Tmtv_MsgInfo  msgInfo, int method = 0, int color = 37);
	static int InfoCheck(void* pBuffer, long  bufferSize);
	static int ToString(MEGAWSTR wstring, void* pBuffer, long  bufferSize, int method = 0, int color = 37);
};
//==============================================================================
///</class_info>