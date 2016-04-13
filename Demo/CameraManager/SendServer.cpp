#include "stdafx.h"
#include "SendServer.h"

SendServer::SendServer(int queueSize /*= 64*/)
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
		tmpMessageItem.p_Buffer[0] = 0;
		int revLen = RecvMsg((void*)tmpMessageItem.p_Buffer, tmpMessageItem.m_BufferSize,
			&tmpMessageItem.m_SenderPort,tmpMessageItem.m_SenderIp);

		EnterCriticalSection(&m_section);
		if (revLen>0)
		{
			if (m_MessageItemQueue.size() >= m_QueueSize)
			{
				m_MessageItemQueue.pop_front();
			}
			m_MessageItemQueue.push_back(tmpMessageItem);
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