///<proj_info>
//==============================================================================
// 项目名 : 智能监控
// 文件名 : Detector.h
// 作  者 : 王磊
// 用  途 : Detector图像检测算法接口类
//          BackgroundDetector背景图像检测算法类
// 版  权 : 霍比特人
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间      描述
//1.0     王磊        2016.3.20  整合旧版本
//2.0     王磊        2016.4.3   修改数据结构和算法参数,整合到接口类中
//==============================================================================
///</ver_info>

///<header_info>
//==============================================================================
//添加所需的头文件
#pragma once
#include "cv.h"
#include "opencv2/video/background_segm.hpp"
#include "CommonDefine.h"
#include "VisionStruct.h"
using namespace std;
using namespace cv;
//==============================================================================
///</header_info>

///<class_info>
//==============================================================================
//功能描述: Detector图像检测算法接口类
class Detector
{
public:
	enum {SETTINGSIZE = 1024,PERDETECTNUM=8};
public:
	//预先准备的掩码图像
	Mat *p_maskImageData;
	int m_imageHeight;
	int m_imageWidth;
	//不同算法参数结构不同,派生自Tmtv_AlgorithmInfo结构
	long algorithmInfoSize;
	Tmtv_AlgorithmInfo *p_AlgorithmInfo;
	long m_DetectedNum;
	Detector();
	~Detector();
	//初始化资源和掩码图像
	bool Initial(Tmtv_AlgorithmInfo *pAlgorithmInfo);
	//卸载资源和掩码图像
	void Unitial();
	//重设算法
	void Reset(Tmtv_AlgorithmInfo *pAlgorithmInfo);
	//识别当前图像队列
	virtual bool Detect(Mat& srcImageData, Mat& rectImageData, 
		Tmtv_DefectInfo & defects,
		void* paras=0, long paraSize=0)=0;
	//识别当前图像队列
	bool Detect(PATHSTR srcImagePath, PATHSTR rectImagePath,
		Tmtv_DefectInfo & defects,
		void* paras = 0, long paraSize = 0);
};
//==============================================================================
///</class_info>

///<datastruct_info>
//==============================================================================
struct Tmtv_BackgroundDetectorInfo: Tmtv_AlgorithmInfo	//算法信息
{
	int STRLEN = 260;
	int GAUSSIANSIZE = 5;
	double GAUSSIANSIGMA = 0.5;
	int MORPHSIZE1 = 3;
	int MORPHSIZE2 = 3;
	int MORPHSIZE3 = 3;
	int MORPHSIZE4 = 3;
	int MORPHSIZE5 = 5;
	int MORPHSIZE6 = 5;
	int MORPHSIZE7 = 5;
	int THEREHOLD = 30;
	Tmtv_BackgroundDetectorInfo()
	{
		structSize = sizeof(Tmtv_BackgroundDetectorInfo);
	};
	~Tmtv_BackgroundDetectorInfo()
	{
		int a = 0;
	}
};
//==============================================================================
///</datastruct_info>

///<class_info>
//==============================================================================
//功能描述: BackgroundDetector背景图像检测算法类
class BackgroundDetector:public Detector
{
public:
	BackgroundDetector();
	~BackgroundDetector();
	//初始化资源和掩码图像
	bool Initial(Tmtv_AlgorithmInfo *pAlgorithmInfo);
	//卸载资源和掩码图像
	void Unitial();
	//重设算法
	void Reset(Tmtv_AlgorithmInfo *pAlgorithmInfo);
	

	Ptr<BackgroundSubtractorMOG2> p_backgroundSubtractor;
	struct RunningPara
	{
		double minArea;
		double learnRatio;
	};
	//识别当前图像队列
	bool Detect(Mat& srcImageData, Mat& rectImageData,
		Tmtv_DefectInfo & defects,
		void* paras = 0, long paraSize = 0);
	//识别当前图像队列
	bool Detect(PATHSTR srcImagePath, PATHSTR rectImagePath,
		Tmtv_DefectInfo & defects,
		void* paras = 0, long paraSize = 0);
};
//==============================================================================
///</class_info>