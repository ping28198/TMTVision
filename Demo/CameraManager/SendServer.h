#pragma once
///<proj_info>
//==============================================================================
// ��Ŀ�� : ͼ����ƽ̨
// �ļ��� : SendServer.h
// ��  �� : ����
// ��  ; : Socket��Ϣ�����߳�, �ں���Ϣ����ʵ�ֶ��߳��첽����
// ��  Ȩ : TMT
//==============================================================================
///</proj_info>

///<ver_info>
// �汾��¼	
//==============================================================================
//�汾��  ������Ա      ʱ��      ����
//1.0     ����        2016-1-31    ����DeletDir
//==============================================================================
///</ver_info>

///<header_info>
//==============================================================================
//��������ͷ�ļ�
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
//Socket��Ϣ�����߳�, �ں���Ϣ����ʵ�ֶ��߳��첽����
class SendServer :
	public Thread, public TmtSocket
{
public:
	enum { MINQUEUESIZE = 64 };
	SendServer(int queueSize = MINQUEUESIZE);
	~SendServer();
//Socket����
private:
	MessageItem tmpMessageItem;
public:
	deque<MessageItem>  m_MessageItemQueue;
	int m_QueueSize;
	bool Initial(int remoteRecvPort, char * remoteRecvIp,
		int localSendPort = 0, char * localSendIP = NULL, DWORD optionFlag = 1);
	bool Unitial();	
	void Task(void);
};
//==============================================================================
///</class_info>