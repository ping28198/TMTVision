#include "TmtSocket.h"
#include "CommonFunc.h"
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
		m_OptionFlag=0;
		m_RemoteRecvPort=0;
		m_RemoteRecvIP[0]=0;
		m_LocalSendPort = 0;
		m_LocalSendIP[0] = 0;
		m_LocalRecvPort = 0;
		m_LocalRecvIP[0] = 0;
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
		m_RemoteRecvPort = 0;
		m_RemoteRecvIP[0] = 0;
		m_LocalSendPort = 0;
		m_LocalSendIP[0] = 0;
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

			m_RemoteRecvPort = remoteRecvPort;
			strcpy_s(m_RemoteRecvIP, TMTV_IPSTRLEN, remoteRecvIp);
			m_LocalSendPort = localSendPort;
			strcpy_s(m_LocalSendIP, TMTV_IPSTRLEN, localSendIP);

			return true;
		}
		else
		{
			m_SkStatus &= ~enSendOK;

			m_RemoteRecvPort = 0;
			m_RemoteRecvIP[0] = 0;
			m_LocalSendPort = 0;
			m_LocalSendIP[0] = 0;

			return false;
		}
	}

	return true;
}

bool TmtSocket::SetRecvAddr(int localRecvPort, char* localRecvIP)
{
	if (m_SKInitialOK != true)
	{
		m_SkStatus = enNoSendOrRecv;
		m_LocalRecvPort = 0;
		m_LocalRecvIP[0] = 0;
		return false;
	}
	if (localRecvIP==NULL)
	{
		m_LocalRecvIP[0] = 0;
		Rc_MyAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		strcpy_s(m_LocalRecvIP, TMTV_IPSTRLEN, localRecvIP);
		Rc_MyAddr.sin_addr.s_addr = inet_addr(localRecvIP);
	}
	Rc_MyAddr.sin_family = AF_INET;
	Rc_MyAddr.sin_port = htons(localRecvPort);
	int err = bind(sock_recv, (SOCKADDR*)&Rc_MyAddr, sizeof(SOCKADDR));
	if (INVALID_SOCKET != err)
	{
		m_SkStatus |= enRecvOK;
		m_LocalRecvPort = localRecvPort;
		return true;
	}
	else
	{
		m_SkStatus &= ~enRecvOK;
		m_LocalRecvPort = 0;
		m_LocalRecvIP[0] = 0;
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

	m_LocalRecvPort = 0;
	m_LocalRecvIP[0] = 0;
	m_RemoteRecvPort = 0;
	m_RemoteRecvIP[0] = 0;
	m_LocalSendPort = 0;
	m_LocalSendIP[0] = 0;

	return 1;
}

void TmtSocket::ToString(MEGAWSTR & string, int method, int color)
{
	NetIPW m_RemoteRecvIPW;
	NetIPW m_LocalSendIPW;
	NetIPW m_LocalRecvIPW;
	CCommonFunc::AnsiToUnicode(m_RemoteRecvIP, m_RemoteRecvIPW, TMTV_IPSTRLEN);
	CCommonFunc::AnsiToUnicode(m_LocalSendIP, m_LocalSendIPW, TMTV_IPSTRLEN);
	CCommonFunc::AnsiToUnicode(m_LocalRecvIP, m_LocalRecvIPW, TMTV_IPSTRLEN);

	string[0] = 0;
	if (1)
	{
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<TmtSocket>\n");
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s<m_RemoteRecvIP=\"%s\" m_RemoteRecvPort=%d>\n",
			string, m_RemoteRecvIPW, m_RemoteRecvPort);
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s<m_LocalSendIP=\"%s\" m_LocalSendPort=%d>\n",
			string, m_LocalSendIPW, m_LocalSendPort);
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s<m_LocalRecvIP=\"%s\" m_LocalRecvPort=%d>\n",
			string, m_LocalRecvIPW, m_LocalRecvPort);
		//CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s<pBuffer>\n\"%s\"\n</pBuffer>\n", string, pBuffer);
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s</TmtSocket>\n", string);
	}
	//#define COL(x)  L"\033[;" #x L"m"  
	//#define RED     COL(31)  
	//#define GREEN   COL(32)  
	//#define YELLOW  COL(33)  
	//#define BLUE    COL(34)  
	//#define MAGENTA COL(35)  
	//#define CYAN    COL(36)  
	//#define WHITE   COL(0)
	//#define GRAY    L"\033[0m" 
	if (color >= 30 && color <= 39)
	{
		MEGAWSTR testString = { 0 };
		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, string);
		CCommonFunc::SafeWStringCpy(string, TMTV_HUGESTRLEN, testString);
	}
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
	ForceEnd();
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
	ForceEnd();
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

void TmtSocketServer::ToString(MEGAWSTR & string, int method, int color)
{
	string[0] = 0;
	MEGAWSTR tmpStr = {0};
	TmtSocket::ToString(tmpStr, 0, 0);
	if (method == 0)
	{
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<TmtSocketServer>\n");
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s%s", string, tmpStr);
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s<pBuffer>\n\"%s\"\n</pBuffer>\n", string, pBuffer);
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s</TmtSocketServer>\n", string);
	}
	else if (method == 1)
	{
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<TmtSocketServer>\n");
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s%s", string, tmpStr);
		//CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s<pBuffer>\n\"%s\"\n</pBuffer>\n", string, pBuffer);
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s</TmtSocketServer>\n", string);
	}
	else if (method == 2)
	{
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<TmtSocketServer>\n");
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s%s", string, tmpStr);
		//CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s<pBuffer>\n\"%s\"\n</pBuffer>\n", string, pBuffer);
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s</TmtSocketServer>\n", string);
	}
	//#define COL(x)  L"\033[;" #x L"m"  
	//#define RED     COL(31)  
	//#define GREEN   COL(32)  
	//#define YELLOW  COL(33)  
	//#define BLUE    COL(34)  
	//#define MAGENTA COL(35)  
	//#define CYAN    COL(36)  
	//#define WHITE   COL(0)
	//#define GRAY    L"\033[0m" 
	if (color >= 30 && color <= 39)
	{
		MEGAWSTR testString = { 0 };
		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, string);
		CCommonFunc::SafeWStringCpy(string, TMTV_HUGESTRLEN, testString);
	}
}