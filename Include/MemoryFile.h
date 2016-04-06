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
//
//#ifndef _FILE_DEFINED
//#define _FILE_DEFINED
//typedef struct _iobuf
//{
//	void* _Placeholder;
//} FILE;
//#endif

///<header_info>
//==============================================================================
#pragma once
#include <stdio.h>
//==============================================================================
///</header_info>


class CMemoryFile
{
public:
	CMemoryFile();
	~CMemoryFile();
	//////////////////////////////////////////////////////////////////////////
	//��ֻ����ʽ��
	bool OpenFile_R(wchar_t *pFileName);
	//////////////////////////////////////////////////////////////////////////
	//�Ը���д�뷽ʽ��
	bool OpenFile_W(wchar_t *pFileName);
	//////////////////////////////////////////////////////////////////////////
	//��׷��д�뷽ʽ��
	bool OpenFile_AW(wchar_t *pFileName);
	//////////////////////////////////////////////////////////////////////////
	//���ڴ�д���ļ�
	bool WriteMemoryToFile(const void *pMem,size_t mLength);
	//////////////////////////////////////////////////////////////////////////
	//���ļ���ȡ�ڴ�
	bool ReadMemoryFromFile(void *pMem, size_t mLength);

	bool CloseFile();
	//////////////////////////////////////////////////////////////////////////
	//�Ը��ǵķ�ʽ�����ڴ�д���ļ�
	static bool WriteMemoryToFile_W(const void *pMem, size_t elementSize, wchar_t *strFilePath);
	//////////////////////////////////////////////////////////////////////////
	//��׷�ӵķ�ʽ�����ڴ�д���ļ�
	static bool WriteMemoryToFile_AW(const void *pMem, size_t elementSize, wchar_t *strFilePath);
	//////////////////////////////////////////////////////////////////////////
	//���ļ���ȡ�ڴ棬
	//�׸�Ԫ��ָ�룬����Ԫ�س��ȣ�Ԫ�ظ������ļ�λ��
	static int ReadMemoryFromFile(void *pMem, size_t elementSize, size_t  elementCount, wchar_t *strFilePath);
protected:
private:
	FILE* pFile;
	wchar_t  strFilePath[256];
	int mode;
	
};
