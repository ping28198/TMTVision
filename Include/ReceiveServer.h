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
	MessageItem(void* pBuffer, long bufferSize)
	{
		memcpy_s(p_Buffer, MAXMSGSIZE, pBuffer, bufferSize);
		m_BufferSize = MIN(bufferSize, MAXMSGSIZE);
	}
	MessageItem(MessageItem& messageItem)
	{
		memcpy_s(p_Buffer, MAXMSGSIZE, messageItem.p_Buffer, messageItem.m_BufferSize);
		m_BufferSize = MIN(messageItem.m_BufferSize, MAXMSGSIZE);
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
	enum { QUEUESIZE = 64 };
	ReceiveServer(void* pParent, HANDLE  hParent = 0)
	{
		p_Parent = pParent;
		m_MessageItemQueue.Initial(QUEUESIZE);
	}
	~ReceiveServer()
	{
		m_MessageItemQueue.Unitial();
	}
//Socket功能
private:
	MessageItem tmpMessageItem;
public:
	Queue<MessageItem>  m_MessageItemQueue;
	bool Initial(int localRecvPort, char* localRecvIP, DWORD optionFlag = 1);
	bool Unitial();	
	void Task(void);
	//拉取消息
	bool PullMsg(void *pBuffer, long &msgLength, int *senderPort = 0, NetIP senderIP = 0);
	bool PullMsg(MessageItem &messageItem);
};
//==============================================================================
///</class_info>