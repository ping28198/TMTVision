
// VisionClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "CommonInclude.h"
#include "VisionRunState.h"
#include "NetWorkServer.h"
#include "AddCamDlg.h"
#include "AskHistDlg.h"
#include "ZoomImgDlg.h"
#include "LoginDlg.h"
#include "CipherCode.h"
// CVisionClientDlg �Ի���




class CVisionClientDlg : public CDialogEx
{
// ����
public:
	CVisionClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VISIONCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
	CRITICAL_SECTION cs;
	vector<Tmtv_CameraInfo> mCamInfoVec;//�����Ϣ
	vector<Tmtv_ImageInfo> mImageShowVec;//����ʾͼ����Ϣ����
	vector<CRect> mRectVec;//����ʾ����
	CRect mImgRect;
	CBitmap* pImgBmp;
	CDC *pBmpDC;
	int xImgNum;
	int yImgNum;
	CNetWorkServer *pNetWorkServer;
	CVisionRunState *pRunState;
	CAddCamDlg* pAddCamDlg;
	CAskHistDlg* pAskHistDlg;
	CZoomImgDlg* pZoomImgDlg;
	bool m_IsRolling;
	TmtCrypter mcrypt;
	//Mat mt;//����
	//////////////////////////////////////////////////////////////////////////
	//������ͼ����Ƶ�ָ�������һ�����Ϣ
	//ͼ����Ϣ��������Ϣ
	static void DrawImage(Tmtv_ImageInfo& ImgInfo,CRect mRect, CDC* pDC);//
	//////////////////////////////////////////////////////////////////////////
	//��������ͼ��
	//ͼ����Ϣ������������������ͼ�����������ɫ
	static void DrawAllImage(vector<Tmtv_ImageInfo> &mImgVec, vector<CRect> &mRcVec, CDC* pDC, COLORREF bkcolor);
	
	//////////////////////////////////////////////////////////////////////////
	//����ͼ����ʾ����
	//ˮƽ��ʾ��������ֱ��ʾ������ͼ����������ͼ��������
	static void AdjustImgRect(int xnum,int ynum,CRect imgRangeRect, vector<CRect> &mRcVec);
	
	//////////////////////////////////////////////////////////////////////////
	//���ƾ����־
	static void DrawWarnSign(Tmtv_ImageInfo &mImg, CRect &mRc, CDC* pDC, int marginwidth,COLORREF bkcolor);

	void InitConfig();//�������ò���
	void DrawImgToMemBmp();//���ƺ���
	void ShowMemBmpToScreen();//��ʾ����

	//�����Ϣ��غ���
	void AddCam(Tmtv_CameraInfo *camInfo);
	void ModifyCam(Tmtv_CameraInfo *camInfo);
	void DelCam(int camIndex);
	//////////////////////////////////////////////////////////////////////////
	//�����Ϣ�Ƿ����ظ�
	//����0=û���ظ���1=������ظ���2=��������ظ���3=���Ŀ¼�ظ�
	int CheckCam(Tmtv_CameraInfo *camInfo);
	void SaveCamInfo();
	void LoadCamInfo();
	//////////////////////////////////////////////////////////////////////////
	//������Ϣ������غ���
	void ShowCamInfoToList();




	//////////////////////////////////////////////////////////////////////////
	//�Ŵ�ͼ����غ���
	void ShowZoomImg(Tmtv_ImageInfo& imgInfo);
	void PointShowZoomImg(CPoint point);

private:
	bool m_IsLogin;//��־�û��Ƿ��¼
	CString LoginName;
	TmtCrypter mCrypt;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl mListCamera;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedAddcamBt();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedIsrollingBt();
	afx_msg void OnBnClickedHistimgBt();
	afx_msg void OnBnClickedShnewimgBt();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedUserLoginBt();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
