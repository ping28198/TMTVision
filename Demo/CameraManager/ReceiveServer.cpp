#include "stdafx.h"
#include "ReceiveServer.h"

ReceiveServer::ReceiveServer(int queueSize /*= 64*/):tmpMessageItem(MessageItem::MAXMSGSIZE)
{
	m_QueueSize = MIN(queueSize, MINQUEUESIZE);
}

ReceiveServer::~ReceiveServer()
{
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
			if (m_MessageItemQueue.size()>= m_QueueSize)
			{
				m_MessageItemQueue.pop_front();
			}
			m_MessageItemQueue.push_back(tmpMessageItem);
		}
		LeaveCriticalSection(&m_section);
	}
}

bool ReceiveServer::Initial(int localRecvPort, char* localRecvIP, DWORD optionFlag /*= 1*/)
{
	ForceEnd();
	EnterCriticalSection(&m_section);
	m_MessageItemQueue.clear();
	LeaveCriticalSection(&m_section);
	if (!SetOption(optionFlag))
	{
		return false;
	}
	else
	{
		bool isOK = SetRecvAddr(localRecvPort, localRecvIP);
		return isOK;
	}
}

bool ReceiveServer::Unitial()
{
	ForceEnd();
	return ReleaseSocket() != 0;
}