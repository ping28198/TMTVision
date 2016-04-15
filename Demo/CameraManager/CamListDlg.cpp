// CamListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CamListDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "CameraManagerDlg.h"
// CCamListDlg �Ի���

IMPLEMENT_DYNAMIC(CCamListDlg, CDialog)

CCamListDlg::CCamListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CAMERASINFO_DLG, pParent)
{
	this->pParent = (CCameraManagerDlg*)pParent;
}

CCamListDlg::~CCamListDlg()
{
}

void CCamListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMERAS_LST, m_CamList);
}


BEGIN_MESSAGE_MAP(CCamListDlg, CDialog)
	ON_BN_CLICKED(IDC_ADDCAMERA_BTN, &CCamListDlg::OnBnClickedAddcameraBtn)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_CAMERAS_LST, &CCamListDlg::OnNMCustomdrawCamList)
END_MESSAGE_MAP()


// CCamListDlg ��Ϣ�������


void CCamListDlg::OnBnClickedAddcameraBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	pParent->pAddCamDlg->ShowWindow(SW_SHOW);
}


void CCamListDlg::OnNMCustomdrawCamList(NMHDR* pNMHDR, LRESULT* pResult)
{//////////////////////////////////////////////////////////////////////////
	//�޸�����б�Ԫ����ɫ
	//NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	//if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	//{
	//	*pResult = CDRF_NOTIFYITEMDRAW;
	//}
	//else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	//{
	//	// This is the notification message for an item. We'll request  
	//	// notifications before each subitem's prepaint stage.  

	//	*pResult = CDRF_NOTIFYSUBITEMDRAW;
	//}
	//else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	//{
	//	int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
	//	switch (pLVCD->iSubItem)
	//	{
	//	case 2:
	//	case 4:
	//	{
	//		if (nItem % 2 == 0)
	//		{

	//			COLORREF clrNewTextColor, clrNewBkColor;
	//			clrNewTextColor = RGB(0, 0, 0);
	//			clrNewBkColor = RGB(198, 202, 198);

	//			pLVCD->clrText = clrNewTextColor;
	//			pLVCD->clrTextBk = clrNewBkColor;
	//			*pResult = CDRF_DODEFAULT;
	//			break;
	//		}
	//	}
	//	default:
	//		pLVCD->clrText = RGB(0, 0, 0);
	//		pLVCD->clrTextBk = RGB(255, 255, 255);
	//		*pResult = CDRF_DODEFAULT;
	//		break;
	//	}
	//}
}

BOOL CCamListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LONG mStyle;
	mStyle = GetWindowLong(m_CamList.m_hWnd, GWL_STYLE);//��ȡ��ǰ���ڷ�
	mStyle &= ~LVS_TYPEMASK;
	mStyle |= LVS_REPORT;
	SetWindowLong(m_CamList.m_hWnd, GWL_STYLE, mStyle);
	DWORD dwStyle = m_CamList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_CHECKBOXES;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ�����뱨�����listctrl��
	m_CamList.SetExtendedStyle(dwStyle); //������չ��ʽ

	m_CamList.InsertColumn(0, _T("�����"), LVCFMT_CENTER, 50);        //����б��� 
	m_CamList.InsertColumn(1, _T("����״̬"), LVCFMT_CENTER, 60);
	m_CamList.InsertColumn(2, _T("�㷨״̬"), LVCFMT_CENTER, 60);
	m_CamList.InsertColumn(3, _T("����ȼ�"), LVCFMT_CENTER, 60);
	m_CamList.InsertColumn(4, _T("�������"), LVCFMT_CENTER, 60);
	m_CamList.InsertColumn(5, _T("���λ��"), LVCFMT_CENTER, 60);













	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
