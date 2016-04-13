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
//���������ͷ�ļ�
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
//Socket��Ϣ�����߳�, �ں���Ϣ����ʵ�ֶ��߳��첽����
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
//Socket����
public:
	bool Initial(int remoteRecvPort, char * remoteRecvIp,
		int localSendPort = 0, char * localSendIP = NULL, DWORD optionFlag = 1);
	bool Unitial();	
	void Task(void);
};
//==============================================================================
///</class_info>