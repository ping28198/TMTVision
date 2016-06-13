#pragma once



#include "afxwin.h"
#include "CommonInclude.h"



#define MAX_RANGE_NUM 5

// CAddCamDlg �Ի���
class CVisionClientDlg;
class CAddCamDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddCamDlg)
public:
	CAddCamDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddCamDlg();
	CVisionClientDlg* pParent;
	char Campath[TMTV_PATHSTRLEN];
	char Camname[TMTV_SHORTSTRLEN];
	Tmtv_CameraInfo mCamInfo;
	vector<CPoint> mWarnRange[MAX_RANGE_NUM];
	int mRangeIndex;
	CRect mImgRect;
	CImage mimg;//�����ߴ��ͼ��ͼƬ
	CImage maskimg;//����ͼƬ
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDCAM_DLG };
#endif

public:
	void DrawImg();
	void DrawWarnRange();
	bool GenerateMskImg(CString& mstr);









protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	bool isDrawImg;
	int m_CamIndex;
	CString m_CamName;
	int mCamPos_X;
	int m_CamPos_Y;
	int mCamPos_Z;
	afx_msg void OnBnClickedAddSetcampathBt();
	afx_msg void OnBnClickedAddFinishBt();
	afx_msg void OnBnClickedAddSetmskBt();
	afx_msg void OnBnClickedAddRedrawBt();
	afx_msg void OnBnClickedAddAddrangeBt();
	CString m_CamPath;
	CComboBox m_WarningLevel;
	afx_msg void OnBnClickedAddOpenimg();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
};
