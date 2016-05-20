///<proj_info>
//==============================================================================
// ��Ŀ�� : ���ܼ��
// �ļ��� : Detector.h
// ��  �� : ����
// ��  ; : Detectorͼ�����㷨�ӿ���
//          BackgroundDetector����ͼ�����㷨��
// ��  Ȩ : ��������
//==============================================================================
///</proj_info>

///<ver_info>
// �汾��¼	
//==============================================================================
//�汾��  ������Ա      ʱ��      ����
//1.0     ����        2016.3.20  ���Ͼɰ汾
//2.0     ����        2016.4.3   �޸����ݽṹ���㷨����,���ϵ��ӿ�����
//==============================================================================
///</ver_info>

///<header_info>
//==============================================================================
//��������ͷ�ļ�
#pragma once
//#include "cv.h"
//#include "opencv2/core.hpp"
#include "CommonDefine.h"
#include "VisionStruct.h"
#include "CommonFunc.h"
//opencvͷ�ļ�
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "cv.h"
#include "opencv2/video/background_segm.hpp"

//��׼ģ��ͷ�ļ�
#include <map>
#include <vector>

using namespace cv;
using namespace std;
//==============================================================================
///</header_info>

///<class_info>
//==============================================================================
//��������: Detectorͼ�����㷨�ӿ���
class Detector
{
public:
	enum {SETTINGSIZE = 1024,PERDETECTNUM=8};
public:
	//Ԥ��׼��������ͼ��
	cv::Mat *p_maskImageData;
	int m_imageHeight;
	int m_imageWidth;
	//��ͬ�㷨�����ṹ��ͬ,������Tmtv_AlgorithmInfo�ṹ
	Tmtv_AlgorithmInfo m_AlgorithmInfo;
	long m_DetectedNum;
	Detector();
	~Detector();
	//��ʼ����Դ������ͼ��
	bool Initial(Tmtv_AlgorithmInfo algorithmInfo);
	//ж����Դ������ͼ��
	void Unitial();
	//�����㷨
	void Reset(Tmtv_AlgorithmInfo algorithmInfo);
	//ʶ��ǰͼ�����
	virtual bool Detect(cv::Mat& srcImageData, cv::Mat& rectImageData,
		Tmtv_DefectInfo & defects,
		void* paras=0, long paraSize=0)=0;
	//ʶ��ǰͼ�����
	bool Detect(PATHSTR srcImagePath, PATHSTR rectImagePath,
		Tmtv_DefectInfo & defects,
		void* paras = 0, long paraSize = 0);
};
//==============================================================================
///</class_info>

///<datastruct_info>
//==============================================================================
//�������㷨�ڲ�
struct Tmtv_BackgroundDetectorInfo : Tmtv_AlgorithmInfo	//�㷨��Ϣ
{
	//MEGASTR Reservechar = {
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
    //}
public:
	Tmtv_BackgroundDetectorInfo() 
	{
		structSize = sizeof(Tmtv_BackgroundDetectorInfo);
	}
	Tmtv_BackgroundDetectorInfo(const Tmtv_AlgorithmInfo& algorithmInfo);
	Tmtv_BackgroundDetectorInfo& operator= (const Tmtv_AlgorithmInfo& algorithmInfo);
};
//==============================================================================
///</datastruct_info>

///<class_info>
//==============================================================================
//��������: BackgroundDetector����ͼ�����㷨��
class BackgroundDetector:public Detector
{
public:
	BackgroundDetector();
	~BackgroundDetector();
	//��ͬ�㷨�����ṹ��ͬ,������Tmtv_AlgorithmInfo�ṹ
	Tmtv_BackgroundDetectorInfo m_BackgroundDetectorInfo;
	//��ʼ����Դ������ͼ��
	bool Initial(Tmtv_AlgorithmInfo algorithmInfo);
	//ж����Դ������ͼ��
	void Unitial();
	//�����㷨
	void Reset(Tmtv_AlgorithmInfo algorithmInfo);
	

	cv::Ptr<cv::BackgroundSubtractorMOG2> p_backgroundSubtractor;
	struct RunningPara
	{
		double minArea;
		double learnRatio;
	};
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