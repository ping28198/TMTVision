#pragma once
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
#include "Thread.h"
#include "TmtSocket.h"
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
	enum { QUEUESIZE = 64 };
	MessageItem tmpMessageItem;
	Queue<MessageItem>  m_MessageItemQueue;
	SendServer(HANDLE  hParent = 0)
	{
		m_MessageItemQueue.Initial(QUEUESIZE);
	}
	~SendServer()
	{
		m_MessageItemQueue.Unitial();
	}
//Socket功能
public:
	bool Initial(int remoteRecvPort, char * remoteRecvIp,
		int localSendPort = 0, char * localSendIP = NULL, DWORD optionFlag = 1);
	bool Unitial();	
	void Task(void);
};
//==============================================================================
///</class_info>