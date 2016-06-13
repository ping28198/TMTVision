// AskHistDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VisionClient.h"
#include "AskHistDlg.h"
#include "afxdialogex.h"
#include "VisionClientDlg.h"

// CAskHistDlg �Ի���

IMPLEMENT_DYNAMIC(CAskHistDlg, CDialog)

CAskHistDlg::CAskHistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ASKHIST_DLG, pParent)
{
	//mComBoxCtrl.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, CRect(0, 0, 100, 20), this, IDC_COMBO1);
	this->pParent = (CVisionClientDlg*)pParent;
}

CAskHistDlg::~CAskHistDlg()
{
}

void CAskHistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, mComBoxCtrl);
	DDX_Control(pDX, IDC_MONTHCALENDAR1, mCalCtrl);
}


BEGIN_MESSAGE_MAP(CAskHistDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAskHistDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAskHistDlg ��Ϣ�������


INT_PTR CAskHistDlg::DoModal()
{

	return CDialog::DoModal();
}


void CAskHistDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int mIndex = mComBoxCtrl.GetCurSel();
	if (mIndex<0)
	{
		MessageBox(_T("��ѡ�������"));
		return;
	}
	if (mIndex>=pParent->mCamInfoVec.size())
	{
		MessageBox(_T("���ݹ��ڣ������²�����"));
		return;
	}
	int camnum = pParent->mCamInfoVec.at(mIndex).Indexnum;
	CTime mTime;
	bool backcode = mCalCtrl.GetCurSel(mTime);
	if (!backcode)
	{
		MessageBox(_T("��ѡ�����ڣ�"));
		return;
	}
	char mDate[TMTV_LONGSTRLEN] = {0};
	sprintf_s(mDate, TMTV_LONGSTRLEN, "%d-%02d-%02d", mTime.GetYear(), mTime.GetMonth(), mTime.GetDay());
	//pParent->pRunState->ShowHistoryImg(camnum, mDate);


	CDialog::OnOK();
}
