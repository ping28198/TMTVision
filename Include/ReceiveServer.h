///<proj_info>
//==============================================================================
// 项目名 : 图像处理平台
// 文件名 : ReceiveServer.h
// 作  者 : 王磊
// 用  途 : Socket消息接受线程, 内含消息队列实现多线程异步访问
// 版  权 : TMT
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间      描述
//1.0     王磊        2016-1-31    增加DeletDir
//2.0	  任威平		2016-4-19		修改网络消息传输从消息中解析目标地址和消息长度
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
struct ReceiveServerSetting
{
	int m_LocalRecvPort = 5003;
	NetIP m_LocalRecvIP = "0.0.0.0";
	DWORD m_OptionFlag = 1;
	long m_SleepTime;
public:
	ReceiveServerSetting() {}
	ReceiveServerSetting(const int localRecvPort, const NetIP localRecvIP,
		const DWORD optionFlag = TmtSocket::RECV_NOWAIT | TmtSocket::ADDR_REUSE,
		const long sleepTime =0)
	{
		m_LocalRecvPort = localRecvPort;
		strcpy_s(m_LocalRecvIP, TMTV_IPSTRLEN, localRecvIP);
		m_OptionFlag = optionFlag;
		m_SleepTime = sleepTime;
	}
	ReceiveServerSetting(const ReceiveServerSetting& receiveServerSetting)
	{
		m_LocalRecvPort = receiveServerSetting.m_LocalRecvPort;
		strcpy_s(m_LocalRecvIP, TMTV_IPSTRLEN, receiveServerSetting.m_LocalRecvIP);
		m_OptionFlag = receiveServerSetting.m_OptionFlag;
		m_SleepTime = receiveServerSetting.m_SleepTime;
	}
	ReceiveServerSetting& operator= (const ReceiveServerSetting& receiveServerSetting)
	{
		m_LocalRecvPort = receiveServerSetting.m_LocalRecvPort;
		strcpy_s(m_LocalRecvIP, TMTV_IPSTRLEN, receiveServerSetting.m_LocalRecvIP);
		m_OptionFlag = receiveServerSetting.m_OptionFlag;
		m_SleepTime = receiveServerSetting.m_SleepTime;
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
	MessageItem(const void* pBuffer, const long bufferSize)
	{
		memcpy_s(p_Buffer, MAXMSGSIZE, pBuffer, bufferSize);
		m_BufferSize = MIN(bufferSize, MAXMSGSIZE);
	}
	MessageItem(const MessageItem& messageItem)
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
//Socket消息接受线程, 内含消息队列实现多线程异步访问
class ReceiveServer :
	public Thread, public TmtSocket
{
public:
	void* p_Parent;
	ReceiveServer(void* pParent, HANDLE  hParent = 0)
	{
		p_Parent = pParent;
		m_MessageItemQueue.Initial(QUEUESIZE);
	}
	~ReceiveServer()
	{
		m_MessageItemQueue.Unitial();
	}
	
	//初始化
	bool Initial(int localRecvPort, NetIP localRecvIP,
		DWORD optionFlag = RECV_NOWAIT|ADDR_REUSE,long sleepTime=0);
	bool Initial(ReceiveServerSetting receiveServerSetting);

	//************************************
	// 作用:  获取当前网络配置
	// 说明:  
	// 名称:  ReceiveServer::GetSetting
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   ReceiveServerSetting & mSetting  //
	//************************************
	bool GetSetting(ReceiveServerSetting &mSetting);

	//************************************
	// 作用:  应用给定的配置重设网络
	// 说明:  
	// 名称:  ReceiveServer::ReSetSetting
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   const ReceiveServerSetting & mSetting  //
	//************************************
	bool ReSetSetting(const ReceiveServerSetting &mSetting);

	//************************************
	// 作用:  应用当前配置重设网络
	// 说明:  
	// 名称:  ReceiveServer::ReSetSocket
	// Access:    public 
	// 返回值:   bool  // 
	//************************************
	bool ResetSocket();

	DWORD GetReceiveStatus();
	void Create();
	bool Unitial();	
//Socket功能
private:
	MessageItem tmpMessageItem;
	ReceiveServerSetting m_ReceiveServerSetting;
public:
	enum { QUEUESIZE = 64 };
	Queue<MessageItem>  m_MessageItemQueue;
	//拉取消息
	bool PullMsg(void *pBuffer, long &msgLength, int *senderPort = 0, NetIP senderIP = 0);
	bool PullMsg(MessageItem &messageItem);
	//接受消息
	void Task(void);
};
//==============================================================================
///</class_info>