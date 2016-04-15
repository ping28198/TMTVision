///<proj_info>
//==============================================================================
// 项目名 : 图像处理平台
// 文件名 : SendServer.h
// 作  者 : 王磊
// 用  途 : Socket消息发送线程, 内含消息队列实现多线程异步访问
// 版  权 : TMT
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间      描述
//1.0     王磊        2016-1-31    增加DeletDir
//==============================================================================
///</ver_info>

///<header_info>
//==============================================================================
//添加所需的头文件
#pragma once
#include "TmtSocket.h"
#include "Thread.h"
#include "Queue.h"
//#include <deque>
//#include <queue>
#include "CommonDefine.h"
using namespace std;
//==============================================================================
///</header_info>

///<datastruct_info>
//==============================================================================
//Xml配置文件按照此结构存储, 读取后再逐步启动程序
struct SendServerSetting
{
	int m_RemoteRecvPort = 5005;
	NetIP m_RemoteRecvIp = "0.0.0.0";
	int m_LocalSendPort = 5006;
	NetIP m_LocalSendIP = "0.0.0.0";
	DWORD m_OptionFlag = 1;
	long m_SleepTime;
public:
	SendServerSetting() {}
	SendServerSetting(const int remoteRecvPort, const NetIP  remoteRecvIp,
		const int localSendPort = 0, const NetIP  localSendIP = NULL, 
		DWORD optionFlag = TmtSocket::RECV_NOWAIT | TmtSocket::ADDR_REUSE,
		const long sleepTime = 0)
	{
		m_RemoteRecvPort = remoteRecvPort;
		strcpy_s(m_RemoteRecvIp, TMTV_IPSTRLEN, remoteRecvIp);
		m_LocalSendPort = localSendPort;
		strcpy_s(m_LocalSendIP, TMTV_IPSTRLEN, localSendIP);
		m_OptionFlag = optionFlag;
		m_SleepTime = sleepTime;
	}
	SendServerSetting(const SendServerSetting& SendServerSetting)
	{
		m_RemoteRecvPort = SendServerSetting.m_RemoteRecvPort;
		strcpy_s(m_RemoteRecvIp, TMTV_IPSTRLEN, SendServerSetting.m_RemoteRecvIp);
		m_LocalSendPort = SendServerSetting.m_LocalSendPort;
		strcpy_s(m_LocalSendIP, TMTV_IPSTRLEN, SendServerSetting.m_LocalSendIP);
		m_OptionFlag = SendServerSetting.m_OptionFlag;
		m_SleepTime = SendServerSetting.m_SleepTime;
	}
	SendServerSetting& operator= (const SendServerSetting& SendServerSetting)
	{
		m_RemoteRecvPort = SendServerSetting.m_RemoteRecvPort;
		strcpy_s(m_RemoteRecvIp, TMTV_IPSTRLEN, SendServerSetting.m_RemoteRecvIp);
		m_LocalSendPort = SendServerSetting.m_LocalSendPort;
		strcpy_s(m_LocalSendIP, TMTV_IPSTRLEN, SendServerSetting.m_LocalSendIP);
		m_OptionFlag = SendServerSetting.m_OptionFlag;
		m_SleepTime = SendServerSetting.m_SleepTime;
		return *this;
	}
};
//==============================================================================
///</datastruct_info>

///<datastruct_info>
//==============================================================================
#ifndef MESSAGEITEM
#define  MESSAGEITEM
class MessageItem
{
public:
	enum { MAXMSGSIZE = 10240 };
	char p_Buffer[MAXMSGSIZE];
	long m_BufferSize = 0;
	int m_SenderPort = 0;
	NetIP m_SenderIp;
public:
	MessageItem() {}
	MessageItem(void* pBuffer,long bufferSize) 
	{
		memcpy_s(p_Buffer, MAXMSGSIZE, pBuffer, bufferSize);
		m_BufferSize = MIN(bufferSize, MAXMSGSIZE);
	}	
	MessageItem(MessageItem& messageItem)
	{
		memcpy_s(p_Buffer, MAXMSGSIZE, messageItem.p_Buffer, messageItem.m_BufferSize);
		m_BufferSize = MIN(messageItem.m_BufferSize, MAXMSGSIZE);
	}
	MessageItem& operator= (const MessageItem& messageItem)
	{
		memcpy_s(p_Buffer, MAXMSGSIZE, messageItem.p_Buffer, messageItem.m_BufferSize);
		m_BufferSize = MIN(messageItem.m_BufferSize, MAXMSGSIZE);
		return *this;
	}
};
#endif
//==============================================================================
///</datastruct_info>

///<class_info>
//==============================================================================
//Socket消息发送线程, 内含消息队列实现多线程异步访问
class SendServer :
	public Thread, public TmtSocket
{
public:
	void* p_Parent;
	SendServer(void* pParent,HANDLE  hParent = 0)
	{
		p_Parent = pParent;
		m_MessageItemQueue.Initial(QUEUESIZE);
	}
	~SendServer()
	{
		m_MessageItemQueue.Unitial();
	}
	SendServerSetting m_SendServerSetting;
	bool Initial(int remoteRecvPort, NetIP  remoteRecvIp,
		int localSendPort = 0, NetIP  localSendIP = NULL,
		DWORD optionFlag = RECV_NOWAIT | ADDR_REUSE, long sleepTime = 0);	
	bool Initial(SendServerSetting sendServerSetting);
	void Create();

	bool GetSetting(SendServerSetting &mSetting);

	bool ReSetSetting(const SendServerSetting &mSetting);

	bool ResetSocket();

	DWORD GetSendServerStatus();

	bool Unitial();
//Socket功能
private:
	MessageItem tmpMessageItem;	
public:
	enum { QUEUESIZE = 256 };
	Queue<MessageItem>  m_MessageItemQueue;
	//推送消息
	bool PushMsg(void *pBuffer, long msgLength);
	bool PushMsg(MessageItem &messageItem);
	//发送消息
	void Task(void);
};
//==============================================================================
///</class_info>