#pragma once
#include "CryptHeader\\cryptlib.h"
#include "CryptHeader\\aes.h"
#include "CryptHeader\\rsa.h"
#include "CryptHeader\\default.h"   
#include "CryptHeader\\randpool.h"
#include "CryptHeader\\hex.h" 
#include "CryptHeader\\files.h"  
#include <string>
using namespace CryptoPP;
using namespace std;

#pragma comment(lib, "cryptlib.lib")   

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
class TmtCrypter
{
public:
	TmtCrypter();
	~TmtCrypter();
	string AESEncrypt(string message);
	string AESDecrypt(string ciphertext);

private:
	 char m_AESkey[256];
	 char m_AESiv[256];
	int AESkeysize;// = 16;

};