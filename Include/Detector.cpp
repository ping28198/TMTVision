#include "Detector.h"

Detector::Detector()
{
	m_imageHeight = 0;
	m_imageWidth = 0;
	p_maskImageData = 0;
	m_DetectedNum = 0;
}
Detector::~Detector() { Unitial(); }
//初始化资源和掩码图像
bool Detector::Initial(Tmtv_AlgorithmInfo algorithmInfo)
{
	m_DetectedNum = 0;
	m_AlgorithmInfo = algorithmInfo;
	cv::Mat tmpMat = cv::imread(m_AlgorithmInfo.MaskImgPath);
	if (tmpMat.empty()) return false;
	cv::Size mskSize = tmpMat.size();
	int mskDepth = tmpMat.depth();
	int mskChannels = tmpMat.channels();
	if (mskSize.height<4 ||
		mskSize.width<4 ||
		mskDepth != CV_8U ||
		mskChannels != 1) {
		return false;
	}
	m_imageHeight = mskSize.height;
	m_imageWidth = mskSize.width;
	p_maskImageData = new cv::Mat(m_imageHeight, m_imageWidth, CV_8UC1);
	if (p_maskImageData != 0)
	{
		tmpMat.copyTo(*p_maskImageData);
		return true;
	}
	return false;
}
//卸载资源和掩码图像
void Detector::Unitial()
{
	if (p_maskImageData != 0)
	{
		p_maskImageData->release();
		delete p_maskImageData;
		p_maskImageData = 0;
	}
	m_imageHeight = 0;
	m_imageWidth = 0;
	m_DetectedNum = 0;
}
//重设算法
void Detector::Reset(Tmtv_AlgorithmInfo algorithmInfo)
{
	Unitial();
	Initial(algorithmInfo);
}
//识别当前图像队列
bool Detector::Detect(PATHSTR srcImagePath, PATHSTR rectImagePath,
	Tmtv_DefectInfo & defects,
	void* paras, long paraSize)
{
	if (p_maskImageData == 0) return false;
	cv::Mat srcImageData = cv::imread(srcImagePath);
	cv::Mat rectImageData;
	srcImageData.copyTo(rectImageData);
	if (srcImageData.empty())
	{
		return false;
	}

	bool detected = Detect(srcImageData, rectImageData, defects, paras, paraSize);
	if (detected)
	{
		if (rectImagePath[0] != 0)
		{
			cv::imwrite(rectImagePath, srcImageData);
		}
		return true;
	}
	return false;
};


Tmtv_BackgroundDetectorInfo::Tmtv_BackgroundDetectorInfo(const Tmtv_AlgorithmInfo& algorithmInfo)
{
	structSize = sizeof(Tmtv_BackgroundDetectorInfo);
	strcpy_s(MaskImgPath, TMTV_PATHSTRLEN, algorithmInfo.MaskImgPath);
	strcpy_s(DstImgPath, TMTV_PATHSTRLEN, algorithmInfo.DstImgPath);
	WarnningLevel = algorithmInfo.WarnningLevel;
	mAlgoStatus = algorithmInfo.mAlgoStatus;
	strcpy_s(Reservechar, TMTV_LONGSTRLEN, algorithmInfo.Reservechar);
	//
	//CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "GAUSSIANSIZE", GAUSSIANSIZE);
	//CCommonFunc::GetStringDoublePara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "GAUSSIANSIGMA", GAUSSIANSIGMA);
	//CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "MORPHSIZE1", MORPHSIZE1);
	//CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "MORPHSIZE2", MORPHSIZE2);
	//CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "MORPHSIZE3", MORPHSIZE3);
	//CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "MORPHSIZE4", MORPHSIZE4);
	//CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "MORPHSIZE5", MORPHSIZE5);
	//CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "MORPHSIZE6", MORPHSIZE6);
	//CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "MORPHSIZE7", MORPHSIZE7);
	//CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "THEREHOLD", THEREHOLD);
}
Tmtv_BackgroundDetectorInfo& Tmtv_BackgroundDetectorInfo::operator= (const Tmtv_AlgorithmInfo& algorithmInfo)
{
	structSize = sizeof(Tmtv_BackgroundDetectorInfo);
	strcpy_s(MaskImgPath, TMTV_PATHSTRLEN, algorithmInfo.MaskImgPath);
	strcpy_s(DstImgPath, TMTV_PATHSTRLEN, algorithmInfo.DstImgPath);
	WarnningLevel = algorithmInfo.WarnningLevel;
	mAlgoStatus = algorithmInfo.mAlgoStatus;
	strcpy_s(Reservechar, TMTV_LONGSTRLEN, algorithmInfo.Reservechar);
	//
	CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "GAUSSIANSIZE", GAUSSIANSIZE);
	CCommonFunc::GetStringDoublePara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "GAUSSIANSIGMA", GAUSSIANSIGMA);
	CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "MORPHSIZE1", MORPHSIZE1);
	CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "MORPHSIZE2", MORPHSIZE2);
	CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "MORPHSIZE3", MORPHSIZE3);
	CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "MORPHSIZE4", MORPHSIZE4);
	CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "MORPHSIZE5", MORPHSIZE5);
	CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "MORPHSIZE6", MORPHSIZE6);
	CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "MORPHSIZE7", MORPHSIZE7);
	CCommonFunc::GetStringIntPara(algorithmInfo.Reservechar, TMTV_LONGSTRLEN, "THEREHOLD", THEREHOLD);
	return *this;
}


BackgroundDetector::BackgroundDetector()
{
	m_imageHeight = 0;
	m_imageWidth = 0;
	p_maskImageData = 0;
}
BackgroundDetector::~BackgroundDetector()
{
	Unitial(); 
}
//初始化资源和掩码图像
bool BackgroundDetector::Initial(Tmtv_AlgorithmInfo algorithmInfo)
{
	Detector::Initial(algorithmInfo);
	///<BackgroundSubtractorMOG2初始化>
	m_BackgroundDetectorInfo = algorithmInfo;
	if (!p_backgroundSubtractor.empty())
	{
		p_backgroundSubtractor->clear();
	}
	p_backgroundSubtractor.release();
	p_backgroundSubtractor = cv::createBackgroundSubtractorMOG2(500, m_BackgroundDetectorInfo.THEREHOLD, false);
	///</BackgroundSubtractorMOG2初始化>
	m_AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_STARTWARN;
	return false;
}
//卸载资源和掩码图像
void BackgroundDetector::Unitial()
{
	///<BackgroundSubtractorMOG2卸载>
	if (!p_backgroundSubtractor.empty())
	{
		p_backgroundSubtractor->clear();
	}
	p_backgroundSubtractor.release();
	///</BackgroundSubtractorMOG2卸载>
	Detector::Unitial();
}
//重设算法
void BackgroundDetector::Reset(Tmtv_AlgorithmInfo algorithmInfo)
{
	Unitial();
	Initial(algorithmInfo);
}

//识别当前图像队列
#define STRLEN 260
#define GAUSSIANSIZE 5
#define GAUSSIANSIGMA 0.5
#define MORPHSIZE1 3
#define MORPHSIZE2 3
#define MORPHSIZE3 3
#define MORPHSIZE4 3
#define MORPHSIZE5 5
#define MORPHSIZE6 5
#define MORPHSIZE7 5
#define THEREHOLD 30
bool BackgroundDetector::Detect(cv::Mat & srcImageData, cv::Mat & rectImageData, Tmtv_DefectInfo & defects, void * paras, long paraSize)
{
	std::vector<cv::Rect> rects;
	std::vector<double> areas;
	if (p_maskImageData == 0) return false;
	m_DetectedNum++;
	if (m_DetectedNum < PERDETECTNUM)
	{
		m_AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_PREWARN;
	}
	else
	{
		m_AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_STARTWARN;
	}
	RunningPara *pPara = (RunningPara *)paras;
	defects.ImgWidth = srcImageData.size().width;
	defects.ImgWidth = srcImageData.size().height;

	////转换为灰度图像
	//Mat grayImg;
	//cv::cvtColor(srcImageData, grayImg, COLOR_BGR2GRAY);
	double resizeRatio = 1.0;
	//图像缩放减少计算量
	cv::Mat *subImg = new cv::Mat;
	cv::Size dsize;
	cv::resize(srcImageData, *subImg, dsize, resizeRatio, resizeRatio, cv::INTER_LINEAR);
	//高斯模糊减少平移影响
	cv::Mat gaussImg;
	cv::Size maskSize;
	maskSize.width = GAUSSIANSIZE;
	maskSize.height = GAUSSIANSIZE;
	cv::GaussianBlur(*subImg, gaussImg, dsize, GAUSSIANSIGMA, GAUSSIANSIGMA);

	//背景监测算法
	p_backgroundSubtractor->apply(gaussImg, rectImageData, pPara->learnRatio);
	cv::Mat RGBRectImageData[3];
	cv::split(rectImageData, RGBRectImageData);
	cv::Mat RGBMaskImageData[3];
	cv::split(*p_maskImageData, RGBMaskImageData);
	//cv::min(RGBRectImageData[0], RGBMaskImageData[1], RGBMaskImageData[0]);
	//cv::min(RGBRectImageData[0], RGBMaskImageData[2], RGBMaskImageData[0]);

	RGBMaskImageData[1].copyTo(rectImageData);
	cv::Mat tmpImg;
	//开运算用来消除小物体,先腐蚀后膨胀
	cv::erode(rectImageData, tmpImg, cv::Mat(MORPHSIZE1, MORPHSIZE1, CV_8U), cv::Point(-1, -1), 1);
	cv::dilate(tmpImg, rectImageData, cv::Mat(MORPHSIZE2, MORPHSIZE2, CV_8U), cv::Point(-1, -1), 1);
	cv::erode(rectImageData, tmpImg, cv::Mat(MORPHSIZE1, MORPHSIZE1, CV_8U), cv::Point(-1, -1), 1);
	cv::dilate(tmpImg, rectImageData, cv::Mat(MORPHSIZE2, MORPHSIZE2, CV_8U), cv::Point(-1, -1), 1);

	//闭运算用来填充物体内细小空洞,先膨胀后腐蚀
	cv::dilate(rectImageData, tmpImg, cv::Mat(MORPHSIZE3, MORPHSIZE3, CV_8U), cv::Point(-1, -1), 1);
	cv::erode(tmpImg, rectImageData, cv::Mat(MORPHSIZE4, MORPHSIZE4, CV_8U), cv::Point(-1, -1), 1);
	cv::dilate(rectImageData, tmpImg, cv::Mat(MORPHSIZE3, MORPHSIZE3, CV_8U), cv::Point(-1, -1), 1);
	cv::erode(tmpImg, rectImageData, cv::Mat(MORPHSIZE4, MORPHSIZE4, CV_8U), cv::Point(-1, -1), 1);

	cv::dilate(rectImageData, tmpImg, cv::Mat(MORPHSIZE5, MORPHSIZE5, CV_8U), cv::Point(-1, -1), 1);
	cv::erode(tmpImg, rectImageData, cv::Mat(MORPHSIZE6, MORPHSIZE6, CV_8U), cv::Point(-1, -1), 1);

	//寻找区域边缘
	std::vector<std::vector<cv::Point> > contours;

	cv::findContours(rectImageData, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	//联合区域并消除小区域
	for (int i = 0; i < contours.capacity(); i++)
	{
		cv::Rect rect = cv::boundingRect(contours.at(i));
		double area = cv::contourArea(contours.at(i));
		if (pPara->minArea > 0)
		{
			if (area < pPara->minArea) continue;
		}
		cv::rectangle(rectImageData, rect, cv::Scalar(255, 255, 255), CV_FILLED);
	}
	cv::dilate(rectImageData, tmpImg, cv::Mat(MORPHSIZE7, MORPHSIZE7, CV_8U), Point(-1, -1), 1);
	cv::erode(tmpImg, rectImageData, cv::Mat(MORPHSIZE7, MORPHSIZE7, CV_8U), Point(-1, -1), 1);

	//统计矩形区域
	std::vector<std::vector<cv::Point>> contours1;

	cv::findContours(rectImageData, contours1, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours1.capacity(); i++)
	{
		cv::Rect rect = cv::boundingRect(contours1.at(i));
		double area = cv::contourArea(contours1.at(i));
		if (pPara->minArea > 0)
		{
			if (area < pPara->minArea) continue;
		}
		cv::rectangle(rectImageData, rect, Scalar(255, 255, 255));
	}
	//统计矩形区域
	std::vector<std::vector<cv::Point> > contours2;
	cv::findContours(rectImageData, contours2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours2.capacity(); i++)
	{
		cv::Rect rect = cv::boundingRect(contours2.at(i));
		double area = cv::contourArea(contours2.at(i));
		if (pPara->minArea > 0)
		{
			if (area < pPara->minArea) continue;
		}
		cv::rectangle(rectImageData, rect, cv::Scalar(255, 255, 255));
	}
	//统计矩形区域
	cv::Mat tmpMaskImageData;
	rectImageData.copyTo(tmpMaskImageData);
	std::vector<std::vector<cv::Point> > contours3;
	cv::findContours(tmpMaskImageData, contours3, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	rects.clear();
	areas.clear();
	defects.DefectNum = 0;

	for (int i = 0; i < MIN(contours3.capacity(), TMTV_MAXDEFECTNUM); i++)
	{
		cv::Rect rect = cv::boundingRect(contours3.at(i));
		defects.DefectPos[i][0] = rect.x;
		defects.DefectPos[i][1] = rect.y;
		defects.DefectPos[i][2] = rect.width;
		defects.DefectPos[i][3] = rect.height;
		defects.DefectPos[i][4] = 1;
		defects.DefectNum++;
		//rects.push_back(rect);
		//double area = cv::contourArea(contours3.at(i));
		//areas.push_back(area);
		//cv::rectangle(srcImageData, rect, Scalar(0, 0, 255));
	}

	delete subImg;
	return true;
}

//识别当前图像队列
bool BackgroundDetector::Detect(PATHSTR srcImagePath, PATHSTR rectImagePath,
	Tmtv_DefectInfo & defects,
	void* paras, long paraSize)
{
	cv::Mat srcImageData = cv::imread(srcImagePath);
	cv::Mat rectImageData;
	srcImageData.copyTo(rectImageData);
	if (srcImageData.empty())
	{
		return false;
	}

	bool detected = Detect(srcImageData, rectImageData, defects, paras, paraSize);
	if (detected)
	{
		if (rectImagePath[0] != 0)
		{
			cv::imwrite(rectImagePath, srcImageData);
		}
		return true;
	}
	return false;
};