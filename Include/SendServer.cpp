#include "stdafx.h"
#include "SendServer.h"



bool SendServer::Initial(int remoteRecvPort, NetIP remoteRecvIp,
	                 int localSendPort, NetIP localSendIP,
	                 DWORD optionFlag, long sleepTime = 0)
{
	ForceEnd();
	bool isOK = true;
	SendServerSetting tmpSetting(remoteRecvPort, remoteRecvIp,
		localSendPort, localSendIP,optionFlag, sleepTime);
	EnterCriticalSection(&m_section);
	m_MessageItemQueue.Clear();
	isOK &= SetOption(optionFlag);
	isOK &= SetSendAddr(remoteRecvPort, remoteRecvIp, localSendPort, localSendIP);
	m_SendServerSetting = tmpSetting;
	LeaveCriticalSection(&m_section);
}

bool SendServer::Initial(SendServerSetting sendServerSetting)
{
	ForceEnd();
	bool isOK = true;
	EnterCriticalSection(&m_section);
	m_MessageItemQueue.Clear();
	isOK &= SetOption(sendServerSetting.m_OptionFlag);
	isOK &= SetSendAddr(sendServerSetting.m_RemoteRecvPort, sendServerSetting.m_RemoteRecvIp,
		sendServerSetting.m_LocalSendPort, sendServerSetting.m_LocalSendIP);
	m_SendServerSetting = sendServerSetting;
	LeaveCriticalSection(&m_section);
	return isOK;
}

void SendServer::Create()
{
	Thread::Create(-1, MIN(m_ReceiveServerSetting.m_SleepTime, 0), true);
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

void SendServer::Task(void)
{
	if (m_SkStatus == enSendOK || m_SkStatus == enSendAndRecvOK)
	{
		if (m_MessageItemQueue.GetLength() > 0)
		{
			for (int i = 0; i < 3; i++)
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