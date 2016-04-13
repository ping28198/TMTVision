#include "stdafx.h"
#include "CvImageView.h"

CvImageView::CvImageView(void)
{
	m_ration=1.0;
	m_leftTopPoint.x=0;
	m_leftTopPoint.y=0;
	m_movedDist.x=0;
	m_movedDist.y=0;
	m_movingDist.x=0;
	m_movingDist.y=0;
	m_lastPoint.x=0;
	m_lastPoint.y=0;
	m_validFlag=false;
}

CvImageView::~CvImageView(void)
{
	ReLease();
}

void CvImageView::ReLease()
{
	m_ration=1.0;
	m_leftTopPoint.x=0;
	m_leftTopPoint.y=0;
	m_movedDist.x=0;
	m_movedDist.y=0;
	m_movingDist.x=0;
	m_movingDist.y=0;
	m_lastPoint.x=0;
	m_lastPoint.y=0;
	m_ImageData.Destroy();
}
//还原设置重绘
void CvImageView::ReSet()
{
	m_ration=1.0;
	m_leftTopPoint.x=0;
	m_leftTopPoint.y=0;
	m_movedDist.x=0;
	m_movedDist.y=0;
	m_movingDist.x=0;
	m_movingDist.y=0;
	m_lastPoint.x=0;
	m_lastPoint.y=0;
	ReDraw();
}
//按照当前设置重绘
void CvImageView::ReDraw()
{
	if (IsIconic())
	{

	}
	else
	{
		CDC* pDC = this->GetDC();
		COLORREF backColor=RGB(0,0,0);
		//pDC->FillSolidRect(&m_drawRect, backColor);
		//pDC->FillSolidRect(&m_unusedRect, backColor);
		//pDC->FillSolidRect(&m_infoRect, backColor);
		CRect m_drawRect;
		this->GetClientRect(m_drawRect);
		//MemoryFile * tmp=&m_ImageData;
		if (m_ImageData.IsNull() != true
			&& m_ImageData.GetWidth()>0
			&& m_ImageData.GetHeight()>0)
		{
			int srcX=m_leftTopPoint.x;
			int srcY=m_leftTopPoint.y;
			int srcW=m_ImageData.GetWidth();
			int srcH=m_ImageData.GetHeight();
            int dstX=m_drawRect.left;
			int dstY=m_drawRect.top;
			int dstW=m_drawRect.Width();
			int dstH=m_drawRect.Height();
			double ration=m_ration;

			int iWidth = m_drawRect.right - m_drawRect.left;
			int iHeight = m_drawRect.bottom - m_drawRect.top;

			//裁切源、目标区域大小到有效大小
			if (dstW<0) dstW=iWidth;
			if (dstH<0) dstH=iHeight;
			if (dstW+dstX>iWidth) dstW=iWidth-dstX;
			if (dstH+dstY>iHeight) dstH=iHeight-dstY;
			//if (srcW<0) srcW=m_dataWidth;
			//if (srcH<0) srcH=m_dataHeight;
			if (srcW+srcX>m_ImageData.GetWidth()) srcW=m_ImageData.GetWidth()-srcX;
			if (srcH+srcY>m_ImageData.GetHeight()) srcH=m_ImageData.GetHeight()-srcY;
			//根据比例情况确定实际画图大小
			ration=(ration<0.1?0.1:ration);
			ration=(ration>4?4:ration);
			dstW=(int)srcW*ration;
			dstH=(int)srcH*ration;

			SetStretchBltMode(pDC->m_hDC,STRETCH_HALFTONE);
			m_ImageData.StretchBlt(pDC->m_hDC,
					dstX,dstY,dstW,dstH,
					srcX,srcY,srcW,srcH);
			//m_ImageData.Draw(pDC->m_hDC,
			//		dstX,dstY,dstW,dstH,
			//		srcX,srcY,srcW,srcH);

			//绘制无用区域
			if (dstW<m_drawRect.right-m_drawRect.left+1)
			{
				CRect rightBack=m_drawRect;
				rightBack.left=0;
				rightBack.top=0;
				rightBack.right=iWidth-1;
				rightBack.bottom=iHeight-1;

				rightBack.left=dstW;
				pDC->FillSolidRect(&rightBack, backColor);
				//FillRgn(pDC->GetSafeHdc(),rightBackRGN,backColor);
			}
			if (dstH<m_drawRect.bottom-m_drawRect.top+1)
			{
				CRect bottomBack=m_drawRect;
				bottomBack.left=0;
				bottomBack.top=0;
				bottomBack.right=iWidth-1;
				bottomBack.bottom=iHeight-1;

				bottomBack.top=dstH;
				bottomBack.right=dstW;
				pDC->FillSolidRect(&bottomBack, backColor);
				//FillRgn(pDC->GetSafeHdc(),bottomBackRGN,backColor);
			}
		}
		else pDC->FillSolidRect(&m_drawRect, backColor);
		//pDC->FillSolidRect(&m_drawRect, backColor);
	}
}
//按照当前设置重绘
void CvImageView::ReSize()
{
	ReDraw();
}
//设置显示参数,不自动重绘
void CvImageView::SetImageRect(int srcX,int srcY,double ration)
{
	m_ration=ration;
	m_leftTopPoint.x=srcX;
	m_leftTopPoint.y=srcY;
}
//获得 显示参数
void CvImageView::GetImageRect(int& srcX,int& srcY,double& ration)
{
	ration=m_ration;
	srcX=m_leftTopPoint.x;
	srcY=m_leftTopPoint.y;
}
//获得 显示参数
void CvImageView::GetPixelValue(int x,int y,DWORD& pixData)
{
	pixData=m_ImageData.GetPixel(x,y);
}
//有效性
bool CvImageView::IsValid()
{
	return m_validFlag;
}
//文件载入
bool CvImageView::LoadImageFromFile(wchar_t pathStr[512])
{
	m_ImageData.Destroy();
	HRESULT result;
	result=m_ImageData.Load(pathStr);//读取JPG图片
	if(result!=S_OK) 
	{
		ReLease();
		return false;
	}
	return true;
}
//文件自内存中载入
bool CvImageView::LoadImageFromFileInMemory(unsigned char* srcFileInMemory,long srcFileSize)
{
//根据内存文件创建解码前的文件流;
	void* hPreGlbMem   =0;
	hPreGlbMem   =GlobalAlloc(GMEM_MOVEABLE,srcFileSize);//可移动的缓冲区
	IStream*  preIStm  =   0;  
	CreateStreamOnHGlobal(hPreGlbMem,   FALSE,   &preIStm);//将文件缓存作为流的起始     
	if (preIStm   ==   0)     
	{
		GlobalFree(hPreGlbMem);
		ReLease();
		return  false;     
	}
	void *  preGlbData  = GlobalLock(hPreGlbMem);
	GlobalUnlock(hPreGlbMem);
	memcpy(hPreGlbMem,srcFileInMemory,srcFileSize);
	GlobalLock(hPreGlbMem);

	//CImage cimage用于读取文件流;
	HRESULT result;
	result=m_ImageData.Load(preIStm);//读取JPG图片
	if(result!=S_OK) 
	{
		GlobalUnlock(hPreGlbMem);
		GlobalFree(hPreGlbMem);
		ReLease();
		return false;
	}
	GlobalUnlock(hPreGlbMem);
	GlobalFree(hPreGlbMem);
	return true;
}
//文件自内存中载入
bool CvImageView::LoadImageFromFileStream(IStream*  pIStream)
{
	HRESULT result;
	result=m_ImageData.Load(pIStream);//读取JPG图片
	if(result!=S_OK) 
	{
		ReLease();
		return false;
	}
	return true;
}

IMPLEMENT_DYNCREATE(CvImageView, CView)

BEGIN_MESSAGE_MAP(CvImageView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
END_MESSAGE_MAP()

void CvImageView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect m_drawRect;
	this->GetClientRect(m_drawRect);
	if (point.x>=m_drawRect.left &&point.y>=m_drawRect.top
		&&point.x<=m_drawRect.right &&point.y<=m_drawRect.bottom)
	{
		//添加记录点
		m_lastPoint.x=point.x;
		m_lastPoint.y=point.y;
	}
	CView::OnLButtonDown(nFlags, point);
}
void CvImageView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_movedDist.x+=m_movingDist.x;
	m_movedDist.y+=m_movingDist.y;
	CView::OnLButtonUp(nFlags, point);
}
void CvImageView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ReSet();
	CView::OnRButtonUp(nFlags, point);
}
void CvImageView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//自动获得焦点
	CRect m_drawRect;
	CPoint imageDist;
	this->GetClientRect(m_drawRect);
	if (point.x>=m_drawRect.left &&point.y>=m_drawRect.top
		&&point.x<=m_drawRect.right &&point.y<=m_drawRect.bottom
		&& nFlags==MK_LBUTTON)
	{
		//根据记录点确定绘制初始点
		m_movingDist.x=m_lastPoint.x-point.x;
		m_movingDist.y=m_lastPoint.y-point.y;
		imageDist.x=(m_movingDist.x+m_movedDist.x)/m_ration;
		imageDist.y=(m_movingDist.y+m_movedDist.y)/m_ration;
		m_leftTopPoint.x=imageDist.x;
		m_leftTopPoint.y=imageDist.y;
		m_leftTopPoint.x=(m_leftTopPoint.x<0?0:m_leftTopPoint.x);
		m_leftTopPoint.y=(m_leftTopPoint.y<0?0:m_leftTopPoint.y);
		ReDraw();
	}
	CView::OnMouseMove(nFlags, point);
}

BOOL CvImageView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (zDelta<0)
	{
		if (m_ration>1)
		{
			m_ration-=0.5;
			m_ration=(m_ration>=1?m_ration:1);
		}
		else
		{
			m_ration-=0.125;
			m_ration=(m_ration>=0.125?m_ration:0.125);
		}
	} 
	else
	{
		if (m_ration>=1)
		{
			m_ration+=0.5;
			m_ration=(m_ration<=8?m_ration:8);
		}
		else
		{
			m_ration+=0.125;
			m_ration=(m_ration<=1?m_ration:1);
		}
	}
	ReDraw();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CvImageView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ReSize();
}

void CvImageView::OnDraw(CDC* /*pDC*/)
{
	// TODO: 在此添加专用代码和/或调用基类
	ReDraw();
}
int CvImageView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_validFlag=true;
	return 0;
}
