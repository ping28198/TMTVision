// AskHistDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VisionClient.h"
#include "AskHistDlg.h"
#include "afxdialogex.h"
#include "VisionClientDlg.h"

// CAskHistDlg 对话框

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


// CAskHistDlg 消息处理程序


INT_PTR CAskHistDlg::DoModal()
{

	return CDialog::DoModal();
}


void CAskHistDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int mIndex = mComBoxCtrl.GetCurSel();
	if (mIndex<0)
	{
		MessageBox(_T("请选择相机！"));
		return;
	}
	if (mIndex>=pParent->mCamInfoVec.size())
	{
		MessageBox(_T("数据过期，请重新操作！"));
		return;
	}
	int camnum = pParent->mCamInfoVec.at(mIndex).Indexnum;
	CTime mTime;
	bool backcode = mCalCtrl.GetCurSel(mTime);
	if (!backcode)
	{
		MessageBox(_T("请选择日期！"));
		return;
	}
	char mDate[TMTV_LONGSTRLEN] = {0};
	sprintf_s(mDate, TMTV_LONGSTRLEN, "%d-%02d-%02d", mTime.GetYear(), mTime.GetMonth(), mTime.GetDay());
	//pParent->pRunState->ShowHistoryImg(camnum, mDate);


	CDialog::OnOK();
}
