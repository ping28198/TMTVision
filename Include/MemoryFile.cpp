#include "stdafx.h"
#include "MemoryFile.h"
#include "CommonFunc.h"
#include <windows.h>
#include <string.h>
#include <StrSafe.h>
#include <locale.h>
#ifndef STR_LEN
#define STR_LEN(x)  (sizeof(x)/sizeof(x[0]))
#endif





CMemoryFile::CMemoryFile()
{
	pFile = NULL;
	mode = 0;
}

bool CMemoryFile::OpenFile_R(wchar_t *pFileName)
{
	CCommonFunc::SafeStringPrintf(strFilePath, _countof(strFilePath), L"%s", pFileName);
	if (pFile)
	{
		fclose(pFile);
	}
	_wfopen_s(&pFile, strFilePath, L"rb");
	if (!pFile) return false;
	mode = 1;
	return true;
}

bool CMemoryFile::OpenFile_W(wchar_t *pFileName)
{
	CCommonFunc::SafeStringPrintf(strFilePath, _countof(strFilePath), L"%s", pFileName);
	if (pFile)
	{
		fclose(pFile);
	}
	_wfopen_s(&pFile, strFilePath, L"wb");
	if (!pFile) return false;
	mode = 2;
	return true;
}

bool CMemoryFile::OpenFile_AW(wchar_t *pFileName)
{
	CCommonFunc::SafeStringPrintf(strFilePath, _countof(strFilePath), L"%s", pFileName);
	if (pFile)
	{
		fclose(pFile);
	}
	_wfopen_s(&pFile, strFilePath, L"ab+");
	if (!pFile) return false;
	mode = 3;
	return true;
}

bool CMemoryFile::WriteMemoryToFile(const void *pMem, size_t mLength)
{
	if (mode != 2 && mode != 3) return false;
	int backcode=fwrite(pMem, mLength, 1, pFile);
	return backcode;
}

bool CMemoryFile::ReadMemoryFromFile(void *pMem, size_t mLength)
{
	if (mode != 1 && mode != 3) return false;
	int backcode = fread_s(pMem, mLength, mLength, 1, pFile);
	return backcode;
}

bool CMemoryFile::CloseFile()
{
	fflush(pFile);
	if (pFile)
	{
		fclose(pFile);
	}
	return true;
}

CMemoryFile::~CMemoryFile()
{
	CloseFile();
}
