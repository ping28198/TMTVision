#ifndef COMMON_DEFINE_H
#define COMMON_DEFINE_H
//日志级别的提示信息
static const char * KEYINFOPREFIX   = " KeyInfo: ";
static const char * ERRORPREFIX = " Error: ";
static const char * WARNINGPREFIX   = " Warning: ";
static const char * INFOPREFIX      = " Info: ";
static const int MAX_STR_LEN = 1024;
//日志级别枚举
typedef enum tagEnumLogLevel
{
	LogLevelAll = 0,    //所有信息都写日志
	LogLevelMid,        //写错误、警告信息
	LogLevelNormal,     //只写错误信息
	LogLevelStop        //不写日志
}EnumLogLevel;


#define TMTV_MAXSTRLEN	256//字符串最大长度
#define TMTV_MAXDEFECTNUM 16//单个图像中最大缺陷数量
#define TMTV_CHECKCODE 20160321//网络通信间的验证信息



















#endif


