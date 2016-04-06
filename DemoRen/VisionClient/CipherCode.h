#pragma once

#include "TmtSocket.h"
#include "TmtCrypt.h"




class CipherCode
{
public:
	CipherCode();
	~CipherCode();
	bool CompareNetMac(string mMac);


	string GetAvailableNetMac();
	string Chars2Hex(string CharStr);
	string Hex2Chars(string HexStr);


};