#include "TmtSocket.h"


TmtSocket::TmtSocket()
{
	DWORD Rc_wVersionRequest = MAKEWORD(2, 2);
	err = WSAStartup(Rc_wVersionRequest, &wsadata);

	sock_recv = socket(AF_INET, SOCK_DGRAM, 0);
	sock_send = socket(AF_INET, SOCK_DGRAM, 0);

}

TmtSocket::~TmtSocket()
{
	closesocket(sock_send);
	closesocket(sock_recv);
	WSACleanup();
}

bool TmtSocket::SetSendAddr(char* DstIp, int DstPort, int MyPort/*=0*/, char* MyIp/*=NULL*/)
{
	Sd_DstAddr.sin_addr.s_addr = inet_addr(DstIp);
	Sd_DstAddr.sin_family = AF_INET;
	Sd_DstAddr.sin_port = htons(DstPort);
	if (MyPort != 0 || MyIp != NULL)
	{
		if (MyIp==NULL)
		{
			Sd_MyAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else
		{
			Sd_MyAddr.sin_addr.s_addr = inet_addr(MyIp);
		}
		Sd_MyAddr.sin_family = AF_INET;
		Sd_MyAddr.sin_port = htons(DstPort);
		err = bind(sock_send, (SOCKADDR*)&Sd_MyAddr, sizeof(SOCKADDR));
	}
	return true;
}

bool TmtSocket::SetRecvAddr(int MyPort, char* MyIp)
{
	if (MyIp==NULL)
	{
		Rc_MyAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		Rc_MyAddr.sin_addr.s_addr = inet_addr(MyIp);
	}
	Rc_MyAddr.sin_family = AF_INET;
	Rc_MyAddr.sin_port = htons(MyPort);
	err = bind(sock_recv, (SOCKADDR*)&Rc_MyAddr, sizeof(SOCKADDR));
	return true;
}

bool TmtSocket::SetOption(int flag)
{
	switch (flag)
	{
	case ADDR_REUSE:
	{
		int enable = 1;
		setsockopt(sock_recv, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int));
		setsockopt(sock_send, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int));
		return true;
	}
	case RECV_NOWAIT:
	{
		u_long mode = 1;
		ioctlsocket(sock_recv, FIONBIO, &mode);
		return true;
	}

	default:
		break;
	}
	return false;
}

int TmtSocket::SendMsg(void *pBuffer, size_t MsgLength)
{
	int recnum = sendto(sock_send, (char*)pBuffer, MsgLength, 0, (SOCKADDR*)&Sd_DstAddr, sizeof(Sd_DstAddr));
	return recnum;
}

int TmtSocket::RecvMsg(void *pBuffer, size_t bufLength, int *SrcPort /*= NULL*/, char* SrcIp /*= NULL*/)
{
	int len = sizeof(Rc_SrcAddr);
	int recvnum = recvfrom(sock_recv, (char*)pBuffer, bufLength, 0, (SOCKADDR*)&Rc_SrcAddr, &len);
	if (SrcPort!=NULL)
	{
		*SrcPort = Rc_SrcAddr.sin_port;
	}
	if (SrcIp!=NULL)
	{
		SrcIp = inet_ntoa(Rc_SrcAddr.sin_addr);
	}
	return recvnum;
}

int TmtSocket::ReSet()
{
	closesocket(sock_send);
	closesocket(sock_recv);
	WSACleanup();
	DWORD Rc_wVersionRequest = MAKEWORD(2, 2);
	err = WSAStartup(Rc_wVersionRequest, &wsadata);
	sock_recv = socket(AF_INET, SOCK_DGRAM, 0);
	sock_send = socket(AF_INET, SOCK_DGRAM, 0);
	return 1;
}

int TmtSocket::ReleaseSocket()
{
	closesocket(sock_send);
	closesocket(sock_recv);
	return 1;
}
