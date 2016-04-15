// NetWorkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NetWorkDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "CameraManagerDlg.h"
// NetWorkDlg 对话框

IMPLEMENT_DYNAMIC(NetWorkDlg, CDialogEx)

NetWorkDlg::NetWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MESSAGEINFO_DLG, pParent)
{
	this->pParent = (CCameraManagerDlg*)pParent;
}

NetWorkDlg::~NetWorkDlg()
{
}

void NetWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGEINFO_LST, mNetStatusList);
}


BEGIN_MESSAGE_MAP(NetWorkDlg, CDialogEx)
END_MESSAGE_MAP()


// NetWorkDlg 消息处理程序
