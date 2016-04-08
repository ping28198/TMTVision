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
//��������: Detectorͼ�����㷨�ӿ���
class Detector
{
public:
	enum {SETTINGSIZE = 1024,PERDETECTNUM=8};
public:
	//Ԥ��׼��������ͼ��
	Mat *p_maskImageData;
	int m_imageHeight;
	int m_imageWidth;
	//��ͬ�㷨�����ṹ��ͬ,������Tmtv_AlgorithmInfo�ṹ
	long algorithmInfoSize;
	Tmtv_AlgorithmInfo *p_AlgorithmInfo;
	long m_DetectedNum;
	Detector();
	~Detector();
	//��ʼ����Դ������ͼ��
	bool Initial(Tmtv_AlgorithmInfo *pAlgorithmInfo);
	//ж����Դ������ͼ��
	void Unitial();
	//�����㷨
	void Reset(Tmtv_AlgorithmInfo *pAlgorithmInfo);
	//ʶ��ǰͼ�����
	virtual bool Detect(Mat& srcImageData, Mat& rectImageData, 
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
struct Tmtv_BackgroundDetectorInfo: Tmtv_AlgorithmInfo	//�㷨��Ϣ
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
//��������: BackgroundDetector����ͼ�����㷨��
class BackgroundDetector:public Detector
{
public:
	BackgroundDetector();
	~BackgroundDetector();
	//��ʼ����Դ������ͼ��
	bool Initial(Tmtv_AlgorithmInfo *pAlgorithmInfo);
	//ж����Դ������ͼ��
	void Unitial();
	//�����㷨
	void Reset(Tmtv_AlgorithmInfo *pAlgorithmInfo);
	

	Ptr<BackgroundSubtractorMOG2> p_backgroundSubtractor;
	struct RunningPara
	{
		double minArea;
		double learnRatio;
	};
	//ʶ��ǰͼ�����
	bool Detect(Mat& srcImageData, Mat& rectImageData,
		Tmtv_DefectInfo & defects,
		void* paras = 0, long paraSize = 0);
	//ʶ��ǰͼ�����
	bool Detect(PATHSTR srcImagePath, PATHSTR rectImagePath,
		Tmtv_DefectInfo & defects,
		void* paras = 0, long paraSize = 0);
};
//==============================================================================
///</class_info>