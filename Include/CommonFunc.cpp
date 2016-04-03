#include "stdafx.h"
#include "CommonFunc.h"
#include <windows.h>
#include <string.h>
#include <StrSafe.h>
#include <locale.h>
#ifndef STR_LEN
#define STR_LEN(x)  (sizeof(x)/sizeof(x[0]))
#endif

CCommonFunc::CCommonFunc(void)
{
	
}

CCommonFunc::~CCommonFunc(void)
{
}

//////////////////////////////////////////////////
//定义的公共函数
//------------------------------------------------------------
//描述: 获取完整的程序名描述
void CCommonFunc::GetAppPathName(wchar_t* strAppPath , size_t strLen)
{
	::GetModuleFileNameW(NULL, strAppPath, strLen);
}

//------------------------------------------------------------
//描述: 获取程序所在路径
void CCommonFunc::GetAppPath(wchar_t* strAppPath, size_t strLen)
{
	wchar_t strTempAppPath[256] = L"\0";
	memset(strTempAppPath, 0, sizeof(strTempAppPath));
	::GetModuleFileNameW(NULL, strTempAppPath, _countof(strTempAppPath));

	wchar_t* pChar = strTempAppPath;
	while(*pChar != L'\0')
	{
		pChar++;
	}
    while(*pChar != L'\\')
	{
		pChar--;
	}
	*pChar = L'\0';
	StringCchCopyW(strAppPath, strLen, strTempAppPath);
}
//------------------------------------------------------------
//描述: 
void CCommonFunc::GetModulePath(void* hModuel, wchar_t* strPath , size_t strLen)
{
	wchar_t strTempAppPath[256] = L"\0";
	memset(strTempAppPath, 0, sizeof(strTempAppPath));
	::GetModuleFileNameW((HMODULE)hModuel, strTempAppPath, _countof(strTempAppPath));

	wchar_t* pChar = strTempAppPath;
	while(*pChar != L'\0')
	{
		pChar++;
	}
    while(*pChar != L'\\')
	{
		pChar--;
	}
	*pChar = L'\0';
	StringCchCopyW(strPath, strLen, strTempAppPath);

}
//------------------------------------------------------------
//描述: 
void CCommonFunc::GetAppName(wchar_t* strAppName, size_t iStrLen)
{
	wchar_t strTempAppPath[256] = L"\0";
	memset(strTempAppPath, 0, sizeof(strTempAppPath));
	::GetModuleFileNameW(NULL, strTempAppPath, sizeof(strTempAppPath));

	wchar_t* pChar = strTempAppPath;
	while(*pChar != L'\0')
	{
		pChar++;
	}
	while(*pChar != L'.')
	{
		pChar--;
	}
	*pChar = L'\0';
    while(*pChar != L'\\')
	{
		pChar--;
	}
	pChar++;
	StringCchCopyW(strAppName,iStrLen, pChar);
}
//------------------------------------------------------------
//描述: 
void CCommonFunc::GetFilePath(wchar_t* strPathName, wchar_t* strPath, size_t iStrPathLen)
{
	wchar_t strTempAppPath[256] = L"\0";
	SafeStringPrintf(strTempAppPath, _countof(strTempAppPath),  L"%s", strPathName);
	wchar_t* pChar = strTempAppPath;
	while(*pChar != L'\0')
	{
		pChar++;
	}
    while(*pChar != L'\\')
	{
		pChar--;
	}
	*pChar = L'\0';
	StringCchCopyW(strPath, iStrPathLen, strTempAppPath);


}
//------------------------------------------------------------
//描述: 
void CCommonFunc::GetFileName(wchar_t* strPathName, wchar_t* strFileName, size_t iStrFileNameLen)
{
	if(wcslen(strPathName) == 0) return;
	wchar_t strTempPathName[256] = L"\0";
	//wsprintfW(strTempPathName, L"%s", strPathName);
	SafeStringPrintf(strTempPathName, _countof(strTempPathName),  L"%s", strPathName);
	wchar_t* pChar = strTempPathName;
	wchar_t* pStartChar = strTempPathName;

	while(*pChar != L'\0')
	{
		pChar++;
	}
    while(*pChar != L'\\')
	{
		if(pChar >= pStartChar)
		{
			pChar--;
		}else
		{
			break;
		}
	}
	pChar++;
	StringCchCopyW(strFileName,iStrFileNameLen, pChar);
}
//------------------------------------------------------------
//描述: 
void CCommonFunc::GetFileExt(wchar_t* strPathName, wchar_t* strExt, size_t iStrExtLen)
{
	wchar_t strTempPathName[256] = L"\0";
	SafeStringPrintf(strTempPathName, _countof(strTempPathName),  L"%s", strPathName);
	//wsprintfW(strTempPathName, L"%s", strPathName);
	wchar_t* pChar = strTempPathName;
	while(*pChar != L'\0')
	{
		pChar++;
	}
    while(*pChar != L'.')
	{
		pChar--;
	}
	pChar++;
	StringCchCopyW(strExt, iStrExtLen, pChar);
}
//------------------------------------------------------------
//描述: 
void CCommonFunc::GetFileNoExt(wchar_t* strPathName, wchar_t* strName, int iNameLen)
{
	wchar_t strTempPathName[256] = L"\0";
	SafeStringPrintf(strTempPathName, _countof(strTempPathName),  L"%s", strPathName);
	wchar_t* pChar = strTempPathName;
	while(*pChar != L'.')
	{
		pChar++;
	}
	*pChar = L'\0';
	StringCchCopyW(strName, iNameLen, strTempPathName);
}

//------------------------------------------------------------
//描述:创建多极目录
void CCommonFunc::CreateDir(wchar_t* strDir)
{
	if(DirExist(strDir))
	{
		return;
	}	
	
	wchar_t strTemp[256];
	//首先确定有多少级目录

	wchar_t* pChar = strDir;
	int iPos[20]; //最多允许创建20级目录
	int iLayNum = 0; //目录的级数
	int iCurrPos = 0;
	while(*pChar != L'\0')
	{
		if(*pChar == L'\\')
		{
			iPos[iLayNum++] = iCurrPos;
		}
		pChar++;
		iCurrPos++;
	}
	iPos[iLayNum++] = iCurrPos;
	//创建目录	
	for(int i = 1; i < iLayNum; i++)
	{
		wcscpy_s(strTemp, _countof(strTemp), strDir);
		strTemp[iPos[i]] = L'\0';
		//判断目录是否存在
		HANDLE hDir = ::CreateFileW(strTemp, 
								GENERIC_READ, 
								FILE_SHARE_READ, 
								NULL,
								OPEN_EXISTING,
								FILE_FLAG_BACKUP_SEMANTICS,
								NULL);
		if(INVALID_HANDLE_VALUE == hDir)
		{
			::CreateDirectoryW(strTemp,NULL);
		}
		::CloseHandle(hDir);
	}

}

//------------------------------------------------------------
//描述:判断目录是否存在,true表示存在
bool CCommonFunc::DirExist(wchar_t* strDir)
{
	HANDLE hDir = ::CreateFileW(strDir, 
								GENERIC_READ, 
								FILE_SHARE_READ, 
								NULL,
								OPEN_EXISTING,
								FILE_FLAG_BACKUP_SEMANTICS,
								NULL);
	if(INVALID_HANDLE_VALUE == hDir)
	{
		return false;
	}
	::CloseHandle(hDir);
	return true;
}

//------------------------------------------------------------
//描述:递归删除非空目录
bool CCommonFunc::DelDir(wchar_t* strDir)
{
	int b = 0;
	HANDLE hDir = ::CreateFileW(strDir,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);
	if (INVALID_HANDLE_VALUE == hDir)
	{
		return true;
	}
	else
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		wchar_t strSearchPath[256];
		SafeStringPrintf(strSearchPath, _countof(strSearchPath), L"%s*.*", strDir);
		hFind = ::FindFirstFileW(strSearchPath, (LPWIN32_FIND_DATAW)&FindFileData);
		do
		{ 
			if (hFind != INVALID_HANDLE_VALUE)
			{
				if ((FindFileData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
				{
					if (wcscmp(FindFileData.cFileName, L".") == 0 || wcscmp(FindFileData.cFileName, L"..") == 0)
					{
						if (FindNextFile(hFind, &FindFileData))  continue;
						else break;
					}
					WIN32_FIND_DATA SubFindFileData;
					HANDLE hSubFind;
					wchar_t strSubSearchPath[256];
					SafeStringPrintf(strSubSearchPath, _countof(strSubSearchPath), L"%s%s\\*.*", strDir, FindFileData.cFileName);
					hSubFind = ::FindFirstFileW(strSubSearchPath, (LPWIN32_FIND_DATAW)&SubFindFileData);
					SafeStringPrintf(strSubSearchPath, _countof(strSubSearchPath), L"%s%s\\", strDir, FindFileData.cFileName);
					//if (hSubFind == INVALID_HANDLE_VALUE)
					//{
					//	//::RemoveDirectory(strSubSearchPath);
					//}
					//else 
					DelDir(strSubSearchPath);
					::FindClose(hSubFind);
				}
				else
				{
					SafeStringPrintf(strSearchPath, _countof(strSearchPath), L"%s%s", strDir, FindFileData.cFileName);
					DelFile(strSearchPath);
				}
			}
			else
			{
				break;
			}
		} while (FindNextFile(hFind, &FindFileData));
		::FindClose(hFind);
		::CloseHandle(hDir);
		SafeStringPrintf(strSearchPath, _countof(strSearchPath), L"%s", strDir);
		::RemoveDirectory(strSearchPath);
		b = ::GetLastError();
	}
	return true;
}

//------------------------------------------------------------
//描述:判断文件是否存在,true表示存在
bool CCommonFunc::FileExist(wchar_t* strFile)
{
	HANDLE hFile = ::CreateFileW(strFile, 
								GENERIC_READ, 
								FILE_SHARE_READ, 
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_READONLY,
								NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{
		return false;
	}
	::CloseHandle(hFile);
	return true;
}

//------------------------------------------------------------
//描述:判断文件是否存在,存在则删除
bool CCommonFunc::DelFile(wchar_t* strFile)
{
	//wchar_t strFile[260];
	HANDLE hFile = ::CreateFileW(strFile,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_READONLY,
		NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return true;
	}
	else
	{
		::CloseHandle(hFile);
		::DeleteFile(strFile);
	}
	//::CloseHandle(hFile);
	return true;
}

//------------------------------------------------------------
//描述:进行文件复制
bool  CCommonFunc::CopyModuleFile(wchar_t* strSourceFile, wchar_t* strDestFile)
{
	return (::CopyFileW(strDestFile, strSourceFile, FALSE) == TRUE);
}
//------------------------------------------------------------
//描述:获取桌面尺寸
void  CCommonFunc::GetDesktopSize(int& iWidth, int&iHeight)
{
	HWND hDeskTop = ::GetDesktopWindow();
	RECT rcSize;
	::GetWindowRect(hDeskTop, &rcSize);
	iWidth = rcSize.right - rcSize.left;
	iHeight = rcSize.bottom - rcSize.top;

}


//------------------------------------------------------------
//描述:目录中的文件数
int  CCommonFunc::FileNumInDir(wchar_t* strDir, wchar_t* strExt)
{
	WIN32_FIND_DATA FindFileData;
	int iFileNum = 0;
	HANDLE hFind;
	wchar_t strSearchPath[256];
	SafeStringPrintf(strSearchPath, _countof(strSearchPath),  L"%s\\*.*", strDir); 
	hFind = ::FindFirstFileW(strSearchPath,(LPWIN32_FIND_DATAW)&FindFileData);
    wchar_t strUppExt[20];
	wcscpy_s(strUppExt,_countof(strUppExt), strExt);
	_wcsupr_s(strUppExt, _countof(strUppExt));
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if((FindFileData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
			{
				size_t iLength = wcslen((wchar_t*)FindFileData.cFileName);
				if(iLength > 1)
				{
					int lastLen = 0;
					wchar_t* pEndChar = (wchar_t*)FindFileData.cFileName + iLength;
					while(*pEndChar != L'.')
					{
						pEndChar--;
						lastLen++;
					}
					pEndChar++;
					 _wcsupr_s(pEndChar, lastLen);
					if(wcscmp(pEndChar, strUppExt) == 0)
					{
						iFileNum++;
					}
				}
			}
		}
		while(FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}

	return iFileNum;
}
//------------------------------------------------------------
//描述:搜索指定后缀的文件, 仅在一级目录中
#ifdef  UNICODE
bool  CCommonFunc::SearchFile(wchar_t* strDir, wchar_t* strExt,FileNameSet& Set)
{
	Set.iItemNum = 0;
	WIN32_FIND_DATA FindFileData;
	//int iFileNum = 0;
	HANDLE hFind;
	wchar_t strSearchPath[256];
	SafeStringPrintf(strSearchPath,  _countof(strSearchPath), L"%s\\*.*", strDir); 
	hFind = ::FindFirstFileW(strSearchPath,(LPWIN32_FIND_DATAW)&FindFileData);
    wchar_t strUppExt[20];
	wcscpy_s(strUppExt,_countof(strUppExt), strExt);
	_wcsupr_s(strUppExt, _countof(strUppExt));
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if((FindFileData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
			{
				size_t iLength = wcslen((wchar_t*)FindFileData.cFileName);

				//wsprintfW(Set.Items[Set.iItemNum], FindFileData.cFileName);
				StringCchCopyW(Set.Items[Set.iItemNum], _countof(Set.Items[Set.iItemNum]), FindFileData.cFileName);
				if(iLength > 1)
				{
					int lastLen = 0;
					wchar_t* pEndChar = (wchar_t*)FindFileData.cFileName + iLength;
					while(*pEndChar != L'.')
					{
						pEndChar--;
						lastLen++;
					}
					pEndChar++;
					_wcsupr_s(pEndChar, lastLen);
					 if(wcscmp(pEndChar, strUppExt) == 0)
					{
						Set.iItemNum++;
						if(Set.iItemNum >= Set.MAX_ITEM_NUM)
						{
							break;
						}
					}
				}
			}
		}
		while(FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}

	return (Set.iItemNum != 0);
}

//------------------------------------------------------------
//描述:搜索指定目录下的子目录(仅限一级)
bool CCommonFunc::SearchDir(wchar_t* strDir, FileNameSet& Set)
{
	Set.iItemNum = 0;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	wchar_t strSearchPath[256];
	SafeStringPrintf(strSearchPath, _countof(strSearchPath), L"%s\\*.*", strDir); 
	hFind = ::FindFirstFileW(strSearchPath,(LPWIN32_FIND_DATAW)&FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if((FindFileData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
			{
				size_t iLength = wcslen((wchar_t*)FindFileData.cFileName);
				if(wcscmp(FindFileData.cFileName, L".") == 0 || wcscmp(FindFileData.cFileName, L"..") == 0) continue;

				StringCchCopyW(Set.Items[Set.iItemNum], _countof(Set.Items[Set.iItemNum]), FindFileData.cFileName);
				Set.iItemNum++;
				if(Set.iItemNum >= Set.MAX_ITEM_NUM)
				{
						break;
				}
			}
		}
		while(FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}

	return (Set.iItemNum != 0);

}
//------------------------------------------------------------
//描述:搜索指定目录下的子目录(仅限一级)
bool CCommonFunc::SearchDir(wchar_t* strDir, FileItemName* pFileItems,  int& iFindItemNum)
{
	iFindItemNum = 0;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	wchar_t strSearchPath[256];
	SafeStringPrintf(strSearchPath, _countof(strSearchPath), L"%s\\*.*", strDir); 
	hFind = ::FindFirstFileW(strSearchPath,(LPWIN32_FIND_DATAW)&FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if((FindFileData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
			{
				size_t iLength = wcslen((wchar_t*)FindFileData.cFileName);
				if(wcscmp(FindFileData.cFileName, L".") == 0 || wcscmp(FindFileData.cFileName, L"..") == 0) continue;

				if(pFileItems)
				{
					StringCchCopyW(pFileItems[iFindItemNum], _countof(pFileItems[iFindItemNum]), FindFileData.cFileName);
				}
				iFindItemNum++;
				
			}
		}
		while(FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}

	return (iFindItemNum > 0);

}

//------------------------------------------------------------
//描述:
bool CCommonFunc::SearchFile(wchar_t* strDir, wchar_t* strExt, FileItemName* pFileItems,  int& iFindItemNum)
{	
	iFindItemNum = 0;
	WIN32_FIND_DATA FindFileData;
	//int iFileNum = 0;
	HANDLE hFind;
	wchar_t strSearchPath[256];
	SafeStringPrintf(strSearchPath, _countof(strSearchPath), L"%s\\*.*", strDir); 
	hFind = ::FindFirstFileW(strSearchPath,(LPWIN32_FIND_DATAW)&FindFileData);
    wchar_t strUppExt[20];
	wcscpy_s(strUppExt,_countof(strUppExt), strExt);
	_wcsupr_s(strUppExt, _countof(strUppExt));
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if((FindFileData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
			{
				size_t iLength = wcslen((wchar_t*)FindFileData.cFileName);
				
				if(iLength > 1)
				{
					int lastLen = 0;
					wchar_t* pEndChar = (wchar_t*)FindFileData.cFileName + iLength;
					while(*pEndChar != L'.')
					{
						pEndChar--;
						lastLen++;
					}
					pEndChar++;
					_wcsupr_s(pEndChar, lastLen);
					if(wcscmp(pEndChar, strUppExt) == 0)
					{
						if(pFileItems)
						{
							SafeStringPrintf(pFileItems[iFindItemNum], STR_LEN(pFileItems[iFindItemNum]),L"%s", FindFileData.cFileName);
						}						
						iFindItemNum++;
					}
				}
			}
		}
		while(FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}

	return (iFindItemNum != 0);
}
#endif
//------------------------------------------------------------
//描述:
void   CCommonFunc::AnsiToUnicode(char* strMutiByte, wchar_t* strWideChar, int strWideCharLen)
{
	int iLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strMutiByte, strlen(strMutiByte),
		                strWideChar,strWideCharLen);

	strWideChar[iLen] = L'\0';

}

//------------------------------------------------------------
//描述:
void  CCommonFunc::UnicodeToAnsi(wchar_t* strWideChar, char* strMutiByte, int strMutiByteLen)
{
	 WideCharToMultiByte(CP_OEMCP, 0, strWideChar, -1, strMutiByte, strMutiByteLen, NULL, 0);
}
//------------------------------------------------------------
//描述:安全的字符串格式化函数
void  CCommonFunc::SafeStringPrintf(wchar_t* strDest, size_t iStrLen, wchar_t* strFormat, ...)
{
	va_list argList;
	va_start(argList, strFormat);
	StringCchVPrintfW(strDest, iStrLen, strFormat, argList);
	va_end(argList);
}
//------------------------------------------------------------
//描述:
void  CCommonFunc::SafeStringCpy(wchar_t* strDest, size_t iStrLen, wchar_t* strSource)
{
	StringCchCopyW(strDest, iStrLen, strSource);
}


//------------------------------------------------------------
//描述:2012.4.13王磊修改分隔符:改为. -改为_以方便应用到文件名中
void  CCommonFunc::GetNowTime(wchar_t*  strNowTime, int iDestLen, bool bHigh)
{
	SYSTEMTIME NowTime;
	GetLocalTime(&NowTime);
	if(bHigh)
	{


		SafeStringPrintf(strNowTime, iDestLen,  
			L"%d_%d_%d %d.%d.%d.%d", 
			NowTime.wYear, 
			NowTime.wMonth, 
			NowTime.wDay, 
			NowTime.wHour, 
			NowTime.wMinute,
			NowTime.wSecond, 
			NowTime.wMilliseconds);

	}else
	{

		SafeStringPrintf(strNowTime, iDestLen,
			L"%d_%d_%d %d.%d.%d", 
			NowTime.wYear, 
			NowTime.wMonth, 
			NowTime.wDay, 
			NowTime.wHour, 
			NowTime.wMinute,
			NowTime.wSecond);

	}
}
//------------------------------------------------------------
//描述:2012.4.13王磊 以方便应用到文件名中
void  CCommonFunc::GetNowDate(wchar_t*  strNowTime, int iDestLen)
{
	SYSTEMTIME NowTime;
	GetLocalTime(&NowTime);
	{

		SafeStringPrintf(strNowTime, iDestLen,
			L"%d%02d%02d", 
			NowTime.wYear, 
			NowTime.wMonth, 
			NowTime.wDay);

	}
}

//------------------------------------------------------------
//描述:
//bool   CCommonFunc::GetFileTimes(wchar_t* strFileName, SYSTEMTIME& tmCreate, SYSTEMTIME& tmAccess, SYSTEMTIME&  tmWrite)
//{
//	HANDLE hFile = ::CreateFile(strFileName, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//	if(INVALID_HANDLE_VALUE == hFile) return false;
//	FILETIME ftCreate, ftAccess, ftWrite;
//	::GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite);
//
//	FILETIME ftLocalCreate, ftLocalAccess, ftLocalWrite;
//	::FileTimeToLocalFileTime(&ftCreate, &ftLocalCreate);
//	::FileTimeToLocalFileTime(&ftAccess, &ftLocalAccess);
//	::FileTimeToLocalFileTime(&ftWrite, &ftLocalWrite);
//
//
//	::FileTimeToSystemTime(&ftLocalCreate, &tmCreate);
//	::FileTimeToSystemTime(&ftLocalAccess, &tmAccess);
//	::FileTimeToSystemTime(&ftLocalWrite, &tmWrite);
//
//	::CloseHandle(hFile);
//	return true;
//}
//------------------------------------------------------------
//描述:
bool   CCommonFunc::RunApp(wchar_t* strAppName)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi));

	if(CreateProcess(NULL, strAppName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL,&si, &pi))
	{
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
		return true;
	}else
	{
		return false;
	}
}

//------------------------------------------------------------
//描述:
bool  CCommonFunc::WriteFloatToText(wchar_t* strFileName, double* pData, int iDatalen, int iColNum)
{
	setlocale(LC_ALL, "chs"); 
	wchar_t  strFilePath[256];
	SafeStringPrintf(strFilePath,  _countof(strFilePath), L"%s", strFileName);
	FILE* pFile = NULL;
	_wfopen_s(&pFile, strFilePath, L"w+");
	if(!pFile)	return false;
	for(int i = 0; i < iDatalen; i++)
	{
		if( i > 0 && (i % iColNum == 0))
		{
			fwprintf(pFile, L"\n%4.1f", pData[i]);
		}else
		{
			fwprintf(pFile, L"%4.1f", pData[i]);	
		}
	}
	fclose(pFile);	
	return true;
}
//------------------------------------------------------------
//描述:输出数据到指定文本中
//参数:
bool  CCommonFunc::WriteIntToText(wchar_t* strFileName, int* pData, int iDatalen, int iColNum)
{
	setlocale(LC_ALL, "chs"); 
	wchar_t  strFilePath[256];
	SafeStringPrintf(strFilePath,  _countof(strFilePath), L"%s", strFileName);
	FILE* pFile = NULL;
	_wfopen_s(&pFile, strFilePath, L"w+");
	if(!pFile)	return false;
	for(int i = 0; i < iDatalen; i++)
	{

		if( i > 0 && (i % iColNum == 0))
		{
			fwprintf(pFile, L"\n%4d", pData[i]);
		}else
		{
			fwprintf(pFile, L"%4d ", pData[i]);	
		}
	}
	fclose(pFile);	
	return true;
}
//------------------------------------------------------------
//描述:
bool  CCommonFunc::WriteBufferToText(wchar_t* strFileName, unsigned char* pData, int iDatalen, int iColNum)
{
	setlocale(LC_ALL, "chs"); 
	wchar_t  strFilePath[256];
	SafeStringPrintf(strFilePath,  _countof(strFilePath), L"%s", strFileName);
	FILE* pFile = NULL;
	_wfopen_s(&pFile, strFilePath, L"w+");
	if(!pFile)	return false;
	for(int i = 0; i < iDatalen; i++)
	{

		if( i > 0 && (i % iColNum == 0))
		{
			fwprintf(pFile, L"\n%4x", pData[i]);
		}else
		{
			fwprintf(pFile, L"%4x", pData[i]);	
		}
	}
	fclose(pFile);	
	return true;
}
//------------------------------------------------------------
//描述:
bool  CCommonFunc::WriteWStrToText(wchar_t* strFileName, wchar_t* pData, int strLen)
{
	setlocale(LC_ALL, "chs"); 
	if(pData[strLen]!=0) pData[strLen]=0;//DeWarning 2016.01.23
	wchar_t  strFilePath[256];
	SafeStringPrintf(strFilePath,  _countof(strFilePath), L"%s", strFileName);
	FILE* pFile = NULL;
	_wfopen_s(&pFile, strFilePath, L"a+");
	if(!pFile)
	{
		return false;
	}

	{
		fwprintf(pFile, L"%s", pData);	
	}
	fclose(pFile);	
	return true;
}

//描述: 将字符串{1,1,1}转换为double型数组
//参数:
//	wchar_t* strAppName 需要运行的程序, 可以带有命令行参数
bool  CCommonFunc::StringToDoubles(wchar_t* srcString,  int strLen,double* pData, int& dataLen)
{
	wchar_t *pChar=srcString;
	wchar_t tmpWord[16]={0};
	int tmpWordNum=0;
	bool lastChar=false;
	double tmpDouble=0.0;
	dataLen=0;
	for (int i=0;i<strLen;i++)
	{
		if (*pChar==L'\0')
		{
			dataLen=0;
			return false;
		}
		if (*pChar==L'{'||*pChar==L' '||*pChar==L'\r'||*pChar==L'\n'||*pChar==L'}'||*pChar==L',')
		{
			if (lastChar&&tmpWord!=L"")
			{
				*(pData+dataLen)=_wtof(tmpWord);
				dataLen++;
				if (*pChar==L'}')
				{
					return true;
				}	
				pChar++;
				memset(tmpWord,0,sizeof(tmpWord));
				tmpWordNum=0;
				lastChar=false;
			}
			else if (*pChar==L'}')
			{
				return true;
			}
			else 
			{
				pChar++;
				memset(tmpWord,0,sizeof(tmpWord));
				tmpWordNum=0;
				lastChar=false;
			}
		} 
		else
		{
			tmpWord[tmpWordNum]=*pChar;
			pChar++;
			tmpWordNum++;
			lastChar=true;
		}
	}
	dataLen=0;
	return false;
}
///</func_info>
//描述: 将字符串{1,1,1}转换为int型数组
//参数:
//	wchar_t* strAppName 需要运行的程序, 可以带有命令行参数
bool  CCommonFunc::StringToInts(wchar_t* srcString,  int strLen,int* pData, int& dataLen)
{
	wchar_t *pChar=srcString;
	wchar_t tmpWord[16]={0};
	int tmpWordNum=0;
	bool lastChar=false;
	double tmpDouble=0.0;
	dataLen=0;
	for (int i=0;i<strLen;i++)
	{
		if (*pChar==L'\0')
		{
			dataLen=0;
			return false;
		}
		if (*pChar==L'{'||*pChar==L' '||*pChar==L'\r'||*pChar==L'\n'||*pChar==L'}'||*pChar==L',')
		{
			if (lastChar&&tmpWord!=L"")
			{
				*(pData+dataLen)=_wtoi(tmpWord);
				dataLen++;
				if (*pChar==L'}')
				{
					return true;
				}	
				pChar++;
				memset(tmpWord,0,sizeof(tmpWord));
				tmpWordNum=0;
				lastChar=false;
			}
			else if (*pChar==L'}')
			{
				return true;
			}
			else 
			{
				pChar++;
				memset(tmpWord,0,sizeof(tmpWord));
				tmpWordNum=0;
				lastChar=false;
			}
		} 
		else
		{
			tmpWord[tmpWordNum]=*pChar;
			pChar++;
			tmpWordNum++;
			lastChar=true;
		}
	}
	dataLen=0;
	return false;
}
///</func_info>