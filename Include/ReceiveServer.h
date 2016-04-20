///<proj_info>
//==============================================================================
// ��Ŀ�� : ͼ����ƽ̨
// �ļ��� : ReceiveServer.h
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
//2.0	  ����ƽ		2016-4-19		�޸�������Ϣ�������Ϣ�н���Ŀ���ַ����Ϣ����
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
//Socket��Ϣ�����߳�, �ں���Ϣ����ʵ�ֶ��߳��첽����
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
	
	//��ʼ��
	bool Initial(int localRecvPort, NetIP localRecvIP,
		DWORD optionFlag = RECV_NOWAIT|ADDR_REUSE,long sleepTime=0);
	bool Initial(ReceiveServerSetting receiveServerSetting);

	//************************************
	// ����:  ��ȡ��ǰ��������
	// ˵��:  
	// ����:  ReceiveServer::GetSetting
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   ReceiveServerSetting & mSetting  //
	//************************************
	bool GetSetting(ReceiveServerSetting &mSetting);

	//************************************
	// ����:  Ӧ�ø�����������������
	// ˵��:  
	// ����:  ReceiveServer::ReSetSetting
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   const ReceiveServerSetting & mSetting  //
	//************************************
	bool ReSetSetting(const ReceiveServerSetting &mSetting);

	//************************************
	// ����:  Ӧ�õ�ǰ������������
	// ˵��:  
	// ����:  ReceiveServer::ReSetSocket
	// Access:    public 
	// ����ֵ:   bool  // 
	//************************************
	bool ResetSocket();

	DWORD GetReceiveStatus();
	void Create();
	bool Unitial();	
//Socket����
private:
	MessageItem tmpMessageItem;
	ReceiveServerSetting m_ReceiveServerSetting;
public:
	enum { QUEUESIZE = 64 };
	Queue<MessageItem>  m_MessageItemQueue;
	//��ȡ��Ϣ
	bool PullMsg(void *pBuffer, long &msgLength, int *senderPort = 0, NetIP senderIP = 0);
	bool PullMsg(MessageItem &messageItem);
	//������Ϣ
	void Task(void);
};
//==============================================================================
///</class_info>