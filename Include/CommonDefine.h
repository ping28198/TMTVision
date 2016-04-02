///<proj_info>
//==============================================================================
// 项目名 ：智能监控
// 文件名 ：CommonDefine.h
// 作  者 ：任威平
// 用  途 ：提供宏
// 版  权 ：霍比特人 
//==============================================================================
///</proj_info>

///<ver_info>
// 版本记录	
//==============================================================================
//版本号  开发人员      时间      描述
//1.0     任威平      2016.3.26  创建
//1.1     王磊        2016.3.29  整合旧版本,调整了CommonDefine
//==============================================================================
///</ver_info>


#ifndef COMMON_DEFINE_H
#define COMMON_DEFINE_H

#define TMTV_TINYSTRLEN	    32//字符串最大长度
#define TMTV_SHORTSTRLEN	128//字符串最大长度
#define TMTV_LONGSTRLEN	    256//字符串最大长度
#define TMTV_HUGESTRLEN	    512//字符串最大长度
#define TMTV_IPSTRLEN	    16//字符串最大长度
#define TMTV_MAXDEFECTNUM   16//单个图像中最大缺陷数量
#define TMTV_MAXCAMERANUM   256//最大相机数量
#define TMTV_CHECKCODE      20160321//网络通信间的验证信息

///<datastruct_info>
//==============================================================================
// 基本数据类型定义
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
//定义常用的宏
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


