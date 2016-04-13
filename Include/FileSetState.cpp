//#include "stdafx.h"
#include "FileSetState.h"
#include "CommonFunc.h"

CString FileSetState::m_Path = L"";
CString FileSetState::m_Ext = L"";
long FileSetState::m_FileIndex = -1;
CArray<CString, CString&> FileSetState::m_NameArray{};

bool FileSetState::LoadDirectory(CString path, CString ext, int index)
{
	if (path == L"")//默认参数用于更新
	{
		path = m_Path;
		ext = m_Ext;
	}
	if (!CCommonFunc::DirExisit(path.GetBuffer()))
	{
		return false;
	}
	m_Path = path;
	if (m_Path.Right(1) != L"\\")
	{
		m_Path += L"\\";
	}
	m_Ext = ext;
	m_Ext.MakeLower();
	bool reVal = SearchFile(m_Path, m_Ext, m_NameArray);
	if (m_NameArray.GetCount()>0)
	{
		m_FileIndex = min(index,max(0, m_NameArray.GetCount()-1));
		if (index == -1)
		{
			m_FileIndex = m_NameArray.GetCount() - 1;
		}
	}
	else m_FileIndex = -1;
	return reVal;
}
int FileSetState::LoadDirectoryFile(CString srcFilePath, CString srcExt)
{
	CString srcFolderPath;
	wchar_t wcSrcFolderPath[260];
	CCommonFunc::GetFilePath(srcFilePath.GetBuffer(), wcSrcFolderPath, 260);
	srcFolderPath = wcSrcFolderPath;
	int num = LoadDirectory(srcFolderPath, srcExt);

	CString srcFileName;
	wchar_t wcSrcFileName[260];
	CCommonFunc::GetFileName(srcFilePath.GetBuffer(), wcSrcFileName, 260);
	srcFileName = wcSrcFileName;
	CString srcFileExt;

	if (m_NameArray.GetCount()>0)
	{
		for (int i = 0; i<m_NameArray.GetCount(); i++)
		{
			if (srcFileName == m_NameArray.GetAt(i))
			{
				m_FileIndex = i;
			}
		}
	}
	else m_FileIndex = -1;
	return num;
}
bool  FileSetState::SearchFile(CString path, CString ext, CArray<CString, CString&>& nameArray)
{
	nameArray.SetSize(0);
	if (path.Right(1) == L"\\")
	{
		path = path.Left(path.GetLength()-1);
	}
	FileNameSet  nameSet;
	nameSet.iItemNum = 0;
	CCommonFunc::SearchFile(path.GetBuffer(), ext.GetBuffer(), nameSet);
	CString tmpName = L"";

	for (int i = 0; i < nameSet.iItemNum; i++)
	{
		tmpName = nameSet.Items[i];
		nameArray.Add(tmpName);
	}
	return (nameArray.GetCount() != 0);
}
