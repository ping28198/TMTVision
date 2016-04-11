#include "TmtSocket.h"
#include "CommonFunc.h"
#include "ObjToString.h"
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

bool TmtSocket::GetAvailableNetIP(NetIP& localIP)
{
	PIP_ADAPTER_INFO pAdapter;
	PIP_ADAPTER_INFO pAdapterInfo;
	pAdapter = NULL;
	pAdapterInfo = NULL;
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (PIP_ADAPTER_INFO)malloc(ulOutBufLen);
	DWORD dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
	// 第一次调用GetAdapterInfo获取ulOutBufLen大小
	if (dwRetVal == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
		dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
	}
	char mStr[TMTV_SHORTSTRLEN] = { 0 };
	if (dwRetVal == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			mStr[0]=0;
			strcpy_s(mStr, TMTV_SHORTSTRLEN,pAdapter->IpAddressList.IpAddress.String);
			//printf("Adapter Name: \t%s\n", pAdapter->AdapterName);
			//printf("Adapter Desc: \t%s\n", pAdapter->Description);
			//printf("IP Address: \t%s\n", );
			//printf("IP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String);
			//printf("Gateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);
			if (!(mStr[0] == '0' && mStr[1] == '.' && mStr[2] == '0' && mStr[3] == '.' && mStr[4] == '0'
				&& mStr[5] == '.' && mStr[6] == '0'))
			{
				strcpy_s(localIP, TMTV_IPSTRLEN, mStr);
				return true;
			}
			pAdapter = pAdapter->Next;
		}
	}
	else
	{
		if (pAdapterInfo) free(pAdapterInfo);
		return false;
	}
	if (pAdapterInfo) free(pAdapterInfo);
	return false;
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
			m_LocalSendIP[0] = 0;
			Sd_MyAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else
		{
			strcpy_s(m_LocalSendIP, TMTV_IPSTRLEN, localSendIP);
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
			//strcpy_s(m_LocalSendIP, TMTV_IPSTRLEN, localSendIP);

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
	NetIPW m_RemoteRecvIPW = { 0 };
	NetIPW m_LocalSendIPW = { 0 };
	NetIPW m_LocalRecvIPW = { 0 };
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

bool TmtSocketServer::Initial(int remoteRecvPort, char * remoteRecvIp, 
	                          int localRecvPort, char* localRecvIP, 
	                          int localSendPort, char * localSendIP,  DWORD optionFlag)
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
	MEGAWSTR tmpStr1 = {0};
	TmtSocket::ToString(tmpStr1, 0, 0);
	MEGAWSTR tmpStr2 = { 0 };
	ObjToString::ToString(tmpStr2, (void*)pBuffer, m_BufferSize, method, 0);
	if (method >= 0 && method <= 2)
	{
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"<TmtSocketServer>\n");
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s%s", string, tmpStr1);
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s%s", string, tmpStr2);
		CCommonFunc::SafeWStringPrintf(string, TMTV_HUGESTRLEN, L"%s</TmtSocketServer>\n", string);
	}
	if (color >= 30 && color <= 39)
	{
		MEGAWSTR testString = { 0 };
		CCommonFunc::SafeWStringPrintf(testString, TMTV_HUGESTRLEN, L"\033[;%dm%s\033[0m\n", color, string);
		CCommonFunc::SafeWStringCpy(string, TMTV_HUGESTRLEN, testString);
	}
}