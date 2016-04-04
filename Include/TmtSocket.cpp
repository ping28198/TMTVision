#include "TmtSocket.h"
#include <exception>
TmtSocket::TmtSocket()
{
	try
	{
		DWORD Rc_wVersionRequest = MAKEWORD(2, 2);
		int err = WSAStartup(Rc_wVersionRequest, &wsadata);
		sock_recv = socket(AF_INET, SOCK_DGRAM, 0);
		sock_send = socket(AF_INET, SOCK_DGRAM, 0);
		m_SkStatus = enNoSendOrRecv;
		m_SKInitialOK = true;
	}
	catch (const std::exception&)
	{
		m_SkStatus = enNoSendOrRecv;
		m_SKInitialOK = false;
		OutputDebugString(L"<TmtSocket::TmtSocket() failed>\n");
	}

}

TmtSocket::~TmtSocket()
{
	try
	{
		closesocket(sock_send);
		closesocket(sock_recv);
		WSACleanup();
		m_SkStatus = enNoSendOrRecv;
	}
	catch (const std::exception&)
	{
		OutputDebugString(L"<TmtSocket::~TmtSocket() failed>\n");
	}
}

bool TmtSocket::SetSendAddr(int remoteRecvPort, char* remoteRecvIp, int localSendPort, char* localSendIP)
{
	if (m_SKInitialOK!=true)
	{
		m_SkStatus = enNoSendOrRecv;
		return false;
	}
	Sd_DstAddr.sin_addr.s_addr = inet_addr(remoteRecvIp);
	Sd_DstAddr.sin_family = AF_INET;
	Sd_DstAddr.sin_port = htons(remoteRecvPort);
	if (localSendPort != 0 || localSendIP != NULL)
	{
		if (localSendIP==NULL)
		{
			Sd_MyAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else
		{
			Sd_MyAddr.sin_addr.s_addr = inet_addr(localSendIP);
		}
		Sd_MyAddr.sin_family = AF_INET;
		Sd_MyAddr.sin_port = htons(remoteRecvPort);
		int err = bind(sock_send, (SOCKADDR*)&Sd_MyAddr, sizeof(SOCKADDR));
		if (INVALID_SOCKET != err)
		{
			m_SkStatus |= enSendOK;
			return true;
		}
		else
		{
			m_SkStatus &= ~enSendOK;
			return false;
		}
	}
	return false;
}

bool TmtSocket::SetRecvAddr(int localRecvPort, char* localRecvIP)
{
	if (m_SKInitialOK != true)
	{
		m_SkStatus = enNoSendOrRecv;
		return false;
	}
	if (localRecvIP==NULL)
	{
		Rc_MyAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		Rc_MyAddr.sin_addr.s_addr = inet_addr(localRecvIP);
	}
	Rc_MyAddr.sin_family = AF_INET;
	Rc_MyAddr.sin_port = htons(localRecvPort);
	int err = bind(sock_recv, (SOCKADDR*)&Rc_MyAddr, sizeof(SOCKADDR));
	if (INVALID_SOCKET != err)
	{
		m_SkStatus |= enRecvOK;
		return true;
	}
	else
	{
		m_SkStatus &= ~enRecvOK;
		return false;
	}
}

bool TmtSocket::SetOption(DWORD flag)
{
	if (m_SKInitialOK != true)
	{
		m_SkStatus = enNoSendOrRecv;
		return false;
	}
	if ((flag & ADDR_REUSE) == ADDR_REUSE)
	{
		int enable = 1;
		setsockopt(sock_recv, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int));
		setsockopt(sock_send, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int));
	}
	if ((flag & RECV_NOWAIT) == RECV_NOWAIT)
	{
		u_long mode = 1;
		ioctlsocket(sock_recv, FIONBIO, &mode);
	}
	return true;
}

int TmtSocket::SendMsg(void *pBuffer, size_t MsgLength)
{
	int recnum = sendto(sock_send, (char*)pBuffer, MsgLength, 0, (SOCKADDR*)&Sd_DstAddr, sizeof(Sd_DstAddr));
	return recnum;
}

int TmtSocket::RecvMsg(void *pBuffer, size_t bufLength, int *pRemoteSendPort, char* pRemoteSendIp)
{
	int len = sizeof(Rc_SrcAddr);
	int recvnum = recvfrom(sock_recv, (char*)pBuffer, bufLength, 0, (SOCKADDR*)&Rc_SrcAddr, &len);
	if (pRemoteSendPort !=NULL)
	{
		*pRemoteSendPort = Rc_SrcAddr.sin_port;
	}
	if (pRemoteSendIp !=NULL)
	{
		pRemoteSendIp = inet_ntoa(Rc_SrcAddr.sin_addr);
	}
	return recvnum;
}

int TmtSocket::ReSet()
{
	if (m_SKInitialOK != true)
	{
		m_SkStatus = enNoSendOrRecv;
		return 0;
	}
	try
	{
		closesocket(sock_send);
		closesocket(sock_recv);
		WSACleanup();
		DWORD Rc_wVersionRequest = MAKEWORD(2, 2);
		int err = WSAStartup(Rc_wVersionRequest, &wsadata);
		sock_recv = socket(AF_INET, SOCK_DGRAM, 0);
		sock_send = socket(AF_INET, SOCK_DGRAM, 0);
		m_SkStatus = enNoSendOrRecv;
	}
	catch (const std::exception&)
	{
		OutputDebugString(L"<TmtSocket::ReSet() failed>\n");
	}
	return 1;
}

int TmtSocket::ReleaseSocket()
{
	if (m_SKInitialOK != true)
	{
		m_SkStatus = enNoSendOrRecv;
		return 0;
	}
	closesocket(sock_send);
	closesocket(sock_recv);
	m_SkStatus = enNoSendOrRecv;
	return 1;
}


TmtSocketServer::TmtSocketServer(int bufferSize)
{
	bufferSize = max(MINBUFFERSIZE, bufferSize);
	pBuffer = 0;
	if (pBuffer==0)
	{
		pBuffer = new char[bufferSize];
		m_BufferSize = bufferSize;
	}
}

TmtSocketServer::~TmtSocketServer()
{
	if (pBuffer!=0)
	{
		delete[] pBuffer;
	}
}

bool TmtSocketServer::Initial(int remoteRecvPort, char * remoteRecvIp, int localRecvPort, char* localRecvIP, int localSendPort, char * localSendIP,  DWORD optionFlag)
{
	if (!SetOption(optionFlag))
	{
		return false;
	}
	else
	{
		bool isOK = SetSendAddr(remoteRecvPort, remoteRecvIp, localSendPort, localSendIP);
		isOK &= SetRecvAddr(localRecvPort, localRecvIP);
		return isOK;
	}
}

bool TmtSocketServer::Unitial()
{
	return ReleaseSocket()!=0;
}

void TmtSocketServer::Task(void)
{
	if (m_SkStatus == enRecvOK || m_SkStatus == enSendAndRecvOK)
	{
		int revLen = RecvMsg((void*)pBuffer, m_BufferSize);
		ServerProcess(revLen);
	}
}
