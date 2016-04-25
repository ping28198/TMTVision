#pragma once
///<proj_info>
//==============================================================================
// ��Ŀ�� : ͼ����ƽ̨
// �ļ��� : CommonFunc.h
// ��  �� : ���
// ��  ; : ������һЩ���õĲ�������
// ��  Ȩ : �����Ƽ���ѧұ�𹤳��о�Ժ
//==============================================================================
///</proj_info>

///<ver_info>
// �汾��¼	
//==============================================================================
//�汾��  ������Ա      ʱ��      ����
//1.0     ���      2009-03-23   ����ģ��
//1.1     ���      2010-08-25   ����˽���Ȩ���޸ĺ���
//1.2     ���      2010-08-29   �������������ַ�����ʽ���ú�����Ϊ��SafeWStringPrintf
//1.3     ���      2011-1-19    TimeOffsetʱ�����͸�ΪULONGLONG
//1.4     ����        2012-10-1    ���StringToDoubles\StringToInts\StringToTINYSTRS����
//1.5     ����        2013-2-20    ���GetNowDate�õ����ַ���ֻ��������
//1.6     ����        2013-2-28    ���WriteWStrToText�õ����ַ���ֻ��������
//1.9     ���      2012-2-14    �޸���SearchFile�������ܴ��ڵ��ڴ��������
//2.0     ���      2012-2-15    �����WriteStringToFile����
//3.0     ����        2014-4-28    �������ļ�����
//3.1     ����        2016-1-31    ����DeletDir
//==============================================================================
///</ver_info>

///<header_info>
//��������ͷ�ļ�
//#include "MCSISysDataStruct.h"
#include <Windows.h>
///</header_info>

///<datastruct_info>
//==============================================================================
// �����������Ͷ���
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
//����:CCommonFunc
//����:ͨ���ྲ̬�����ķ�ʽ���峣�ò�������
class CCommonFunc
{
public:
	///<func_info>
	//����: ��ȡ��ǰ���̵�Ӧ�ó�������·�����ļ�����
	//����: wchar_t* strAppPath ��ȡ�����ļ���
	//size_t strLen ���ڱ���·�����ļ����Ļ���������
	static void GetAppPathName(wchar_t* strAppPath , size_t strLen);
	///</func_info>
	
	///<func_info>
	//����: ��ȡ��ǰ���̵�Ӧ�ó�������·��
	//����: wchar_t* strAppPath ��ȡ����·��
	//size_t strLen ���ڱ���·�����Ļ���������
	static void GetAppPath(wchar_t* strAppPath , size_t strLen);
	///</func_info>
	
	///<func_info>
	//����: ��ȡָ��ģ�������·��
	//����: HMODULE hModuel ģ��ľ��, ���ΪNULL, ��ʾ��ȡ��ǰӦ�ó������ڵ�·��
	// wchar_t* strPath ����·���Ļ�����
	//size_t strLen ���ڱ���·�����Ļ���������
	static void GetModulePath(void* hModuel, wchar_t* strPath , size_t strLen);
	///</func_info>
	
	///<func_info>
	//����: ��ȡ��ǰ���̵�Ӧ�ó�������, ������·��
	//����: wchar_t* strAppName Ӧ�ó��������
	//size_t strLen ���ڱ����ļ����Ļ���������
	static void GetAppName(wchar_t* strAppName, size_t iStrLen);
	///</func_info>
	
	///<func_info>
	//����: ������·���ļ����л�ȡ�ļ���
	//����: wchar_t* strPathName ������·���ļ���
	// wchar_t* strFileName �����ļ����Ļ���
	//size_t strLen ���ڱ����ļ����Ļ���������
	static void GetFileName(wchar_t* strPathName, wchar_t* strFileName, size_t iStrFileNameLen);
	///</func_info>
	
	///<func_info>
	//����: ��ȡ�ļ����ĺ�׺
	//����: wchar_t* strPathName ������·���ļ���
	// wchar_t* strExt �����׺�Ļ���
	//size_t strLen ���ڱ����׺�Ļ���������
	static void GetFileExt(wchar_t* strPathName, wchar_t* strExt, size_t iStrExtLen); 
	///</func_info>

	///<func_info>
	//����: ��ȡ��������׺���ļ���
	//����: wchar_t* strPathName ������·���ļ���
	// wchar_t* strPathName �����׺�Ļ���
	static void GetFileNoExt(wchar_t* strPathName, wchar_t* strName, int iNameLen); 
	///</func_info>

	
	///<func_info>
	//����: ������·���ļ����л�ȡ·��
	//����: wchar_t* strPathName ������·���ļ���
	// wchar_t* strPath ����·���Ļ���
	//size_t iStrPathLen ���ڱ���·���Ļ���������
	static void GetFilePath(wchar_t* strPathName, wchar_t* strPath, size_t iStrPathLen);//���ļ�����ȡ·����
	///</func_info>

	///<func_info>
	//����: ����Ŀ¼, ֧�ֶ༶Ŀ¼�Ĵ���
	//����: wchar_t* strDir Ҫ������Ŀ¼
	static void CreateDir(wchar_t* strDir);
	///</func_info>

	///<func_info>
	//����: �ж�Ŀ¼�Ƿ����
	//����: wchar_t* strDir Ҫ�жϵ�Ŀ¼
	//����ֵ:true��ʾ����, false��ʾ������
	static bool DirExist(wchar_t* strDir);
	///</func_info>

	///<func_info>
	//����: �ж�Ŀ¼�Ƿ����
	//����: wchar_t* strDir Ҫ�жϵ�Ŀ¼
	//����ֵ:true��ʾ����, false��ʾ������
	static bool DelDir(wchar_t* strDir);
	///</func_info>

	///<func_info>
	//����: �ж��ļ��Ƿ����
	//����: wchar_t* strDir Ҫ�жϵ�Ŀ¼
	//����ֵ:true��ʾ����, false��ʾ������
	static bool FileExist(wchar_t* strFile);
	///</func_info>

	///<func_info>
	//����: �ж�Ŀ¼�Ƿ����
	//����: wchar_t* strDir Ҫ�жϵ�Ŀ¼
	//����ֵ:true��ʾ����, false��ʾ������
	static bool DelFile(wchar_t* strDir);
	///</func_info>

	///<func_info>
	//����: Ŀ¼�е��ļ�����������1����
	//����: wchar_t* strDir ָ����Ŀ¼��
	//      wchar_t* strExt ָ���ĺ�׺, ��ʽ��"exe",����".exe"
	//����ֵ:�ļ�����Ŀ
	static int  FileNumInDir(wchar_t* strDir,  wchar_t* strExt);
	///</func_info>

	///<func_info>
	//����: �����ļ�����
	//����: wchar_t* strSourceFile ָ����Դ�ļ�
	//     wchar_t* strDestFile ָ����Ŀ���ļ�
	//����ֵ:true ��ʾ�ɹ�, false��ʾʧ��
	static bool  CopyModuleFile(wchar_t* strSourceFile, wchar_t* strDestFile); 
	///</func_info>

	///<func_info>
	//����: ��ȡ����ߴ�
	//����: int& iWidth   ����Ŀ��
	//      int&iHeight   ����ĸ߶�
	static void  GetDesktopSize(int& iWidth, int&iHeight); 
	///</func_info>

	///<func_info>
	//����: ����ָ��Ŀ¼�µ��ļ�(����һ��), ��Ҫ�����������ļ�����С��tagFileNameSet::MAX_ITEM_NUM
	//����: wchar_t* strDir ָ����Ŀ¼
	//      wchar_t* strExt ָ�����ļ�����׺
	//      FileNameSet& Set �������ļ�������
	//����ֵ:true ��ʾ�ɹ�, false��ʾʧ��
	static bool SearchFile(wchar_t* strDir, wchar_t* strExt,FileNameSet& Set); 
	///</func_info>

	///<func_info>
	//����: ����ָ��Ŀ¼�µ��ļ�(����һ��), ���pFileItems = NULL, �򷵻���������ͼ������, ���ó�����Ҫ����pFileItems
	//����: wchar_t* strDir ָ����Ŀ¼
	//      wchar_t* strExt ָ�����ļ�����׺,����Ҫ����.. 
	//      FileItemName* pFileItems �������ļ�������
	//		 int& iFindItemNum ���������ļ�������
	//����ֵ:true ��ʾ�ɹ�, false��ʾʧ��
    static bool SearchFile(wchar_t* strDir, wchar_t* strExt,FileItemName* pFileItems,  int& iFindItemNum);
	///</func_info>

	///<func_info>
	//����: ����ָ��Ŀ¼�µ���Ŀ¼(����һ��), ��Ҫ����������Ŀ¼����С��tagFileNameSet::MAX_ITEM_NUM
	//����: wchar_t* strDir ָ����Ŀ¼
	//      FileNameSet& Set��������Ŀ¼����
	//����ֵ:true ��ʾ�ɹ�, false��ʾʧ��
	static bool SearchDir(wchar_t* strDir, FileNameSet& Set); 
	///</func_info>

	///<func_info>
	//����: ����ָ��Ŀ¼�µ���Ŀ¼(����һ��), ��������Ŀ¼����û������
	//����: wchar_t* strDir ָ����Ŀ¼
	//      FileNameSet& Set��������Ŀ¼����
	//����ֵ:true ��ʾ�ɹ�, false��ʾʧ��
	static bool SearchDir(wchar_t* strDir, FileItemName* pFileItems,  int& iFindItemNum); 
	///</func_info>

	
	///<func_info>
	//����: ���ֽڵ����ֽڵ�ת��
	//����: char* strMutiByte ��Ҫת���Ķ��ֽڻ���
	//      wchar_t* strWideChar ת����Ŀ��ֽڻ���
	//      int strWideCharLen ���ֽڻ����������ߴ�
	static void  AnsiToUnicode(char* strMutiByte, wchar_t* strWideChar, int strWideCharLen);
	///</func_info>
	
	///<func_info>
	//����: ���ֽڵ����ֽڵ�ת��
	//����: wchar_t* strWideChar ��Ҫת���Ŀ��ֽڻ���
	//      char* strMutiByte ת����Ķ��ֽڻ���
	//      int strMutiByteLen ���ֽڻ����������ߴ�
	static void  UnicodeToAnsi(wchar_t* strWideChar, char* strMutiByte, int strMutiByteLen);
	///</func_info>

	///<func_info>
	//����: ��ȫ�Ŀ��ַ�����ʽ������
	//����: wchar_t* strDest ��Ҫ��ʽ���Ŀ��ֽڻ���
	//      size_t iStrLen  ���ֽڻ�������ߴ�(���ַ��ĸ���Ϊ��λ, �����ֽڴ�С,����STR_LEN���ȡ)
	//      int strMutiByteLen ���ֽڻ����������ߴ�
	static void  SafeWStringPrintf(wchar_t* strDest, size_t iStrLen, wchar_t* strFormat, ...);
	///</func_info>

	///<func_info>
	//����: ��ȫ�Ŀ��ַ�����������
	//����: wchar_t* strDest ��Ҫ������Ŀ�Ŀ��ֽڻ���
	//      size_t iStrLen  ���ֽڻ�������ߴ�(���ַ��ĸ���Ϊ��λ, �����ֽڴ�С,����STR_LEN���ȡ)
	//      wchar_t* strSource ��Ҫ������Դ���ֽڻ���
	static void  SafeWStringCpy(wchar_t* strDest, size_t iStrLen, wchar_t* strSource);
	///</func_info>

	///<func_info>
	//����: ��ȡ��ǰʱ��, ����ʽ��Ϊ�ַ���
	//����: wchar_t*  strNowTime ��ǰʱ����ַ���
	//      int iDestLen  ʱ���ַ��������ߴ�(���ַ��ĸ���Ϊ��λ, �����ֽڴ�С,����STR_LEN���ȡ)
	//     bool bHigh �Ƿ�֧�ָ߾��ȵ�ʱ����Ϣ, true��ʾ֧��
	static void  GetNowTime(wchar_t*  strNowTime,  int iDestLen, bool bHigh = false);
	///</func_info>

	///<func_info>
	//����: ��ȡ��ǰʱ��, ����ʽ��Ϊ�ַ���
	//����: wchar_t*  strNowTime ��ǰʱ����ַ���
	//      int iDestLen  ʱ���ַ��������ߴ�(���ַ��ĸ���Ϊ��λ, �����ֽڴ�С,����STR_LEN���ȡ)
	static void  GetNowDate(wchar_t*  strNowTime,  int iDestLen);
	///</func_info>

	///<func_info>
	//����: ��ȡ�ļ������ʱ����Ϣ
	//����:wchar_t* strFileName ָ��������·���ļ���
	//	SYSTEMTIME& tmCreate �ļ��Ĵ���ʱ��
	//   SYSTEMTIME& tmAccess �ļ��ķ���ʱ��
	//	SYSTEMTIME&  tmWrite �ļ����޸�ʱ��
	static bool   GetFileTimes(wchar_t* strFileName, 
		SYSTEMTIME& tmCreate, SYSTEMTIME& tmAccess, SYSTEMTIME&  tmWrite);
	///</func_info>

	///<func_info>
	//����: ��ȡ��ǰʱ��, ����ʽ��Ϊ�ַ���
	//����: wchar_t*  strNowTime ��ǰʱ����ַ���
	//      int iDestLen  ʱ���ַ��������ߴ�(���ַ��ĸ���Ϊ��λ, �����ֽڴ�С,����STR_LEN���ȡ)
	//     bool bHigh �Ƿ�֧�ָ߾��ȵ�ʱ����Ϣ, true��ʾ֧��
	static bool  GetFileTime(wchar_t*  filePathStr,
		wchar_t*  createTimeStr, int createTimeLen,
		wchar_t*  accessTimeStr, int accessTimeen,
		wchar_t*  writeTimeStr, int writeTimeLen,
		bool bHigh = false);
	///</func_info>

	///<func_info>
	//����: ����ָ��λ�õĳ���
	//����:
	//	wchar_t* strAppName ��Ҫ���еĳ���, ���Դ��������в���
	static bool  RunApp(wchar_t* strAppName);
	///</func_info>

	///<func_info>
	//����: ������ݵ�ָ���ı���
	//����:
	//	wchar_t* strAppName ��Ҫ���еĳ���, ���Դ��������в���
	static bool  WriteFloatToText(wchar_t* strFileName, double* pData, int iDatalen, int iColNum = 20);
	///</func_info>

	///<func_info>
	//����: ������ݵ�ָ���ı���
	//����:
	//	wchar_t* strAppName ��Ҫ���еĳ���, ���Դ��������в���
	static bool  WriteIntToText(wchar_t* strFileName, int* pData, int iDatalen, int iColNum = 20);
	///</func_info>
	///<func_info>
	//����: ������ݵ�ָ���ı���
	//����:
	//	wchar_t* strAppName ��Ҫ���еĳ���, ���Դ��������в���
	static bool  WriteBufferToText(wchar_t* strFileName, unsigned char* pData, int iDatalen, int iColNum = 20);
	///</func_info>

	///<func_info>
	//����: ������ݵ�ָ���ı���
	//����:
	//	wchar_t* strAppName ��Ҫ���еĳ���, ���Դ��������в���
	static bool  WriteWStrToText(wchar_t* strFileName, wchar_t* pData, int strLen);
	///</func_info>

	///<func_info>
	//����: ���ַ���{1,1,1}ת��Ϊdouble������
	//����:���ַ��ĸ���Ϊ��λ, �����ֽڴ�С,����STR_LEN���ȡ
	//	wchar_t* strAppName ��Ҫ���еĳ���, ���Դ��������в���
	static bool  StringToDoubles(wchar_t* srcString,  int strLen,double* pData, int& dataLen);
	///</func_info>

	///<func_info>
	//����: ���ַ���{1,1,1}ת��Ϊint������
	//����:���ַ��ĸ���Ϊ��λ, �����ֽڴ�С,����STR_LEN���ȡ
	//	wchar_t* strAppName ��Ҫ���еĳ���, ���Դ��������в���
	static bool  StringToInts(wchar_t* srcString,  int strLen,int* pData, int& dataLen);
	///</func_info>

	///<func_info>
	//����: �����ַ�������
	//����:���ַ��ĸ���Ϊ��λ, �����ֽڴ�С,����STR_LEN���ȡ
	//	wchar_t* strAppName ��Ҫ���еĳ���, ���Դ��������в���
	static long GetStringLen(wchar_t* srcString, int strMaxLen);
	///</func_info>

	///<func_info>
	//����: ��ȡ�ַ����еĲ�������
	//����:���ַ��ĸ���Ϊ��λ, �����ֽڴ�С,����STR_LEN���ȡ
	//	wchar_t* strAppName ��Ҫ���еĳ���, ���Դ��������в���
	static bool GetWStringIntPara(const wchar_t* srcString, int strMaxLen, const wchar_t* paraName,int& paraVal);
	static bool GetWStringDoublePara(const wchar_t* srcString, int strMaxLen, const wchar_t* paraName, double& paraVal);
	static bool GetStringIntPara(const char* srcString, int strMaxLen, const char* paraName, int& paraVal);
	static bool GetStringDoublePara(const char* srcString, int strMaxLen, const char* paraName, double& paraVal);
	///</func_info>
private:
	CCommonFunc(void);
	~CCommonFunc(void);
};
//==============================================================================
///</class_info>