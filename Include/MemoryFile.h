#pragma once

///<proj_info>
//==============================================================================
// ��Ŀ�� ��ͼ����ƽ̨
// �ļ��� ��MemoryFile.h
// ��  �� ������ƽ
// ��  ; ��ʵ���˲����࣬����������д���ļ����ʹ��ļ���ȡ���ݼ��ص��ڴ�
// ��  Ȩ ��TMT
//==============================================================================
///</proj_info>

///<ver_info>
// �汾��¼	
//==============================================================================
//�汾��  ������Ա      ʱ��      ����
//1.0     ����ƽ      2016-03-25   ����ģ��

//==============================================================================
///</ver_info>








class CMemoryFile
{
public:
	CMemoryFile();
	~CMemoryFile();
	//************************************
	// ����:  ��ֻ����ʽ��
	// ˵��:  
	// ����:  CMemoryFile::OpenFile_R
	// Access:    public 
	// ����ֵ:   bool //���Ƿ�ɹ�
	// ����:   const wchar_t * pFileName //�ļ�·��ָ��
	//************************************
	bool OpenFile_R(const wchar_t *pFileName);
	//************************************
	// ����:  �Ը���д�뷽ʽ��
	// ˵��:  
	// ����:  CMemoryFile::OpenFile_W
	// Access:    public 
	// ����ֵ:   bool //�Ƿ�ɹ�
	// ����:   const wchar_t * pFileName //�ļ�·��ָ��
	//************************************
	bool OpenFile_W(const wchar_t *pFileName);
	//************************************
	// ����:  ��׷��д�뷽ʽ��
	// ˵��:  
	// ����:  CMemoryFile::OpenFile_AW
	// Access:    public 
	// ����ֵ:   bool //
	// ����:   const wchar_t * pFileName //
	//************************************
	bool OpenFile_AW(const wchar_t *pFileName);
	//************************************
	// ����:  ���ڴ�д���ļ�
	// ˵��:  
	// ����:  CMemoryFile::WriteMemoryToFile
	// Access:    public 
	// ����ֵ:   bool
	// ����ֵ����: �ɹ�д���ֽ���
	// ����:   const void * pMem �ڴ��ַ
	// ����:   size_t mLength �ֽڳ���
	//************************************
	bool WriteMemoryToFile(const void *pMem,size_t mLength);
	//************************************
	// ����:  ���ļ���ȡ�ڴ�
	// ˵��:  
	// ����:  CMemoryFile::ReadMemoryFromFile
	// Access:    public static 
	// ����ֵ:   bool  // �Ƿ�ɹ�
	// ����:   void * pMem  // �ڴ��ַ
	// ����:   size_t mLength  // �ֽ���
	//************************************
	bool ReadMemoryFromFile(void *pMem, size_t mLength);

	//************************************
	// ����:  �ر��ļ�
	// ˵��:  ���ļ������ر��ļ�
	// ����:  CMemoryFile::CloseFile
	// Access:    public 
	// ����ֵ:   bool  //�Ƿ�ɹ� 
	//************************************
	bool CloseFile();
	//************************************
	// ����:  �Ը��ǵķ�ʽ�����ڴ�д���ļ�
	// ˵��:  ��̬
	// ����:  CMemoryFile::WriteMemoryToFile_W
	// Access:    public static 
	// ����ֵ:   bool  // 
	// ����:   const void * pMem  //�ڴ��ַ
	// ����:   size_t elementSize  //�ֽ���
	// ����:   const wchar_t * strFilePath  //�ļ���ַ
	//************************************
	static bool WriteMemoryToFile_W(const void *pMem, size_t elementSize, const wchar_t *strFilePath);
	//************************************
	// ����:  ��׷�ӵķ�ʽ�����ڴ�д���ļ���
	// ˵��:  ��̬
	// ����:  CMemoryFile::WriteMemoryToFile_AW
	// Access:    public static 
	// ����ֵ:   bool  // 
	// ����:   const void * pMem  //�ڴ��ַ
	// ����:   size_t elementSize  //�ֽ���
	// ����:   const wchar_t * strFilePath  //�ļ���
	//************************************
	static bool WriteMemoryToFile_AW(const void *pMem, size_t elementSize, const wchar_t *strFilePath);
	//************************************
	// ����:  ���ļ���ȡ�ڴ棬��̬
	// ˵��:  ����֪���ļ����ݳ��ȣ�����elementSizeΪ1��Ԫ�ظ������ô���
	// ����:  CMemoryFile::ReadMemoryFromFile
	// Access:    public static 
	// ����ֵ:   int  // �ɹ�����
	// ����:   void * pMem  //������ָ��
	// ����:   size_t buffersize  //����������
	// ����:   size_t elementSize  //����Ԫ�س���
	// ����:   size_t elementCount  //Ԫ�ظ���
	// ����:   const wchar_t * strFilePath  //�ļ���
	//************************************
	static int ReadMemoryFromFile(void *pMem, size_t buffersize, size_t elementSize, size_t  elementCount,const  wchar_t *strFilePath);
protected:
private:
	FILE* pFile;
	wchar_t  strFilePath[256];
	int mode;
	
};
