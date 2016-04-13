#include "stdafx.h"
#include "SendServer.h"

void SendServer::Task(void)
{
	if (m_SkStatus == enSendOK || m_SkStatus == enSendAndRecvOK)
	{
		EnterCriticalSection(&m_section);	
		if (m_MessageItemQueue.GetLength()>0)
		{
			SendMsg((void*)m_MessageItemQueue.GetHead()->p_Buffer, 
				m_MessageItemQueue.GetHead()->m_BufferSize);
			m_MessageItemQueue.DelHead();
		}
		LeaveCriticalSection(&m_section);
	}
}

bool SendServer::Initial(int remoteRecvPort, char * remoteRecvIp,
	int localSendPort/* = 0*/, char * localSendIP/* = NULL*/, DWORD optionFlag/* = 1*/)
{
	ForceEnd();
	EnterCriticalSection(&m_section);
	m_MessageItemQueue.Clear();
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