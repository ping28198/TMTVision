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

void ReceiveServer::Create()
{
	Thread::Create(-1, MIN(m_ReceiveServerSetting.m_SleepTime, 0), true);
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
		EnterCriticalSection(&m_section);
		tmpMessageItem.p_Buffer[0] = 0;
		int revLen = RecvMsg((void*)tmpMessageItem.p_Buffer, tmpMessageItem.m_BufferSize,
			&tmpMessageItem.m_SenderPort,tmpMessageItem.m_SenderIp);
		if (revLen>0)
		{
			m_MessageItemQueue.ForcTail(tmpMessageItem);
		}
		LeaveCriticalSection(&m_section);
	}
}