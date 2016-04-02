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

#define TMTV_TINYSTRLEN	    32//�ַ�����󳤶�
#define TMTV_SHORTSTRLEN	128//�ַ�����󳤶�
#define TMTV_LONGSTRLEN	    256//�ַ�����󳤶�
#define TMTV_HUGESTRLEN	    512//�ַ�����󳤶�
#define TMTV_IPSTRLEN	    16//�ַ�����󳤶�
#define TMTV_MAXDEFECTNUM   16//����ͼ�������ȱ������
#define TMTV_MAXCAMERANUM   256//����������
#define TMTV_CHECKCODE      20160321//����ͨ�ż����֤��Ϣ

///<datastruct_info>
//==============================================================================
// �����������Ͷ���
typedef wchar_t				TINYWSTR[TMTV_TINYSTRLEN];
typedef wchar_t				SHORTWSTR[TMTV_SHORTSTRLEN];
typedef wchar_t				LONGWSTR[TMTV_LONGSTRLEN];
typedef wchar_t				HUGEWSTR[TMTV_HUGESTRLEN];
typedef wchar_t				NetIPW[TMTV_IPSTRLEN];
typedef wchar_t*			WStrPointer;
typedef wchar_t				WStrChar;

typedef char				TINYSTR[TMTV_TINYSTRLEN];
typedef char				SHORTSTR[TMTV_SHORTSTRLEN];
typedef char				LONGSTR[TMTV_LONGSTRLEN];
typedef char				HUGESTR[TMTV_HUGESTRLEN];
typedef char				NetIP[TMTV_IPSTRLEN];
typedef char*			    StrPointer;
typedef char				StrChar;
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


