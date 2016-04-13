#include "stdafx.h"
#include "ReceiveServer.h"

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

bool ReceiveServer::Initial(int localRecvPort, char* localRecvIP, DWORD optionFlag /*= 1*/)
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
		bool isOK = SetRecvAddr(localRecvPort, localRecvIP);
		return isOK;
	}
}

bool ReceiveServer::Unitial()
{
	ForceEnd();
	EnterCriticalSection(&m_section);
	m_MessageItemQueue.Unitial();
	LeaveCriticalSection(&m_section);
	return ReleaseSocket() != 0;
}