#include "Detector.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

//��ʼ����Դ������ͼ��
bool BackgroundDetector::Initial(Tmtv_AlgorithmInfo *pAlgorithmInfo)
{
	Detector::Initial(pAlgorithmInfo);
		///<BackgroundSubtractorMOG2��ʼ��>






		///</BackgroundSubtractorMOG2��ʼ��>
}
//ж����Դ������ͼ��
void BackgroundDetector::Unitial()
{
	///<BackgroundSubtractorMOG2ж��>






	///</BackgroundSubtractorMOG2ж��>
	Detector::Unitial();
}
//ʶ��ǰͼ�����
bool BackgroundDetector::Detect(Mat & srcImageData, Mat & rectImageData, Tmtv_DefectInfo & defects, void * paras, long paraSize)
{

	return false;
}


//BackgroundDetector::BackgroundDetector()
//{
//}
//
//BackgroundDetector::~BackgroundDetector()
//{
//}
//
////��ʼ������ͼ��
//bool BackgroundDetector::Initial(int imageWidth, int imageHeight, Mat * pMaskImageData)
//{
//	return Detector::Initial(imageWidth, imageHeight, pMaskImageData);
//}
////��ʼ������ͼ��
//bool BackgroundDetector::Initial(LONGSTR maskImagePath)
//{
//	return Detector::Initial(maskImagePath);
//}
////ж������ͼ��
//void BackgroundDetector::Unitial()
//{
//	Detector::Unitial();
//}

////ʶ��ǰͼ�����
//bool BackgroundDetector::Detect(Mat& srcImageData, Mat& rectImageData, vector<Rect>& rects, vector<double>& areas, double minArea, double learnRatio)
//{
//	////ת��Ϊ�Ҷ�ͼ��
//	//Mat grayImg;
//	//cv::cvtColor(srcImageData, grayImg, COLOR_BGR2GRAY);
//	double resizeRatio = 1.0;
//	//ͼ�����ż��ټ�����
//	Mat *subImg=new Mat;
//	Size dsize;
//	cv::resize(srcImageData, *subImg, dsize, resizeRatio, resizeRatio, INTER_LINEAR);
//	//��˹ģ������ƽ��Ӱ��
//	Mat gaussImg;
//	Size maskSize;
//	maskSize.width = GAUSSIANSIZE;
//	maskSize.height = GAUSSIANSIZE;
//	cv::GaussianBlur(*subImg, gaussImg, dsize, GAUSSIANSIGMA, GAUSSIANSIGMA);
//
//	//��������㷨
//	p_backgroundSubtractor->apply(gaussImg, rectImageData, learnRatio);
//	Mat RGBRectImageData[3];
//	cv::split(rectImageData, RGBRectImageData);
//	Mat RGBMaskImageData[3];
//	cv::split(m_maskImageData, RGBMaskImageData);
//	cv::min(RGBRectImageData[0], RGBMaskImageData[0], RGBMaskImageData[1]);
//
//	RGBMaskImageData[1].copyTo(rectImageData);
//	Mat tmpImg;
//	//��������������С����,�ȸ�ʴ������
//	cv::erode(rectImageData, tmpImg, Mat(MORPHSIZE1, MORPHSIZE1, CV_8U), Point(-1, -1), 1);
//	cv::dilate(tmpImg, rectImageData, Mat(MORPHSIZE2, MORPHSIZE2, CV_8U), Point(-1, -1), 1);
//	cv::erode(rectImageData, tmpImg, Mat(MORPHSIZE1, MORPHSIZE1, CV_8U), Point(-1, -1), 1);
//	cv::dilate(tmpImg, rectImageData, Mat(MORPHSIZE2, MORPHSIZE2, CV_8U), Point(-1, -1), 1);
//
//	//�������������������ϸС�ն�,�����ͺ�ʴ
//	cv::dilate(rectImageData, tmpImg, Mat(MORPHSIZE3, MORPHSIZE3, CV_8U), Point(-1, -1), 1);
//	cv::erode(tmpImg, rectImageData, Mat(MORPHSIZE4, MORPHSIZE4, CV_8U), Point(-1, -1), 1);
//	cv::dilate(rectImageData, tmpImg, Mat(MORPHSIZE3, MORPHSIZE3, CV_8U), Point(-1, -1), 1);
//	cv::erode(tmpImg, rectImageData, Mat(MORPHSIZE4, MORPHSIZE4, CV_8U), Point(-1, -1), 1);
//
//	cv::dilate(rectImageData, tmpImg, Mat(MORPHSIZE5, MORPHSIZE5, CV_8U), Point(-1, -1), 1);
//	cv::erode(tmpImg, rectImageData, Mat(MORPHSIZE6, MORPHSIZE6, CV_8U), Point(-1, -1), 1);
//
//	//Ѱ�������Ե
//	vector<vector<Point> > contours;
//
//	cv::findContours(rectImageData, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
//
//	//������������С����
//	for (int i = 0; i < contours.capacity(); i++)
//	{
//		Rect rect = cv::boundingRect(contours.at(i));
//		double area = cv::contourArea(contours.at(i));
//		if (minArea > 0)
//		{
//			if (area < minArea) continue;
//		}
//		cv::rectangle(rectImageData, rect, Scalar(255, 255, 255), CV_FILLED);
//	}
//	cv::dilate(rectImageData, tmpImg, Mat(MORPHSIZE7, MORPHSIZE7, CV_8U), Point(-1, -1), 1);
//	cv::erode(tmpImg, rectImageData, Mat(MORPHSIZE7, MORPHSIZE7, CV_8U), Point(-1, -1), 1);
//
//	//ͳ�ƾ�������
//	vector<vector<cv::Point>> contours1;
//	
//	cv::findContours(rectImageData, contours1, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
//
//	for (int i = 0; i < contours1.capacity(); i++)
//	{
//		Rect rect = cv::boundingRect(contours1.at(i));
//		double area = cv::contourArea(contours1.at(i));
//		if (minArea > 0)
//		{
//			if (area < minArea) continue;
//		}
//		cv::rectangle(rectImageData, rect, Scalar(255, 255, 255));
//	}
//	//ͳ�ƾ�������
//	vector<vector<cv::Point> > contours2;
//	cv::findContours(rectImageData, contours2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
//	for (int i = 0; i < contours2.capacity(); i++)
//	{
//		Rect rect = cv::boundingRect(contours2.at(i));
//		double area = cv::contourArea(contours2.at(i));
//		if (minArea > 0)
//		{
//			if (area < minArea) continue;
//		}
//		cv::rectangle(rectImageData, rect, Scalar(255, 255, 255));
//	}
//	//ͳ�ƾ�������
//	Mat tmpMaskImageData;
//	rectImageData.copyTo(tmpMaskImageData);
//	vector<vector<Point> > contours3;
//	cv::findContours(tmpMaskImageData, contours3, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
//	rects.clear();
//	areas.clear();
//	for (int i = 0; i < contours3.capacity(); i++)
//	{
//		Rect rect = cv::boundingRect(contours3.at(i));
//		rects.push_back(rect);
//		double area = cv::contourArea(contours3.at(i));
//		areas.push_back(area);
//		cv::rectangle(srcImageData, rect, Scalar(0, 0, 255));
//	}
//
//	delete subImg;
//	return true;
//}
//
//bool BackgroundDetector::Detect(char srcImagePath[256], char rectImagePath[256], double minArea, double learnRatio)
//{
//	vector<Rect> rects;
//	vector<double> areas;
//	Mat srcImageData = cv::imread(srcImagePath);
//	Mat rectImageData;
//	srcImageData.copyTo(rectImageData);
//	if (srcImageData.empty())
//	{
//		return false;
//	}
//
//	bool detected = Detect(srcImageData, rectImageData, rects, areas, minArea, learnRatio);
//	if (detected)
//	{
//		if (rectImagePath[0] != 0)
//		{
//			cv::imwrite(rectImagePath, srcImageData);
//		}
//		return true;
//	}
//	return false;
//}