#include "stdafx.h"
#include "SendServer.h"

void SendServer::Task(void)
{
	if (m_SkStatus == enSendOK || m_SkStatus == enSendAndRecvOK)
	{
		if (m_MessageItemQueue.GetLength()>0)
		{
			for (int i = 0; i < 3;i++)
			{
				if (SendMsg((void*)m_MessageItemQueue.GetHead()->p_Buffer, 
						m_MessageItemQueue.GetHead()->m_BufferSize)>0)
				{
					EnterCriticalSection(&m_section);
					m_MessageItemQueue.DelHead();
					LeaveCriticalSection(&m_section);
					break;
				}
				else
				{
					Sleep(100);
				}
			}
		}
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

bool SendServer::PushMsg(void *pBuffer, long msgLength)
{
	if (m_MessageItemQueue.IsFull())
	{
		return false;
	}
	else
	{
		MessageItem tmpMsg(pBuffer, msgLength);
		tmpMsg.m_SenderPort = m_LocalSendPort;
		strcpy_s(tmpMsg.m_SenderIp, TMTV_IPSTRLEN, m_LocalSendIP);
		EnterCriticalSection(&m_section);
		bool isOK = m_MessageItemQueue.AddTail(tmpMsg);
	    LeaveCriticalSection(&m_section);
	    return isOK;
	}
}

bool SendServer::PushMsg(MessageItem & messageItem)
{
	if (m_MessageItemQueue.IsFull())
	{
		return false;
	}
	else
	{
		messageItem.m_SenderPort = m_LocalSendPort;
		strcpy_s(messageItem.m_SenderIp, TMTV_IPSTRLEN, m_LocalSendIP);
		EnterCriticalSection(&m_section);
		bool isOK = m_MessageItemQueue.AddTail(messageItem);
		LeaveCriticalSection(&m_section);
		return isOK;
	}
}
