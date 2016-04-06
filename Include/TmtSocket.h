
///<proj_info>
//==============================================================================
// 项目名 : 图像处理平台
// 文件名 : TmtSocket.h
// 作  者 : TMT
// 用  途 : 定义了网络传输类
// 版  权 : TMT
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间        描述
//1.0     任威平      2016-03-29   创建模块
//1.1     王磊        2016-03-31   添加线程
//==============================================================================
///</ver_info>



///<header_info>
//==============================================================================
#pragma once
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"WS2_32.lib")
#include "Thread.h"
#include "CommonDefine.h"
//==============================================================================
///</header_info>

///<class_info>
//==============================================================================
//网络通讯Socket方法类
class TmtSocket
{
public:
	TmtSocket();
	~TmtSocket();
	//////////////////////////////////////////////////////////////////////////
	//设置目标地址
	//目标IP, 目标端口, 我的端口, 我的IP
	bool SetSendAddr(int remoteRecvPort,char* remoteRecvIp,
		int localSendPort=0,char* localSendIP=NULL);//如果后两个参数使用默认, 则使用系统指定的ip和随机分配port发送
	//////////////////////////////////////////////////////////////////////////
	//设置接收地址
	//自己的端口,ip地址,位null时不限制网卡
	bool SetRecvAddr( int localRecvPort, char* localRecvIP = NULL);
	//////////////////////////////////////////////////////////////////////////
	//参考Tmtv_SocketOption枚举   0 0 0 0 0 0 0 0
	//                            允许重复绑定┘└阻塞标志位
	enum {RECV_NOWAIT = 1,ADDR_REUSE = 2};
	bool SetOption(DWORD flag);//设置可选配置
	//////////////////////////////////////////////////////////////////////////
	//缓冲区指针, 需要发送数据长度
	int SendMsg(void *pBuffer,size_t MsgLength);
	//////////////////////////////////////////////////////////////////////////
	//缓冲区指针, 缓冲区长度, 接收到的消息长度, 对方端口号, 对方ip
	int RecvMsg(void *pBuffer, size_t bufLength, int *pRemoteSendPort = NULL, char* pRemoteSendIp = NULL);

	int ReSet();//看需要进行重置
	int ReleaseSocket();//手动释放socket
public:
	enum { enNoSendOrRecv = 0, enSendOK= 1, enRecvOK = 2, enSendAndRecvOK = 3};
	DWORD m_SkStatus;
	bool m_SKInitialOK;
	int m_OptionFlag;
	int m_RemoteRecvPort;
	char m_RemoteRecvIp;
	int m_LocalSendPort;
	char m_LocalSendIP;
	int m_LocalRecvPort;
	char m_LocalRecvIP;
private:
	//int err;
	//bool m_IsNormal;
	WSADATA wsadata;
	SOCKET sock_recv;
	SOCKET sock_send;
	SOCKADDR_IN Rc_MyAddr;
	SOCKADDR_IN Rc_SrcAddr;
	SOCKADDR_IN Sd_DstAddr;
	SOCKADDR_IN Sd_MyAddr;
};
//==============================================================================
///</class_info>


///<class_info>
//==============================================================================
//网络通讯Socket方法类
class TmtSocketServer :public Thread, public TmtSocket
{
public:
	//类功能
	char *pBuffer;
	enum { MINBUFFERSIZE = 256 ,MAXBUFFERSIZE = 10240};
	int m_BufferSize;
public:
	TmtSocketServer(int bufferSize = MAXBUFFERSIZE);
	~TmtSocketServer();
	bool Initial(int remotePort, char* remoteIp, int localRecvPort, char* localRecvIP = NULL,int localPort = 0, char* localIP = NULL, DWORD optionFlag = 1);
	bool Unitial();
	//Socket功能
public:
	void Task(void);
	virtual void ServerProcess(int msgLen)=0;
public:
	//调试函数,显示对象信息
	//method=0 xml 完整格式输出, 带换行
	//1 最短格式输出,只输出队列数量
	//2 更新格式输出,输出队列最新元素
	//字背景颜色范围: 40--49        字颜色: 30--39
	//	40 : 黑                           30 : 黑
	//	41 : 红                           31 : 红
	//	42 : 绿                           32 : 绿
	//	43 : 黄                           33 : 黄
	//	44 : 蓝                           34 : 蓝
	//	45 : 紫                           35 : 紫
	//	46 : 深绿                         36 : 深绿
	//	47 : 白色                         37 : 白色
	void ToString(HUGESTR &string, int method = 0, int color = 32);
};
//==============================================================================
///</class_info>