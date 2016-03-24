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
}

bool CMemoryFile::OpenFile(wchar_t *pFileName)
{
	CCommonFunc::SafeStringPrintf(strFilePath, _countof(strFilePath), L"%s", pFileName);




}

CMemoryFile::~CMemoryFile()
{

}
