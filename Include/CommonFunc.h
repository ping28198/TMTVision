#pragma once
///<proj_info>
//==============================================================================
// 项目名 : 图像处理平台
// 文件名 : CommonFunc.h
// 作  者 : 杨朝霖
// 用  途 : 定义了一些常用的操作函数
// 版  权 : 北京科技大学冶金工程研究院
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间      描述
//1.0     杨朝霖      2009-03-23   创建模块
//1.1     杨朝霖      2010-08-25   添加了进程权限修改函数
//1.2     杨朝霖      2010-08-29   将其它函数中字符串格式化得函数改为了SafeStringPrintf
//1.3     杨朝霖      2011-1-19    TimeOffset时间类型改为ULONGLONG
//1.4     王磊        2012-10-1    添加StringToDoubles\StringToInts\StringToTINYSTRS方法
//1.5     王磊        2013-2-20    添加GetNowDate得到的字符串只有年月日
//1.6     王磊        2013-2-28    添加WriteWStrToText得到的字符串只有年月日
//1.9     杨朝霖      2012-2-14    修改了SearchFile函数可能存在的内存访问问题
//2.0     杨朝霖      2012-2-15    添加了WriteStringToFile函数
//3.0     王磊        2014-4-28    消除了文件依赖
//3.1     王磊        2016-1-31    增加DeletDir
//==============================================================================
///</ver_info>

///<header_info>
//添加所需的头文件
//#include "MCSISysDataStruct.h"
//#include "windows.h"
///</header_info>

///<datastruct_info>
//==============================================================================
// 基本数据类型定义
typedef wchar_t FileItemName[256];
typedef struct tagFileNameSet
{
	enum {MAX_ITEM_NUM = 500};
	FileItemName  Items[MAX_ITEM_NUM];
	int  iItemNum;
}FileNameSet, *PFileNameSet;
//==============================================================================
///</datastruct_info>

///<class_info>
//==============================================================================
//名称:CCommonFunc
//功能:通过类静态函数的方式定义常用操作函数
class CCommonFunc
{
public:
	///<func_info>
	//描述: 获取当前进程的应用程序完整路径和文件名称
	//参数: wchar_t* strAppPath 读取到的文件名
	//size_t strLen 用于保存路径和文件名的缓冲区长度
	static void GetAppPathName(wchar_t* strAppPath , size_t strLen);
	///</func_info>
	
	///<func_info>
	//描述: 获取当前进程的应用程序所在路径
	//参数: wchar_t* strAppPath 读取到的路径
	//size_t strLen 用于保存路径名的缓冲区长度
	static void GetAppPath(wchar_t* strAppPath , size_t strLen);
	///</func_info>
	
	///<func_info>
	//描述: 获取指定模块的所在路径
	//参数: HMODULE hModuel 模块的句柄, 如果为NULL, 表示获取当前应用程序所在的路径
	// wchar_t* strPath 保存路径的缓冲区
	//size_t strLen 用于保存路径名的缓冲区长度
	static void GetModulePath(void* hModuel, wchar_t* strPath , size_t strLen);
	///</func_info>
	
	///<func_info>
	//描述: 获取当前进程的应用程序名称, 不包括路径
	//参数: wchar_t* strAppName 应用程序的名称
	//size_t strLen 用于保存文件名的缓冲区长度
	static void GetAppName(wchar_t* strAppName, size_t iStrLen);
	///</func_info>
	
	///<func_info>
	//描述: 从完整路径文件名中获取文件名
	//参数: wchar_t* strPathName 完整的路径文件名
	// wchar_t* strFileName 保存文件名的缓冲
	//size_t strLen 用于保存文件名的缓冲区长度
	static void GetFileName(wchar_t* strPathName, wchar_t* strFileName, size_t iStrFileNameLen);
	///</func_info>
	
	///<func_info>
	//描述: 获取文件名的后缀
	//参数: wchar_t* strPathName 完整的路径文件名
	// wchar_t* strExt 保存后缀的缓冲
	//size_t strLen 用于保存后缀的缓冲区长度
	static void GetFileExt(wchar_t* strPathName, wchar_t* strExt, size_t iStrExtLen); 
	///</func_info>

	///<func_info>
	//描述: 获取不包括后缀的文件名
	//参数: wchar_t* strPathName 完整的路径文件名
	// wchar_t* strPathName 保存后缀的缓冲
	static void GetFileNoExt(wchar_t* strPathName, wchar_t* strName, int iNameLen); 
	///</func_info>

	
	///<func_info>
	//描述: 从完整路径文件名中获取路径
	//参数: wchar_t* strPathName 完整的路径文件名
	// wchar_t* strPath 保存路径的缓冲
	//size_t iStrPathLen 用于保存路径的缓冲区长度
	static void GetFilePath(wchar_t* strPathName, wchar_t* strPath, size_t iStrPathLen);//从文件名获取路径名
	///</func_info>

	///<func_info>
	//描述: 创建目录, 支持多级目录的创建
	//参数: wchar_t* strDir 要创建的目录
	static void CreateDir(wchar_t* strDir);
	///</func_info>

	///<func_info>
	//描述: 判断目录是否存在
	//参数: wchar_t* strDir 要判断的目录
	//返回值:true表示存在, false表示不存在
	static bool DirExist(wchar_t* strDir);
	///</func_info>

	///<func_info>
	//描述: 判断目录是否存在
	//参数: wchar_t* strDir 要判断的目录
	//返回值:true表示存在, false表示不存在
	static bool DelDir(wchar_t* strDir);
	///</func_info>

	///<func_info>
	//描述: 判断文件是否存在
	//参数: wchar_t* strDir 要判断的目录
	//返回值:true表示存在, false表示不存在
	static bool FileExist(wchar_t* strFile);
	///</func_info>

	///<func_info>
	//描述: 判断目录是否存在
	//参数: wchar_t* strDir 要判断的目录
	//返回值:true表示存在, false表示不存在
	static bool DelFile(wchar_t* strDir);
	///</func_info>

	///<func_info>
	//描述: 目录中的文件数（仅限于1级）
	//参数: wchar_t* strDir 指定的目录名
	//      wchar_t* strExt 指定的后缀, 格式如"exe",不是".exe"
	//返回值:文件的数目
	static int  FileNumInDir(wchar_t* strDir,  wchar_t* strExt);
	///</func_info>

	///<func_info>
	//描述: 进行文件复制
	//参数: wchar_t* strSourceFile 指定的源文件
	//     wchar_t* strDestFile 指定的目标文件
	//返回值:true 表示成功, false表示失败
	static bool  CopyModuleFile(wchar_t* strSourceFile, wchar_t* strDestFile); 
	///</func_info>

	///<func_info>
	//描述: 获取桌面尺寸
	//参数: int& iWidth   桌面的宽度
	//      int&iHeight   桌面的高度
	static void  GetDesktopSize(int& iWidth, int&iHeight); 
	///</func_info>

	///<func_info>
	//描述: 搜索指定目录下的文件(仅限一级), 但要求搜索到的文件数量小于tagFileNameSet::MAX_ITEM_NUM
	//参数: wchar_t* strDir 指定的目录
	//      wchar_t* strExt 指定的文件名后缀
	//      FileNameSet& Set 搜索到文件名集合
	//返回值:true 表示成功, false表示失败
	static bool SearchFile(wchar_t* strDir, wchar_t* strExt,FileNameSet& Set); 
	///</func_info>

	///<func_info>
	//描述: 搜索指定目录下的文件(仅限一级), 如果pFileItems = NULL, 则返回搜索到的图像数量, 调用程序需要分配pFileItems
	//参数: wchar_t* strDir 指定的目录
	//      wchar_t* strExt 指定的文件名后缀,不需要加上.. 
	//      FileItemName* pFileItems 搜索到文件名数组
	//		 int& iFindItemNum 搜索到的文件名数量
	//返回值:true 表示成功, false表示失败
    static bool SearchFile(wchar_t* strDir, wchar_t* strExt,FileItemName* pFileItems,  int& iFindItemNum);
	///</func_info>

	///<func_info>
	//描述: 搜索指定目录下的子目录(仅限一级), 但要求搜索到的目录数量小于tagFileNameSet::MAX_ITEM_NUM
	//参数: wchar_t* strDir 指定的目录
	//      FileNameSet& Set搜索到的目录数组
	//返回值:true 表示成功, false表示失败
	static bool SearchDir(wchar_t* strDir, FileNameSet& Set); 
	///</func_info>

	///<func_info>
	//描述: 搜索指定目录下的子目录(仅限一级), 搜索到的目录数量没有限制
	//参数: wchar_t* strDir 指定的目录
	//      FileNameSet& Set搜索到的目录数组
	//返回值:true 表示成功, false表示失败
	static bool SearchDir(wchar_t* strDir, FileItemName* pFileItems,  int& iFindItemNum); 
	///</func_info>

	
	///<func_info>
	//描述: 多字节到宽字节的转换
	//参数: char* strMutiByte 需要转换的多字节缓冲
	//      wchar_t* strWideChar 转换后的宽字节缓冲
	//      int strWideCharLen 宽字节缓冲区的最大尺寸
	static void  AnsiToUnicode(char* strMutiByte, wchar_t* strWideChar, int strWideCharLen);
	///</func_info>
	
	///<func_info>
	//描述: 宽字节到多字节的转换
	//参数: wchar_t* strWideChar 需要转换的宽字节缓冲
	//      char* strMutiByte 转换后的多字节缓冲
	//      int strMutiByteLen 多字节缓冲区的最大尺寸
	static void  UnicodeToAnsi(wchar_t* strWideChar, char* strMutiByte, int strMutiByteLen);
	///</func_info>

	///<func_info>
	//描述: 安全的宽字符串格式化函数
	//参数: wchar_t* strDest 需要格式化的宽字节缓冲
	//      size_t iStrLen  宽字节缓冲的最大尺寸(以字符的个数为单位, 不是字节大小,可用STR_LEN宏获取)
	//      int strMutiByteLen 多字节缓冲区的最大尺寸
	static void  SafeStringPrintf(wchar_t* strDest, size_t iStrLen, wchar_t* strFormat, ...);
	///</func_info>

	///<func_info>
	//描述: 安全的宽字符串拷贝函数
	//参数: wchar_t* strDest 需要拷贝的目的宽字节缓冲
	//      size_t iStrLen  宽字节缓冲的最大尺寸(以字符的个数为单位, 不是字节大小,可用STR_LEN宏获取)
	//      wchar_t* strSource 需要拷贝的源宽字节缓冲
	static void  SafeStringCpy(wchar_t* strDest, size_t iStrLen, wchar_t* strSource);


	///</func_info>
	///</func_info>

	///<func_info>
	//描述: 获取当前时间, 并格式化为字符串
	//参数: wchar_t*  strNowTime 当前时间的字符串
	//      int iDestLen  时间字符串的最大尺寸(以字符的个数为单位, 不是字节大小,可用STR_LEN宏获取)
	//     bool bHigh 是否支持高精度的时间信息, true表示支持
	static void  GetNowTime(wchar_t*  strNowTime,  int iDestLen, bool bHigh = false);
	///</func_info>

	///<func_info>
	//描述: 获取当前时间, 并格式化为字符串
	//参数: wchar_t*  strNowTime 当前时间的字符串
	//      int iDestLen  时间字符串的最大尺寸(以字符的个数为单位, 不是字节大小,可用STR_LEN宏获取)
	static void  GetNowDate(wchar_t*  strNowTime,  int iDestLen);
	///</func_info>

	///<func_info>
	//描述: 获取文件的相关时间信息
	//参数:wchar_t* strFileName 指定的完整路径文件名
	//	SYSTEMTIME& tmCreate 文件的创建时间
	//   SYSTEMTIME& tmAccess 文件的访问时间
	//	SYSTEMTIME&  tmWrite 文件的修改时间
	//static bool   GetFileTimes(wchar_t* strFileName, SYSTEMTIME& tmCreate, SYSTEMTIME& tmAccess, SYSTEMTIME&  tmWrite);
	///</func_info>

	///<func_info>
	//描述: 运行指定位置的程序
	//参数:
	//	wchar_t* strAppName 需要运行的程序, 可以带有命令行参数
	static bool  RunApp(wchar_t* strAppName);
	///</func_info>

	///<func_info>
	//描述: 输出数据到指定文本中
	//参数:
	//	wchar_t* strAppName 需要运行的程序, 可以带有命令行参数
	static bool  WriteFloatToText(wchar_t* strFileName, double* pData, int iDatalen, int iColNum = 20);
	///</func_info>

	///<func_info>
	//描述: 输出数据到指定文本中
	//参数:
	//	wchar_t* strAppName 需要运行的程序, 可以带有命令行参数
	static bool  WriteIntToText(wchar_t* strFileName, int* pData, int iDatalen, int iColNum = 20);
	///</func_info>
	///<func_info>
	//描述: 输出数据到指定文本中
	//参数:
	//	wchar_t* strAppName 需要运行的程序, 可以带有命令行参数
	static bool  WriteBufferToText(wchar_t* strFileName, unsigned char* pData, int iDatalen, int iColNum = 20);
	///</func_info>
	///</func_info>
	///<func_info>
	//描述: 输出数据到指定文本中
	//参数:
	//	wchar_t* strAppName 需要运行的程序, 可以带有命令行参数
	static bool  WriteWStrToText(wchar_t* strFileName, wchar_t* pData, int strLen);
	///</func_info>

	//描述: 将字符串{1,1,1}转换为double型数组
	//参数:以字符的个数为单位, 不是字节大小,可用STR_LEN宏获取
	//	wchar_t* strAppName 需要运行的程序, 可以带有命令行参数
	static bool  StringToDoubles(wchar_t* srcString,  int strLen,double* pData, int& dataLen);
	///</func_info>

	//描述: 将字符串{1,1,1}转换为int型数组
	//参数:以字符的个数为单位, 不是字节大小,可用STR_LEN宏获取
	//	wchar_t* strAppName 需要运行的程序, 可以带有命令行参数
	static bool  StringToInts(wchar_t* srcString,  int strLen,int* pData, int& dataLen);
	///</func_info>
private:
	CCommonFunc(void);
	~CCommonFunc(void);
};
//==============================================================================
///</class_info>