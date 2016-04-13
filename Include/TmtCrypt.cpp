#include "Tmtcrypt.h"


TmtCrypter::TmtCrypter()
{
	AESkeysize = 16;
	//m_AESkey = new  char[AESkeysize +1];// = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08, 0x01,0x02, 0x03,0x04,0x05,0x06,0x07,0x08 };//AES::DEFAULT_KEYLENGTH  
	//m_AESiv = new  char[AESkeysize + 1];//[] = { 0x01,0x02,0x03,0x03,0x03,0x03,0x03,0x03, 0x03,0x03, 0x01,0x02,0x03,0x03,0x03,0x03 };
	sprintf_s(m_AESkey, 256, "TMTcodeForCrypt0");
	sprintf_s(m_AESiv, 256, "TMTabc1234567890");


}

TmtCrypter::~TmtCrypter()
{
	
}

std::string TmtCrypter::AESEncrypt(string message)
{
	string strEncTxt;
	CBC_Mode<AES>::Encryption  Encryptor1((unsigned char*)m_AESkey, AESkeysize, (unsigned char*)m_AESiv);
	StringSource(message,
		true,
		new StreamTransformationFilter(Encryptor1,
			new StringSink(strEncTxt),
			BlockPaddingSchemeDef::BlockPaddingScheme::ONE_AND_ZEROS_PADDING,
			true)
		);
	return strEncTxt;
}

std::string TmtCrypter::AESDecrypt(string ciphertext)
{
	string strDecTxt;
	CBC_Mode<AES>::Decryption Decryptor1((unsigned char*)m_AESkey, AESkeysize, (unsigned char*)m_AESiv);
	StringSource(ciphertext,
		true,
		new StreamTransformationFilter(Decryptor1,
			new StringSink(strDecTxt),
			BlockPaddingSchemeDef::BlockPaddingScheme::ONE_AND_ZEROS_PADDING,
			true)
		);
	return strDecTxt;
}

