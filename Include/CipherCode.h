#pragma once

#include "TmtSocket.h"
#include "TmtCrypt.h"
#include "MemoryFile.h"


class CipherCode
{
public:
	CipherCode();
	~CipherCode();
	//************************************
	// ����:  ���Ҽ�����е������Ƿ���ڸ�����Mac��ַ
	// ����:  CipherCode::CompareNetMac
	// Access:    public static 
	// ����ֵ:   bool
	// ����ֵ����: �Ƿ����
	// ����:   string mMac
	// ��������: ������mac��ַ��Сд������-�ţ�
	//************************************
	static bool CompareNetMac(string mMac);


	//************************************
	// ����:  ��ȡ������е�һ����Ч������mac��ַ
	// ����:  CipherCode::GetAvailableNetMac
	// Access:    public static 
	// ����ֵ:   std::string
	// ����ֵ����: mac��ַ��Сд������-�ţ�
	// ��������: 
	//************************************
	static string GetAvailableNetMac();
	//************************************
	// ����:  ���ַ���תΪ��16���Ʊ�ʾ���ַ���
	// ����:  CipherCode::Chars2Hex
	// Access:    public static 
	// ����ֵ:   std::string
	// ����ֵ����: ת������,���磺12d3ef1a
	// ����:   string CharStr
	// ��������: ��ת���ַ���
	//************************************
	static string Chars2Hex(string CharStr);
	//************************************
	// ����:  ��16�����ַ���ʾ���ַ���תΪ��׼�ַ���
	// ����:  CipherCode::Hex2Chars
	// Access:    public static 
	// ����ֵ:   std::string
	// ����ֵ����: ת������
	// ����:   string HexStr
	// ��������: 16�����ַ���ʾ���ַ��������磺12d3ef1a
	//************************************
	static string Hex2Chars(string HexStr);

	//************************************
	// ����:  ����ļ�ָ���ļ����Ƿ���ںͱ���mac��ַƥ���ע����Ϣ������AES���ܣ�
	// ˵��:  
	// ����:  CipherCode::CheckRegistrInfo
	// Access:    public static 
	// ����ֵ:   bool
	// ����ֵ����: �Ƿ����
	// ����:   const WStrPointer mFilePath
	// ��������: ָ���ļ���ָ��
	//************************************
	static bool CheckRegistrInfo(const WStrPointer mFilePath);

};