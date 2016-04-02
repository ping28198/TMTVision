///<proj_info>
//==============================================================================
// ��Ŀ�� �����ܼ��
// �ļ��� ��CommonDefine.h
// ��  �� ������ƽ
// ��  ; ���ṩ��
// ��  Ȩ ���������� 
//==============================================================================
///</proj_info>

///<ver_info>
// �汾��¼	
//==============================================================================
//�汾��  ������Ա      ʱ��      ����
//1.0     ����ƽ      2016.3.26  ����
//1.1     ����        2016.3.29  ���Ͼɰ汾,������CommonDefine
//==============================================================================
///</ver_info>


#ifndef COMMON_DEFINE_H
#define COMMON_DEFINE_H

#define TMTV_MAXSTRLEN	256//�ַ�����󳤶�
#define TMTV_MAXDEFECTNUM 16//����ͼ�������ȱ������
#define TMTV_CHECKCODE 20160321//����ͨ�ż����֤��Ϣ

///<datastruct_info>
//==============================================================================
// �����������Ͷ���
typedef __int32				ModuleHandle;
typedef __int32				MsgID;
typedef __int32				MsgLParam;
typedef __int32				MsgWParam;
typedef __int16				ShortInt;
typedef __int16*			PShortInt;
typedef unsigned __int16	UShortInt;
typedef unsigned __int16*	PUShortInt;
typedef __int16				TaskID;
typedef __int32				LongInt;
typedef __int32*			PLongInt;
typedef unsigned __int32    ULongInt;
typedef unsigned __int32*   PULongInt;
typedef  unsigned __int16	NetPort;
typedef __int64				LongLongInt;
typedef __int64*			PLongLongInt;
typedef double				LongFloat;
typedef double*				PLongFloat;
typedef unsigned char		ImgDataType;
typedef unsigned char*		PImgDataType;
typedef unsigned char		BufferByte;
typedef bool				MsgBool;
typedef void*				PObjPointer;
typedef void*				PMemPointer;
typedef char				Byte;
typedef char*				PByteBuffer;

typedef wchar_t				TINYSTR[32];
typedef wchar_t				SHORTSTR[128];
typedef wchar_t				LONGSTR[256];
typedef wchar_t				HUGESTR[512];
typedef wchar_t				NetIP[16];
typedef wchar_t*			StrPointer;
typedef wchar_t				StrChar;
//==============================================================================
///</datastruct_info>


///<macro_info>
//==============================================================================
//���峣�õĺ�
#ifndef DWORD
#define DWORD unsigned long
#endif
#ifndef STR_LEN
#define STR_LEN(x)  (sizeof(x)/sizeof(x[0]))
#endif
#ifndef MaxULongInt
#define MaxULongInt   ((ULongInt)0xFFFFFFFF)
#endif
#ifndef MaxLongInt
#define MaxLongInt    ((LongInt) 0x7FFFFFFF)
#endif
#ifndef MIN
#define MIN(i,j) i<j?i:j
#endif
#ifndef MAX
#define MAX(i,j) i<j?j:i
#endif
#ifndef MOD
#define MOD(i,j) i%j==j?0:i%j
#endif
#ifndef NULL
#define NULL 0
#endif
//==============================================================================
///</macro_info>

#endif


