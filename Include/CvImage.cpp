//#include "stdafx.h"
#include "CvImage.h"

CvImage::CvImage(void)
{
	m_mat.release();
}

CvImage::~CvImage(void)
{
	if (!m_mat.empty())
	{
		m_mat.release();
	}
	Destroy();
}
CvImage::CvImage(const Mat& mat)
{
	if (!m_mat.empty())
	{
		m_mat.release();
	}
	mat.copyTo(m_mat);
	UpdateMat2CImage();
}

CvImage& CvImage::operator = (const Mat& mat)
{
	if (!m_mat.empty())
	{
		m_mat.release();
	}
	mat.copyTo(m_mat);
    UpdateMat2CImage();
	return *this;
}
void CvImage::Destroy()
{
	CImage::Destroy();
	if (!m_mat.empty())
	{
		m_mat.release();
	}
}

//Convert CImage from/to Mat
bool CvImage::CImage2Mat(CImage& cimg,Mat& mat)
{
	if (true == cimg.IsNull())  
	{
		return false;  
	}  

	int nChannels = cimg.GetBPP() / 8;  
	if ((1 != nChannels) && (3 != nChannels))  
	{  
		return false;  
	}  
	int nWidth    = cimg.GetWidth();  
	int nHeight   = cimg.GetHeight();  

	mat.release();
	//重建mat  
	if (1 == nChannels)  
	{  
		mat.create(nHeight, nWidth, CV_8UC1);  
	}  
	else if(3 == nChannels)  
	{  
		mat.create(nHeight, nWidth, CV_8UC3);  
	}  

	//拷贝数据  
	uchar* pucRow;                                  //指向数据区的行指针  
	uchar* pucImage = (uchar*)cimg.GetBits();     //指向数据区的指针  
	int nStep = cimg.GetPitch();                  //每行的字节数,注意这个返回值有正有负  

	for (int nRow = 0; nRow < nHeight; nRow++)  
	{  
		pucRow = (mat.ptr<uchar>(nRow));  
		for (int nCol = 0; nCol < nWidth; nCol++)  
		{  
			if (1 == nChannels)  
			{  
				pucRow[nCol] = *(pucImage + nRow * nStep + nCol);  
			}  
			else if (3 == nChannels)  
			{  
				for (int nCha = 0 ; nCha < 3; nCha++)  
				{  
					pucRow[nCol * 3 + nCha] = *(pucImage + nRow * nStep + nCol * 3 + nCha);  
				}             
			}  
		}     
	}
	return true;
}
bool CvImage::Mat2CImage(Mat& mat,CImage& cimg)
{
	if (mat.empty())  
	{  
		return false;  
	}  

	int nChannels = mat.channels();  
	if ((1 != nChannels) && (3 != nChannels))  
	{  
		return false;  
	}  
	int nWidth    = mat.cols;  
	int nHeight   = mat.rows;  

	//重建cimage
	//cimg.Detach();
	cimg.Destroy();
	cimg.Create(nWidth, nHeight, 8 * nChannels);
	//if (!(cimg.GetWidth()== nWidth && cimg.GetHeight() == nHeight && cimg.GetBPP()== 8 * nChannels))
	//{
	//	cimg.Destroy();
	//	cimg.Create(nWidth, nHeight, 8 * nChannels);  
	//}

	//拷贝数据  
	uchar* pucRow;                                  //指向数据区的行指针  
	uchar* pucImage = (uchar*)cimg.GetBits();     //指向数据区的指针  
	int nStep = cimg.GetPitch();                  //每行的字节数,注意这个返回值有正有负  

	if (1 == nChannels)                             //对于单通道的图像需要初始化调色板  
	{  
		RGBQUAD* rgbquadColorTable;  
		int nMaxColors = 256;  
		rgbquadColorTable = new RGBQUAD[nMaxColors];  
		cimg.GetColorTable(0, nMaxColors, rgbquadColorTable);  
		for (int nColor = 0; nColor < nMaxColors; nColor++)  
		{  
			rgbquadColorTable[nColor].rgbBlue = (uchar)nColor;  
			rgbquadColorTable[nColor].rgbGreen = (uchar)nColor;  
			rgbquadColorTable[nColor].rgbRed = (uchar)nColor;  
		}  
		cimg.SetColorTable(0, nMaxColors, rgbquadColorTable);  
		delete []rgbquadColorTable;  
	}  

	for (int nRow = 0; nRow < nHeight; nRow++)  
	{  
		pucRow = (mat.ptr<uchar>(nRow));  
		for (int nCol = 0; nCol < nWidth; nCol++)  
		{  
			if (1 == nChannels)  
			{  
				*(pucImage + nRow * nStep + nCol) = pucRow[nCol];  
			}  
			else if (3 == nChannels)  
			{  
				for (int nCha = 0 ; nCha < 3; nCha++)  
				{  
					*(pucImage + nRow * nStep + nCol * 3 + nCha) = pucRow[nCol * 3 + nCha];  
				}             
			}  
		}     
	}
	return true;
}

//Update CImage from/to Mat
bool  CvImage::UpdateCImage2Mat()
{
	return CvImage::CImage2Mat(*this,this->m_mat);
}
bool  CvImage::UpdateMat2CImage()
{
	return CvImage::Mat2CImage(this->m_mat,*this);
}

//Reload CImage function
BOOL CvImage::CvCreate( int nWidth, int nHeight, int nBPP, DWORD dwFlags )// throw()
{
	BOOL retVal = CImage::Create( nWidth, nHeight, nBPP, dwFlags);
	bool updVal = UpdateCImage2Mat();
	return retVal && updVal;
}
BOOL CvImage::CvCreateEx( int nWidth, int nHeight, int nBPP, DWORD eCompression,
			  const DWORD* pdwBitmasks, DWORD dwFlags )// throw()
{
	BOOL retVal = CImage::CreateEx( nWidth, nHeight, nBPP, eCompression, 
		pdwBitmasks, dwFlags);
	bool updVal = UpdateCImage2Mat();
	return retVal && updVal;
}
HRESULT CvImage::Load( LPCTSTR pszFileName ) throw()
{
	HRESULT retVal =  CImage::Load( pszFileName );
	bool updVal = UpdateCImage2Mat();
	if (updVal)
	{
		return retVal;
	} 
	else
	{
		return S_FALSE;
	}
}
HRESULT CvImage::Load( IStream* pStream ) throw()
{
	HRESULT retVal =  CImage::Load( pStream );
	bool updVal = UpdateCImage2Mat();
	if (updVal)
	{
		return retVal;
	} 
	else
	{
		return S_FALSE;
	}
}
void CvImage::LoadFromResource( HINSTANCE hInstance, LPCTSTR pszResourceName ) throw()
{
	CImage::LoadFromResource( hInstance, pszResourceName );
	UpdateCImage2Mat();
}
void CvImage::LoadFromResource( HINSTANCE hInstance, UINT nIDResource ) throw()
{
	CImage::LoadFromResource( hInstance, nIDResource );
	UpdateCImage2Mat();
}

void maskImage(const CvImage & srcImg, const CvImage & mskImg, CvImage& dstImg)
{
	//Mat srcImgMat;
	//srcImg.m_mat.copyTo(srcImgMat);
	cv::addWeighted(srcImg.m_mat, 0.5, mskImg.m_mat , 0.5, 0, dstImg.m_mat);
	dstImg.UpdateMat2CImage();
}

void maskImage(CvImage & srcImg, CvImage mskImg)
{
	cv::addWeighted(srcImg.m_mat, 0.5, mskImg.m_mat, 0.5, 0, srcImg.m_mat);
	srcImg.UpdateMat2CImage();
}
