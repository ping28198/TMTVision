#include "stdafx.h"
#include "CImageView.h"

CImageView::CImageView(void)
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

CImageView::~CImageView(void)
{
	ReLease();
}

void CImageView::ReLease()
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
//��ԭ�����ػ�
void CImageView::ReSet()
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
//���յ�ǰ�����ػ�
void CImageView::ReDraw()
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

			//����Դ��Ŀ�������С����Ч��С
			if (dstW<0) dstW=iWidth;
			if (dstH<0) dstH=iHeight;
			if (dstW+dstX>iWidth) dstW=iWidth-dstX;
			if (dstH+dstY>iHeight) dstH=iHeight-dstY;
			//if (srcW<0) srcW=m_dataWidth;
			//if (srcH<0) srcH=m_dataHeight;
			if (srcW+srcX>m_ImageData.GetWidth()) srcW=m_ImageData.GetWidth()-srcX;
			if (srcH+srcY>m_ImageData.GetHeight()) srcH=m_ImageData.GetHeight()-srcY;
			//���ݱ������ȷ��ʵ�ʻ�ͼ��С
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

			//������������
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
//���յ�ǰ�����ػ�
void CImageView::ReSize()
{
	ReDraw();
}
//������ʾ����,���Զ��ػ�
void CImageView::SetImageRect(int srcX,int srcY,double ration)
{
	m_ration=ration;
	m_leftTopPoint.x=srcX;
	m_leftTopPoint.y=srcY;
}
//��� ��ʾ����
void CImageView::GetImageRect(int& srcX,int& srcY,double& ration)
{
	ration=m_ration;
	srcX=m_leftTopPoint.x;
	srcY=m_leftTopPoint.y;
}
//��� ��ʾ����
void CImageView::GetPixelValue(int x,int y,DWORD& pixData)
{
	pixData=m_ImageData.GetPixel(x,y);
}
//��Ч��
bool CImageView::IsValid()
{
	return m_validFlag;
}
//�ļ�����
bool CImageView::LoadImageFromFile(wchar_t pathStr[512])
{
	m_ImageData.Destroy();
	HRESULT result;
	result=m_ImageData.Load(pathStr);//��ȡJPGͼƬ
	if(result!=S_OK) 
	{
		ReLease();
		return false;
	}
	return true;
}
//�ļ����ڴ�������
bool CImageView::LoadImageFromFileInMemory(unsigned char* srcFileInMemory,long srcFileSize)
{
//�����ڴ��ļ���������ǰ���ļ���;
	void* hPreGlbMem   =0;
	hPreGlbMem   =GlobalAlloc(GMEM_MOVEABLE,srcFileSize);//���ƶ��Ļ�����
	IStream*  preIStm  =   0;  
	CreateStreamOnHGlobal(hPreGlbMem,   FALSE,   &preIStm);//���ļ�������Ϊ������ʼ     
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

	//CImage cimage���ڶ�ȡ�ļ���;
	HRESULT result;
	result=m_ImageData.Load(preIStm);//��ȡJPGͼƬ
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
//�ļ����ڴ�������
bool CImageView::LoadImageFromFileStream(IStream*  pIStream)
{
	HRESULT result;
	result=m_ImageData.Load(pIStream);//��ȡJPGͼƬ
	if(result!=S_OK) 
	{
		ReLease();
		return false;
	}
	return true;
}

IMPLEMENT_DYNCREATE(CImageView, CView)

BEGIN_MESSAGE_MAP(CImageView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
END_MESSAGE_MAP()

void CImageView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect m_drawRect;
	this->GetClientRect(m_drawRect);
	if (point.x>=m_drawRect.left &&point.y>=m_drawRect.top
		&&point.x<=m_drawRect.right &&point.y<=m_drawRect.bottom)
	{
		//��Ӽ�¼��
		m_lastPoint.x=point.x;
		m_lastPoint.y=point.y;
	}
	CView::OnLButtonDown(nFlags, point);
}
void CImageView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_movedDist.x+=m_movingDist.x;
	m_movedDist.y+=m_movingDist.y;
	CView::OnLButtonUp(nFlags, point);
}
void CImageView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ReSet();
	CView::OnRButtonUp(nFlags, point);
}
void CImageView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�Զ���ý���
	CRect m_drawRect;
	CPoint imageDist;
	this->GetClientRect(m_drawRect);
	if (point.x>=m_drawRect.left &&point.y>=m_drawRect.top
		&&point.x<=m_drawRect.right &&point.y<=m_drawRect.bottom
		&& nFlags==MK_LBUTTON)
	{
		//���ݼ�¼��ȷ�����Ƴ�ʼ��
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

BOOL CImageView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

void CImageView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	ReSize();
}

void CImageView::OnDraw(CDC* /*pDC*/)
{
	// TODO: �ڴ����ר�ô����/����û���
	ReDraw();
}
int CImageView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_validFlag=true;
	return 0;
}
