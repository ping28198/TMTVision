#include "stdafx.h"
#include "ReceiveServer.h"

bool ReceiveServer::Initial(int localRecvPort, NetIP localRecvIP, DWORD optionFlag, long sleepTime)
{
	ForceEnd();
	bool isOK = true;
	ReceiveServerSetting tmpSetting(localRecvPort, localRecvIP, optionFlag, sleepTime);
	EnterCriticalSection(&m_section);
	m_MessageItemQueue.Clear();
	isOK &= SetOption(optionFlag);
	isOK &= SetRecvAddr(localRecvPort, localRecvIP);
	m_ReceiveServerSetting = tmpSetting;
	LeaveCriticalSection(&m_section);
	return isOK;
}

bool ReceiveServer::Initial(ReceiveServerSetting receiveServerSetting)
{
	ForceEnd();
	bool isOK = true;
	EnterCriticalSection(&m_section);
	m_MessageItemQueue.Clear();
	isOK &= SetOption(receiveServerSetting.m_OptionFlag);
	isOK &= SetRecvAddr(receiveServerSetting.m_LocalRecvPort, receiveServerSetting.m_LocalRecvIP);
	m_ReceiveServerSetting = receiveServerSetting;
	LeaveCriticalSection(&m_section);
	return isOK;
}

bool ReceiveServer::GetSetting(ReceiveServerSetting &mSetting)
{	
	EnterCriticalSection(&m_section);
	mSetting = m_ReceiveServerSetting;
	LeaveCriticalSection(&m_section);
	return true;
}

bool ReceiveServer::ReSetSetting(const ReceiveServerSetting &mSetting)
{
	m_ReceiveServerSetting = mSetting;
	
	return ResetSocket();
}

bool ReceiveServer::ResetSocket()
{
	ForceEnd();
	bool isOK = true;
	EnterCriticalSection(&m_section);
	isOK &= SetRecvAddr(m_ReceiveServerSetting.m_LocalRecvPort, m_ReceiveServerSetting.m_LocalRecvIP);
	isOK &= SetOption(m_ReceiveServerSetting.m_OptionFlag);
	m_SkStatus |= enRecvOK;
	LeaveCriticalSection(&m_section);
	Create();
	Resume();
	return isOK;
}

DWORD ReceiveServer::GetReceiveStatus()
{
	EnterCriticalSection(&m_section);
	return GetStatus();
	LeaveCriticalSection(&m_section);
}

void ReceiveServer::Create()
{
	Thread::Create(-1, MAX(m_ReceiveServerSetting.m_SleepTime, 0), true);
}

bool ReceiveServer::Unitial()
{
	ForceEnd();
	EnterCriticalSection(&m_section);
	m_MessageItemQueue.Unitial();
	LeaveCriticalSection(&m_section);
	return ReleaseSocket() != 0;
}

bool ReceiveServer::PullMsg(void * pBuffer, long & msgLength, int * senderPort, NetIP senderIP)
{
	if (m_MessageItemQueue.IsEmpty())
	{
		return false;
	}
	else
	{
		MessageItem tmpMsgItem;
		EnterCriticalSection(&m_section);
		bool isOK = m_MessageItemQueue.GetHead(tmpMsgItem);
		m_MessageItemQueue.DelHead();
		LeaveCriticalSection(&m_section);
		memcpy_s(pBuffer, msgLength, tmpMsgItem.p_Buffer, tmpMsgItem.m_BufferSize);
		msgLength = tmpMsgItem.m_BufferSize;
		if (senderPort!=0)
		{
			*senderPort = tmpMsgItem.m_SenderPort;
		}
		if (senderIP != 0)
		{
			strcpy_s(senderIP, TMTV_IPSTRLEN, tmpMsgItem.m_SenderIp);
		}
		return isOK;
	}
}

bool ReceiveServer::PullMsg(MessageItem & messageItem)
{
	if (m_MessageItemQueue.IsEmpty())
	{
		return false;
	}
	else
	{
		EnterCriticalSection(&m_section);
		bool isOK = m_MessageItemQueue.GetHead(messageItem);
		m_MessageItemQueue.DelHead();
		LeaveCriticalSection(&m_section);
		return isOK;
	}
}

void ReceiveServer::Task(void)
{
	if (m_SkStatus == enRecvOK || m_SkStatus == enSendAndRecvOK)
	{
		tmpMessageItem.p_Buffer[0] = 0;
		int revLen = 0;
		//int revLen = RecvMsg((void*)tmpMessageItem.p_Buffer, tmpMessageItem.m_BufferSize,
		//	&tmpMessageItem.m_SenderPort,tmpMessageItem.m_SenderIp);
		revLen = RecvNetMsg(tmpMessageItem.p_Buffer, tmpMessageItem.m_BufferSize);//2.0
		if (revLen>0)
		{
			EnterCriticalSection(&m_section);
			m_MessageItemQueue.ForcTail(tmpMessageItem);
			m_SkStatus |= enRecvOK;
			LeaveCriticalSection(&m_section);
		}
		if (revLen<0)
		{
			EnterCriticalSection(&m_section);
			m_SkStatus &= ~enRecvOK;
			LeaveCriticalSection(&m_section);
		}
	}
}