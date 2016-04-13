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
	// 作用:  查找计算机中的网卡是否存在给定的Mac地址
	// 名称:  CipherCode::CompareNetMac
	// Access:    public static 
	// 返回值:   bool
	// 返回值含义: 是否存在
	// 参数:   string mMac
	// 参数含义: 给定的mac地址，小写（不含-号）
	//************************************
	static bool CompareNetMac(string mMac);


	//************************************
	// 作用:  获取计算机中的一个有效的网卡mac地址
	// 名称:  CipherCode::GetAvailableNetMac
	// Access:    public static 
	// 返回值:   std::string
	// 返回值含义: mac地址，小写（不含-号）
	// 参数含义: 
	//************************************
	static string GetAvailableNetMac();
	//************************************
	// 作用:  将字符串转为以16进制表示的字符串
	// 名称:  CipherCode::Chars2Hex
	// Access:    public static 
	// 返回值:   std::string
	// 返回值含义: 转换后结果,例如：12d3ef1a
	// 参数:   string CharStr
	// 参数含义: 待转换字符串
	//************************************
	static string Chars2Hex(string CharStr);
	//************************************
	// 作用:  将16进制字符表示的字符串转为标准字符串
	// 名称:  CipherCode::Hex2Chars
	// Access:    public static 
	// 返回值:   std::string
	// 返回值含义: 转换后结果
	// 参数:   string HexStr
	// 参数含义: 16进制字符表示的字符串，例如：12d3ef1a
	//************************************
	static string Hex2Chars(string HexStr);

	//************************************
	// 作用:  检测文件指定文件中是否存在和本机mac地址匹配的注册信息（已由AES加密）
	// 说明:  
	// 名称:  CipherCode::CheckRegistrInfo
	// Access:    public static 
	// 返回值:   bool
	// 返回值含义: 是否存在
	// 参数:   const WStrPointer mFilePath
	// 参数含义: 指定文件名指针
	//************************************
	static bool CheckRegistrInfo(const WStrPointer mFilePath);

};