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
//Xml�����ļ����մ˽ṹ�洢, ��ȡ��������������
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
	bool Unitial();
//Socket����
private:
	MessageItem tmpMessageItem;	
public:
	enum { QUEUESIZE = 64 };
	Queue<MessageItem>  m_MessageItemQueue;
	//������Ϣ
	bool PushMsg(void *pBuffer, long msgLength);
	bool PushMsg(MessageItem &messageItem);
	//������Ϣ
	void Task(void);
};
//==============================================================================
///</class_info>