#pragma once
#include "afxwin.h"
#include <atlimage.h>
//#include <GdiPlusPixelFormats.h>
class CImageView :
	public CView
{
public:
	CImageView(void);
	DECLARE_DYNCREATE(CImageView)
	virtual ~CImageView(void);
public:
	CImage m_ImageData;
	double m_ration;
	CPoint m_leftTopPoint;
	CPoint m_lastPoint;
	CPoint m_movedDist;
	CPoint m_movingDist;
	//释放所有资源
	void ReLease();
	//还原设置重绘
	void ReSet();
	//按照当前设置重绘
	void ReDraw();
	//按照当前设置重绘
	void ReSize();
	//设置显示参数,不自动重绘
	void SetImageRect(int srcX,int srcY,double ration);
	//获得 显示参数
	void GetImageRect(int& srcX,int& srcY,double& ration);
	//获得 像素值
	void GetPixelValue(int x,int y,DWORD& pixData);

	bool m_validFlag;
	//有效性
	bool IsValid();
	//文件载入
	bool LoadImageFromFile(wchar_t pathStr[512]);
	//文件自内存中载入
	bool LoadImageFromFileInMemory(unsigned char* srcFileInMemory,long srcFileSize);
	//文件自内存中载入
	bool LoadImageFromFileStream(IStream*  pIStream);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnDraw(CDC* /*pDC*/);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
