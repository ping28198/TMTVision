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
	//�ͷ�������Դ
	void ReLease();
	//��ԭ�����ػ�
	void ReSet();
	//���յ�ǰ�����ػ�
	void ReDraw();
	//���յ�ǰ�����ػ�
	void ReSize();
	//������ʾ����,���Զ��ػ�
	void SetImageRect(int srcX,int srcY,double ration);
	//��� ��ʾ����
	void GetImageRect(int& srcX,int& srcY,double& ration);
	//��� ����ֵ
	void GetPixelValue(int x,int y,DWORD& pixData);

	bool m_validFlag;
	//��Ч��
	bool IsValid();
	//�ļ�����
	bool LoadImageFromFile(wchar_t pathStr[512]);
	//�ļ����ڴ�������
	bool LoadImageFromFileInMemory(unsigned char* srcFileInMemory,long srcFileSize);
	//�ļ����ڴ�������
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
