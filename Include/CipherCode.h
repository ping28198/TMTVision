#pragma once

#include "TmtSocket.h"
#include "TmtCrypt.h"
#include "MemoryFile.h"


class CipherCode
{
public:
	CipherCode();
	~CipherCode();
	static bool CompareNetMac(string mMac);


	static string GetAvailableNetMac();
	static string Chars2Hex(string CharStr);
	static string Hex2Chars(string HexStr);

	static bool CheckRegistrInfo();

};