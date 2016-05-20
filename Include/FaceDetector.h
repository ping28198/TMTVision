#pragma once
#include "Detector.h"
//opencv头文件
#include "opencv2/objdetect/objdetect.hpp"
//标准模板头文件
#include <map>
#include <vector>

using namespace cv;
using namespace std;
///<datastruct_info>
//==============================================================================
//仅用于算法内部
struct Tmtv_FaceDetectorInfo : public Tmtv_AlgorithmInfo	//算法信息
{
	//MEGASTR Reservechar = {
	PATHSTR CascadeClassifierFileName = "";
	//}
public:
	Tmtv_FaceDetectorInfo()
	{
		structSize = sizeof(Tmtv_BackgroundDetectorInfo);
	}
	Tmtv_FaceDetectorInfo(const Tmtv_AlgorithmInfo& algorithmInfo);
	Tmtv_FaceDetectorInfo& operator= (const Tmtv_AlgorithmInfo& algorithmInfo);
};
//==============================================================================
///</datastruct_info>

///<class_info>
//==============================================================================
//功能描述: BackgroundDetector背景图像检测算法类
class FaceDetector :public Detector
{
public:
	FaceDetector();
	~FaceDetector();
	//不同算法参数结构不同,派生自Tmtv_AlgorithmInfo结构
	Tmtv_FaceDetectorInfo m_FaceDetectorInfo;
	//初始化资源和掩码图像
	bool Initial(Tmtv_AlgorithmInfo algorithmInfo);
	//卸载资源和掩码图像
	void Unitial();
	//重设算法
	void Reset(Tmtv_AlgorithmInfo algorithmInfo);


	cv::CascadeClassifier m_CascadeClassifier;
	//struct RunningPara
	//{
	//	double minArea;
	//	double learnRatio;
	//};
	//识别当前图像队列
	bool Detect(cv::Mat& srcImageData, cv::Mat& rectImageData,
		Tmtv_DefectInfo & defects,
		void* paras = 0, long paraSize = 0);
	//识别当前图像队列
	bool Detect(PATHSTR srcImagePath, PATHSTR rectImagePath,
		Tmtv_DefectInfo & defects,
		void* paras = 0, long paraSize = 0);
};
//==============================================================================
///</class_info>
