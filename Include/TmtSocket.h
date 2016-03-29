
///<proj_info>
//==============================================================================
// 项目名 ：图像处理平台
// 文件名 ：TmtSocket.h
// 作  者 ：TMT
// 用  途 ：定义了网络传输类
// 版  权 ：TMT
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间      描述
//1.0     任威平      2016-03-29   创建模块
//
//==============================================================================
///</ver_info>




#pragma once
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"WS2_32.lib")

enum Tmtv_SocketOption
{
	ADDR_REUSE=1,//允许绑定到正在使用的端口
	RECV_NOWAIT,//接收时非阻塞

};

class TmtSocket
{
public:
	TmtSocket();
	~TmtSocket();
	//////////////////////////////////////////////////////////////////////////
	//设置目标地址
	//目标IP，目标端口，我的端口，我的IP
	bool SetSendAddr(char* DstIp,int DstPort,int MyPort=0,char* MyIp=NULL);//如果后两个参数使用默认，则使用系统指定的ip和随机分配port发送
	//////////////////////////////////////////////////////////////////////////
	//设置接收地址
	//自己的端口,ip地址,位null时不限制网卡
	bool SetRecvAddr( int MyPort, char* MyIp = NULL);
	//////////////////////////////////////////////////////////////////////////
	//参考Tmtv_SocketOption枚举
	bool SetOption(int flag);//设置可选配置
	//////////////////////////////////////////////////////////////////////////
	//缓冲区指针，需要发送数据长度
	int SendMsg(void *pBuffer,size_t MsgLength);
	//////////////////////////////////////////////////////////////////////////
	//缓冲区指针，缓冲区长度，接收到的消息长度，对方端口号，对方ip
	int RecvMsg(void *pBuffer, size_t bufLength, int *SrcPort = NULL, char* SrcIp = NULL);

	int ReSet();//看需要进行重置
	int ReleaseSocket();//手动释放socket
private:
	int err;
	bool m_IsNormal;

	WSADATA wsadata;

	SOCKET sock_recv;
	SOCKET sock_send;
	SOCKADDR_IN Rc_MyAddr;
	SOCKADDR_IN Rc_SrcAddr;
	SOCKADDR_IN Sd_DstAddr;
	SOCKADDR_IN Sd_MyAddr;
};

