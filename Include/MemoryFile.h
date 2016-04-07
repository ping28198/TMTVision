#pragma once

///<proj_info>
//==============================================================================
// 项目名 ：图像处理平台
// 文件名 ：MemoryFile.h
// 作  者 ：任威平
// 用  途 ：实现了操作类，将内容数据写入文件，和从文件读取数据加载到内存
// 版  权 ：TMT
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间      描述
//1.0     任威平      2016-03-25   创建模块

//==============================================================================
///</ver_info>








class CMemoryFile
{
public:
	CMemoryFile();
	~CMemoryFile();
	//////////////////////////////////////////////////////////////////////////
	//以只读方式打开
	bool OpenFile_R(wchar_t *pFileName);
	//////////////////////////////////////////////////////////////////////////
	//以覆盖写入方式打开
	bool OpenFile_W(wchar_t *pFileName);
	//////////////////////////////////////////////////////////////////////////
	//以追加写入方式打开
	bool OpenFile_AW(wchar_t *pFileName);
	//////////////////////////////////////////////////////////////////////////
	//将内存写入文件
	bool WriteMemoryToFile(const void *pMem,size_t mLength);
	//////////////////////////////////////////////////////////////////////////
	//从文件读取内存
	bool ReadMemoryFromFile(void *pMem, size_t mLength);

	bool CloseFile();
	//////////////////////////////////////////////////////////////////////////
	//以覆盖的方式，将内存写入文件
	static bool WriteMemoryToFile_W(const void *pMem, size_t elementSize, wchar_t *strFilePath);
	//////////////////////////////////////////////////////////////////////////
	//以追加的方式，将内存写入文件
	static bool WriteMemoryToFile_AW(const void *pMem, size_t elementSize, wchar_t *strFilePath);
	//////////////////////////////////////////////////////////////////////////
	//从文件读取内存，
	//首个元素指针，单个元素长度，元素个数，文件位置
	static int ReadMemoryFromFile(void *pMem, size_t buffersize, size_t elementSize, size_t  elementCount, wchar_t *strFilePath);
protected:
private:
	FILE* pFile;
	wchar_t  strFilePath[256];
	int mode;
	
};
