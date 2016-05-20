#include "FaceDetector.h"

Tmtv_FaceDetectorInfo::Tmtv_FaceDetectorInfo(const Tmtv_AlgorithmInfo & algorithmInfo)
{
	structSize = sizeof(Tmtv_BackgroundDetectorInfo);
	strcpy_s(MaskImgPath, TMTV_PATHSTRLEN, algorithmInfo.MaskImgPath);
	strcpy_s(DstImgPath, TMTV_PATHSTRLEN, algorithmInfo.DstImgPath);
	WarnningLevel = algorithmInfo.WarnningLevel;
	mAlgoStatus = algorithmInfo.mAlgoStatus;
	strcpy_s(Reservechar, TMTV_LONGSTRLEN, algorithmInfo.Reservechar);
	//
	strcpy_s(CascadeClassifierFileName, TMTV_PATHSTRLEN, algorithmInfo.Reservechar);
}

Tmtv_FaceDetectorInfo & Tmtv_FaceDetectorInfo::operator=(const Tmtv_AlgorithmInfo & algorithmInfo)
{
	structSize = sizeof(Tmtv_BackgroundDetectorInfo);
	strcpy_s(MaskImgPath, TMTV_PATHSTRLEN, algorithmInfo.MaskImgPath);
	strcpy_s(DstImgPath, TMTV_PATHSTRLEN, algorithmInfo.DstImgPath);
	WarnningLevel = algorithmInfo.WarnningLevel;
	mAlgoStatus = algorithmInfo.mAlgoStatus;
	strcpy_s(Reservechar, TMTV_LONGSTRLEN, algorithmInfo.Reservechar);
	//
	strcpy_s(CascadeClassifierFileName, TMTV_PATHSTRLEN, algorithmInfo.Reservechar);
	return *this;
}

FaceDetector::FaceDetector()
{
}


FaceDetector::~FaceDetector()
{
}

bool FaceDetector::Initial(Tmtv_AlgorithmInfo algorithmInfo)
{
	Detector::Initial(algorithmInfo);
	///<CascadeClassifier初始化>
	m_FaceDetectorInfo = algorithmInfo;
	if (!m_CascadeClassifier.load(m_FaceDetectorInfo.CascadeClassifierFileName))
	{
		m_AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_NOWARN;
		return false;
	}
	else
	{
		if (m_CascadeClassifier.empty())
		{
			m_AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_NOWARN;
			return false;
		}
		m_AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_STARTWARN;
		return true;
	}
	///</CascadeClassifier初始化>
}

void FaceDetector::Unitial()
{
	///<CascadeClassifier卸载>
	///</CascadeClassifier卸载>
	Detector::Unitial();

}

void FaceDetector::Reset(Tmtv_AlgorithmInfo algorithmInfo)
{
	Unitial();
	Initial(algorithmInfo);
}

bool FaceDetector::Detect(cv::Mat& srcImageData, cv::Mat& rectImageData, Tmtv_DefectInfo & defects, void* paras /*= 0*/, long paraSize /*= 0*/)
{
	//if (p_maskImageData == 0) return false;
	//m_DetectedNum++;
	//if (m_DetectedNum < PERDETECTNUM)
	//{
	//	m_AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_PREWARN;
	//}
	//else
	//{
	//	m_AlgorithmInfo.WarnningLevel = Tmtv_AlgorithmInfo::TMTV_STARTWARN;
	//}
	std::vector<Rect> faces;
	srcImageData.copyTo(rectImageData);

	cv::cvtColor(rectImageData, rectImageData, CV_BGR2GRAY);
	cv::equalizeHist(rectImageData, rectImageData);
	m_CascadeClassifier.detectMultiScale(rectImageData, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
	
	for (int i = 0; i < faces.size(); i++)
	{
		if (i > TMTV_MAXDEFECTNUM) return true;
		Rect rect = faces[i];
		defects.DefectPos[i][0] = rect.x;
		defects.DefectPos[i][1] = rect.y;
		defects.DefectPos[i][2] = rect.width;
		defects.DefectPos[i][3] = rect.height;
		defects.DefectPos[i][4] = 1;
		defects.DefectNum++;
		cv::rectangle(rectImageData, rect, cv::Scalar(0, 0, 255));
	}
	return true;
}

bool FaceDetector::Detect(PATHSTR srcImagePath, PATHSTR rectImagePath, Tmtv_DefectInfo & defects, void* paras /*= 0*/, long paraSize /*= 0*/)
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
}