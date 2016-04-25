// NetWorkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NetWorkDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "CameraManagerDlg.h"
// NetWorkDlg 对话框
#include "CommonInclude.h"
#include "CameraManager.h"
IMPLEMENT_DYNAMIC(NetWorkDlg, CDialogEx)

NetWorkDlg::NetWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MESSAGEINFO_DLG, pParent)
	, mRemoteRecvIP(0)
	, mLocalRecvIP(0)
	, mLocalSendIP(0)
	, mRemoteRecvPort(0)
	, mLocalRecvPort(0)
	, mLocalSendPort(0)
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
	DDX_IPAddress(pDX, IDC_REMOTERECVIP_ADS, mRemoteRecvIP);
	DDX_IPAddress(pDX, IDC_LOCALRECVIP_ADS, mLocalRecvIP);
	DDX_IPAddress(pDX, IDC_LOCALSENDIP_ADS, mLocalSendIP);
	DDX_Text(pDX, IDC_REMOTERECVPORT_EDT, mRemoteRecvPort);
	DDX_Text(pDX, IDC_LOCALRECVPORT_EDT, mLocalRecvPort);
	DDX_Text(pDX, IDC_LOCALSENDPORT_EDT, mLocalSendPort);
}


BEGIN_MESSAGE_MAP(NetWorkDlg, CDialogEx)
	ON_BN_CLICKED(IDC_NET_SUBMIT, &NetWorkDlg::OnBnClickedNetSubmit)
END_MESSAGE_MAP()


// NetWorkDlg 消息处理程序



void NetWorkDlg::OnBnClickedNetSubmit()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CameraManagerSetting mManagerSet;
	pParent->pCamManager->GetManagerSetting(mManagerSet);




}
