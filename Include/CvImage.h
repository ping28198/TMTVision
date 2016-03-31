#pragma once
#include "atlimage.h"
#include "opencv/cv.h"
#include "opencv2/opencv.hpp"
using namespace cv;
class CvImage :
	public CImage
{
public:
	CvImage(void);
	~CvImage(void);
public:
	//To support Opencv:
	Mat m_mat;
	//Deep clone to member object m_mat
	//       and to CImage itself
	CvImage(const Mat &mat);
	CvImage& operator = (const Mat& m);
	void Destroy();
	Mat* GetMat(bool updateFlag=false)
	{
		if (updateFlag)
		{
			UpdateCImage2Mat();
		} 
		return &m_mat;
	};

	//Convert CImage from/to Mat
	static bool CImage2Mat(CImage& cimg,Mat& mat);
	static bool Mat2CImage(Mat& mat,CImage& cimg);
	//Update CImage from/to Mat
	bool  UpdateCImage2Mat();//(CImage* pCImage=(CImage*)this,Mat* pMat=&(this->m_mat));
	bool  UpdateMat2CImage();//(Mat* pMat=&(this->m_mat),CImage* pCImage=(CImage*)this);

	//Reload CImage function
	BOOL CvCreate(int nWidth, int nHeight, int nBPP, DWORD dwFlags = 0);// throw();
	BOOL CvCreateEx(int nWidth, int nHeight, int nBPP, DWORD eCompression,
		const DWORD* pdwBitmasks = NULL, DWORD dwFlags = 0);// throw();
	HRESULT Load( LPCTSTR pszFileName ) throw();//´ýÐÞ¸Ä
	HRESULT Load( IStream* pStream ) throw();
	void LoadFromResource( HINSTANCE hInstance, LPCTSTR pszResourceName ) throw();
	void LoadFromResource( HINSTANCE hInstance, UINT nIDResource ) throw();
};

void maskImage(const CvImage & srcImg, const CvImage& mskImg, CvImage& dsrImg);

void maskImage(CvImage &srcImg, CvImage mskImg);