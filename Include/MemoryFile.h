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
	//************************************
	// 作用:  以只读方式打开
	// 说明:  
	// 名称:  CMemoryFile::OpenFile_R
	// Access:    public 
	// 返回值:   bool //打开是否成功
	// 参数:   const wchar_t * pFileName //文件路径指针
	//************************************
	bool OpenFile_R(const wchar_t *pFileName);
	//************************************
	// 作用:  以覆盖写入方式打开
	// 说明:  
	// 名称:  CMemoryFile::OpenFile_W
	// Access:    public 
	// 返回值:   bool //是否成功
	// 参数:   const wchar_t * pFileName //文件路径指针
	//************************************
	bool OpenFile_W(const wchar_t *pFileName);
	//************************************
	// 作用:  以追加写入方式打开
	// 说明:  
	// 名称:  CMemoryFile::OpenFile_AW
	// Access:    public 
	// 返回值:   bool //
	// 参数:   const wchar_t * pFileName //
	//************************************
	bool OpenFile_AW(const wchar_t *pFileName);
	//************************************
	// 作用:  将内存写入文件
	// 说明:  
	// 名称:  CMemoryFile::WriteMemoryToFile
	// Access:    public 
	// 返回值:   bool
	// 返回值含义: 成功写入字节数
	// 参数:   const void * pMem 内存地址
	// 参数:   size_t mLength 字节长度
	//************************************
	bool WriteMemoryToFile(const void *pMem,size_t mLength);
	//************************************
	// 作用:  从文件读取内存
	// 说明:  
	// 名称:  CMemoryFile::ReadMemoryFromFile
	// Access:    public static 
	// 返回值:   bool  // 是否成功
	// 参数:   void * pMem  // 内存地址
	// 参数:   size_t mLength  // 字节数
	//************************************
	bool ReadMemoryFromFile(void *pMem, size_t mLength);

	//************************************
	// 作用:  关闭文件
	// 说明:  打开文件后必须关闭文件
	// 名称:  CMemoryFile::CloseFile
	// Access:    public 
	// 返回值:   bool  //是否成功 
	//************************************
	bool CloseFile();
	//************************************
	// 作用:  以覆盖的方式，将内存写入文件
	// 说明:  静态
	// 名称:  CMemoryFile::WriteMemoryToFile_W
	// Access:    public static 
	// 返回值:   bool  // 
	// 参数:   const void * pMem  //内存地址
	// 参数:   size_t elementSize  //字节数
	// 参数:   const wchar_t * strFilePath  //文件地址
	//************************************
	static bool WriteMemoryToFile_W(const void *pMem, size_t elementSize, const wchar_t *strFilePath);
	//************************************
	// 作用:  以追加的方式，将内存写入文件，
	// 说明:  静态
	// 名称:  CMemoryFile::WriteMemoryToFile_AW
	// Access:    public static 
	// 返回值:   bool  // 
	// 参数:   const void * pMem  //内存地址
	// 参数:   size_t elementSize  //字节数
	// 参数:   const wchar_t * strFilePath  //文件名
	//************************************
	static bool WriteMemoryToFile_AW(const void *pMem, size_t elementSize, const wchar_t *strFilePath);
	//************************************
	// 作用:  从文件读取内存，静态
	// 说明:  若不知道文件内容长度，设置elementSize为1，元素个数设置大数
	// 名称:  CMemoryFile::ReadMemoryFromFile
	// Access:    public static 
	// 返回值:   int  // 成功个数
	// 参数:   void * pMem  //缓冲区指针
	// 参数:   size_t buffersize  //缓冲区长度
	// 参数:   size_t elementSize  //单个元素长度
	// 参数:   size_t elementCount  //元素个数
	// 参数:   const wchar_t * strFilePath  //文件名
	//************************************
	static int ReadMemoryFromFile(void *pMem, size_t buffersize, size_t elementSize, size_t  elementCount,const  wchar_t *strFilePath);
protected:
private:
	FILE* pFile;
	wchar_t  strFilePath[256];
	int mode;
	
};
