#include "CipherCode.h"

CipherCode::CipherCode()
{

}

CipherCode::~CipherCode()
{

}

std::string CipherCode::GetAvailableNetMac()
{
	PIP_ADAPTER_INFO pAdapter;
	PIP_ADAPTER_INFO pAdapterInfo;
	pAdapter = NULL;
	pAdapterInfo = NULL;
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (PIP_ADAPTER_INFO)malloc(ulOutBufLen);
	DWORD dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
	// ��һ�ε���GetAdapterInfo��ȡulOutBufLen��С
	if (dwRetVal == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
		dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
	}
	char MacAddress[TMTV_SHORTSTRLEN] = {0};
	string mStr;
	if (dwRetVal == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			mStr.clear();
			for (UINT i = 0; i < pAdapter->AddressLength; i++)
			{
				sprintf_s(MacAddress, "%02x", pAdapter->Address[i]);
				mStr.append(MacAddress);
			}
			bool mMacIsAvailable = false;
			if (mStr.length()>2)
			{
				for (int i = 0; i < mStr.length(); i++)
				{
					if (mStr.at(i) != '0')
					{
						mMacIsAvailable = true;
						break;
					}
				}
			}
			if (mMacIsAvailable==true)
			{
				break;
			}
			//printf("Adapter Name: \t%s\n", pAdapter->AdapterName);
			//printf("Adapter Desc: \t%s\n", pAdapter->Description);
			//printf("IP Address: \t%s\n", pAdapter->IpAddressList.IpAddress.String);
			//printf("IP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String);
			//printf("Gateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);
			pAdapter = pAdapter->Next;
		}
	}
	else
	{
		if (pAdapterInfo) free(pAdapterInfo);
		return mStr;
	}
	if (pAdapterInfo) free(pAdapterInfo);

	return mStr;

}

std::string CipherCode::Chars2Hex(string CharStr)
{
	string mstr;
	unsigned char a[1024] = {0};
	unsigned char b;
	for (int i = 0; i < CharStr.length();i++)
	{
		b = CharStr[i];
		sprintf_s((char*)a, 32, "%02x", (int)b);
		mstr.append((char*)a);
	}
	return mstr;
}

std::string CipherCode::Hex2Chars(string HexStr)
{
	BYTE high, low;
	int idx, ii = 0;
	unsigned char cbuf[TMTV_LONGSTRLEN] = {0};
	for (idx = 0; idx < HexStr.length(); idx += 2)
	{
		high = HexStr[idx];
		low = HexStr[idx + 1];

		if (high >= '0' && high <= '9')
			high = high - '0';
		else if (high >= 'A' && high <= 'F')
			high = high - 'A' + 10;
		else if (high >= 'a' && high <= 'f')
			high = high - 'a' + 10;
		else
			break;
		if (low >= '0' && low <= '9')
			low = low - '0';
		else if (low >= 'A' && low <= 'F')
			low = low - 'A' + 10;
		else if (low >= 'a' && low <= 'f')
			low = low - 'a' + 10;
		else
			break;
		cbuf[ii++] = high << 4 | low;
	}
	return string((char*)cbuf);
}

bool CipherCode::CheckRegistrInfo(const WStrPointer mFilePath)
{
	HUGESTR tmpstr = {0};
	int mcount = CMemoryFile::ReadMemoryFromFile(tmpstr, TMTV_HUGESTRLEN, 1, TMTV_HUGESTRLEN, mFilePath);
	if (mcount<=0)
	{
		return false;
	}
	tmpstr[mcount] = '\0';
	TmtCrypter mCrypt;
	string mMacstr;
	try
	{
		mMacstr = mCrypt.AESDecrypt(string(tmpstr));
	}
	catch (...)
	{
		return false;
	}
	bool backcode = CipherCode::CompareNetMac(mMacstr);
	return backcode;
}

bool CipherCode::CompareNetMac(string mMac)
{

	PIP_ADAPTER_INFO pAdapter;
	PIP_ADAPTER_INFO pAdapterInfo;
	pAdapter = NULL;
	pAdapterInfo = NULL;
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (PIP_ADAPTER_INFO)malloc(ulOutBufLen);
	DWORD dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
	// ��һ�ε���GetAdapterInfo��ȡulOutBufLen��С
	if (dwRetVal == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
		dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
	}
	char MacAddress[TMTV_SHORTSTRLEN] = { 0 };
	string mStr;
	if (dwRetVal == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			mStr.clear();
			for (UINT i = 0; i < pAdapter->AddressLength; i++)
			{
				sprintf_s(MacAddress, "%02x", pAdapter->Address[i]);
				mStr.append(MacAddress);
			}
			if (mMac.compare(mStr)==0)
			{
				if (pAdapterInfo) free(pAdapterInfo);
				return true;
			}
			//printf("Adapter Name: \t%s\n", pAdapter->AdapterName);
			//printf("Adapter Desc: \t%s\n", pAdapter->Description);
			//printf("IP Address: \t%s\n", pAdapter->IpAddressList.IpAddress.String);
			//printf("IP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String);
			//printf("Gateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);
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
