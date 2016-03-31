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
//����Ĺ�������
//------------------------------------------------------------
//��������ȡ�����ĳ���������
void CCommonFunc::GetAppPathName(wchar_t* strAppPath , size_t strLen)
{
	::GetModuleFileNameW(NULL, strAppPath, strLen);
}

//------------------------------------------------------------
//��������ȡ��������·��
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
//������
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
//������
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
//������
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
//������
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
//������
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
//������
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
//����:�����༫Ŀ¼
void CCommonFunc::CreateDir(wchar_t* strDir)
{
	if(DirExist(strDir))
	{
		return;
	}	
	
	wchar_t strTemp[256];
	//����ȷ���ж��ټ�Ŀ¼

	wchar_t* pChar = strDir;
	int iPos[20]; //�����������20��Ŀ¼
	int iLayNum = 0; //Ŀ¼�ļ���
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
	//����Ŀ¼	
	for(int i = 1; i < iLayNum; i++)
	{
		wcscpy_s(strTemp, _countof(strTemp), strDir);
		strTemp[iPos[i]] = L'\0';
		//�ж�Ŀ¼�Ƿ����
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
//����:�ж�Ŀ¼�Ƿ����,true��ʾ����
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
//����:�ݹ�ɾ���ǿ�Ŀ¼
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
//����:�ж��ļ��Ƿ����,true��ʾ����
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
//����:�ж��ļ��Ƿ����,������ɾ��
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
//����:�����ļ�����
bool  CCommonFunc::CopyModuleFile(wchar_t* strSourceFile, wchar_t* strDestFile)
{
	return (::CopyFileW(strDestFile, strSourceFile, FALSE) == TRUE);
}
//------------------------------------------------------------
//����:��ȡ����ߴ�
void  CCommonFunc::GetDesktopSize(int& iWidth, int&iHeight)
{
	HWND hDeskTop = ::GetDesktopWindow();
	RECT rcSize;
	::GetWindowRect(hDeskTop, &rcSize);
	iWidth = rcSize.right - rcSize.left;
	iHeight = rcSize.bottom - rcSize.top;

}


//------------------------------------------------------------
//����:Ŀ¼�е��ļ���
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
//����:����ָ����׺���ļ�������һ��Ŀ¼��
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
//����:����ָ��Ŀ¼�µ���Ŀ¼(����һ��)
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
//����:����ָ��Ŀ¼�µ���Ŀ¼(����һ��)
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
//����:
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
//����:
void   CCommonFunc::AnsiToUnicode(char* strMutiByte, wchar_t* strWideChar, int strWideCharLen)
{
	int iLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strMutiByte, strlen(strMutiByte),
		                strWideChar,strWideCharLen);

	strWideChar[iLen] = L'\0';

}

//------------------------------------------------------------
//����:
void  CCommonFunc::UnicodeToAnsi(wchar_t* strWideChar, char* strMutiByte, int strMutiByteLen)
{
	 WideCharToMultiByte(CP_OEMCP, 0, strWideChar, -1, strMutiByte, strMutiByteLen, NULL, 0);
}
//------------------------------------------------------------
//����:��ȫ���ַ�����ʽ������
void  CCommonFunc::SafeStringPrintf(wchar_t* strDest, size_t iStrLen, wchar_t* strFormat, ...)
{
	va_list argList;
	va_start(argList, strFormat);
	StringCchVPrintfW(strDest, iStrLen, strFormat, argList);
	va_end(argList);
}
//------------------------------------------------------------
//����:
void  CCommonFunc::SafeStringCpy(wchar_t* strDest, size_t iStrLen, wchar_t* strSource)
{
	StringCchCopyW(strDest, iStrLen, strSource);
}


//------------------------------------------------------------
//����:2012.4.13�����޸ķָ���:��Ϊ. -��Ϊ_�Է���Ӧ�õ��ļ�����
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
//����:2012.4.13���� �Է���Ӧ�õ��ļ�����
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
//����:
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
//����:
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
//����:
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
//����:������ݵ�ָ���ı���
//����:
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
//����:
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
//����:
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

//���������ַ���{1,1,1}ת��Ϊdouble������
//����:
//	wchar_t* strAppName ��Ҫ���еĳ��򣬿��Դ��������в���
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
//���������ַ���{1,1,1}ת��Ϊint������
//����:
//	wchar_t* strAppName ��Ҫ���еĳ��򣬿��Դ��������в���
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