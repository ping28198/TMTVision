#include "stdafx.h"
#include "SendServer.h"

SendServer::SendServer(int queueSize /*= 64*/) :tmpMessageItem(MessageItem::MAXMSGSIZE)
{
	m_QueueSize = MIN(queueSize, MINQUEUESIZE);
}

SendServer::~SendServer()
{
}

void SendServer::Task(void)
{
	if (m_SkStatus == enRecvOK || m_SkStatus == enSendAndRecvOK)
	{

		EnterCriticalSection(&m_section);	
		if (m_MessageItemQueue.size()>0)
		{
			SendMsg(m_MessageItemQueue.front().p_Buffer, m_MessageItemQueue.front().m_BufferSize);
			m_MessageItemQueue.pop_front();
		}
		LeaveCriticalSection(&m_section);
	}
}

bool SendServer::Initial(int remoteRecvPort, char * remoteRecvIp,
	int localSendPort/* = 0*/, char * localSendIP/* = NULL*/, DWORD optionFlag/* = 1*/)
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
		bool isOK = SetSendAddr(remoteRecvPort, remoteRecvIp, localSendPort, localSendIP);
		return isOK;
	}
}

bool SendServer::Unitial()
{
	ForceEnd();
	return ReleaseSocket() != 0;
}