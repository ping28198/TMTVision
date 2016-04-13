#pragma once
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
#include "Thread.h"
#include "TmtSocket.h"
#include <deque>
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
	char* p_Buffer = 0;
	long m_BufferSize = 0;
	int m_SenderPort = 0;
    NetIP m_SenderIp;
public:
	enum { DEFAULTMSGSIZE = 512, MAXMSGSIZE = 10240 };
	MessageItem(long bufferSize = DEFAULTMSGSIZE)
	{
		p_Buffer = new char[bufferSize];
		m_BufferSize = bufferSize;
	}
	MessageItem(const void* pBuffer,long bufferSize = DEFAULTMSGSIZE)
	{
		p_Buffer = new char[bufferSize];
		m_BufferSize = bufferSize;
		memcpy_s(p_Buffer, m_BufferSize, pBuffer, bufferSize);
	}
	MessageItem(const MessageItem& messageItem)
	{
		p_Buffer = new char[MAX(messageItem.m_BufferSize, DEFAULTMSGSIZE)];
		m_BufferSize = MAX(messageItem.m_BufferSize, DEFAULTMSGSIZE);
		memcpy_s(p_Buffer, m_BufferSize, messageItem.p_Buffer, messageItem.m_BufferSize);
	}
	MessageItem& operator=(const MessageItem& messageItem)
	{
		p_Buffer = new char[MAX(messageItem.m_BufferSize, DEFAULTMSGSIZE)];
		m_BufferSize = MAX(messageItem.m_BufferSize, DEFAULTMSGSIZE);
		memcpy_s(p_Buffer, m_BufferSize, messageItem.p_Buffer, messageItem.m_BufferSize);
	}
	~MessageItem()
	{
		if (p_Buffer!=0)
		{
			delete[] p_Buffer;
		}
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
	enum { MINQUEUESIZE = 64 };
	ReceiveServer(int queueSize = MINQUEUESIZE);
	~ReceiveServer();
//Socket功能
private:
	MessageItem tmpMessageItem;
public:
	deque<MessageItem>  m_MessageItemQueue;
	int m_QueueSize;
	bool Initial(int localRecvPort, char* localRecvIP, DWORD optionFlag = 1);
	bool Unitial();	
	void Task(void);
};
//==============================================================================
///</class_info>