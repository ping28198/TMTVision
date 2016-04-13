#pragma once
//#include "stdafx.h"
#include <afxtempl.h>

class FileSetState
{
public:

	static FileSetState& GetState()
	{
		static FileSetState obj;
		return obj;
	}
public:
	static CString m_Path;
	static CString m_Ext;
	static long m_FileIndex;
	static CArray<CString, CString&> m_NameArray;
	static bool LoadDirectory(CString path = L"", CString ext = L"*.bmp;*.jpg;*.jpeg;*.png;*.tif;*.tiff", int index = 0);
	static int LoadDirectoryFile(CString srcFilePath, CString srcExt = L"*.bmp;*.jpg;*.jpeg;*.png;*.tif;*.tiff");
	static bool SearchFile(CString path, CString ext, CArray<CString, CString&>& nameArray);
};