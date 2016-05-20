#pragma once
#include "Detector.h"
//opencvͷ�ļ�
#include "opencv2/objdetect/objdetect.hpp"
//��׼ģ��ͷ�ļ�
#include <map>
#include <vector>

using namespace cv;
using namespace std;
///<datastruct_info>
//==============================================================================
//�������㷨�ڲ�
struct Tmtv_FaceDetectorInfo : public Tmtv_AlgorithmInfo	//�㷨��Ϣ
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
//��������: BackgroundDetector����ͼ�����㷨��
class FaceDetector :public Detector
{
public:
	FaceDetector();
	~FaceDetector();
	//��ͬ�㷨�����ṹ��ͬ,������Tmtv_AlgorithmInfo�ṹ
	Tmtv_FaceDetectorInfo m_FaceDetectorInfo;
	//��ʼ����Դ������ͼ��
	bool Initial(Tmtv_AlgorithmInfo algorithmInfo);
	//ж����Դ������ͼ��
	void Unitial();
	//�����㷨
	void Reset(Tmtv_AlgorithmInfo algorithmInfo);


	cv::CascadeClassifier m_CascadeClassifier;
	//struct RunningPara
	//{
	//	double minArea;
	//	double learnRatio;
	//};
	//ʶ��ǰͼ�����
	bool Detect(cv::Mat& srcImageData, cv::Mat& rectImageData,
		Tmtv_DefectInfo & defects,
		void* paras = 0, long paraSize = 0);
	//ʶ��ǰͼ�����
	bool Detect(PATHSTR srcImagePath, PATHSTR rectImagePath,
		Tmtv_DefectInfo & defects,
		void* paras = 0, long paraSize = 0);
};
//==============================================================================
///</class_info>
