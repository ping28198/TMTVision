#ifndef COMMON_DEFINE_H
#define COMMON_DEFINE_H
//��־�������ʾ��Ϣ
static const char * KEYINFOPREFIX   = " KeyInfo: ";
static const char * ERRORPREFIX = " Error: ";
static const char * WARNINGPREFIX   = " Warning: ";
static const char * INFOPREFIX      = " Info: ";
static const int MAX_STR_LEN = 1024;
//��־����ö��
typedef enum tagEnumLogLevel
{
	LogLevelAll = 0,    //������Ϣ��д��־
	LogLevelMid,        //д���󡢾�����Ϣ
	LogLevelNormal,     //ֻд������Ϣ
	LogLevelStop        //��д��־
}EnumLogLevel;


#define TMTV_MAXSTRLEN	256//�ַ�����󳤶�
#define TMTV_MAXDEFECTNUM 16//����ͼ�������ȱ������
#define TMTV_CHECKCODE 20160321//����ͨ�ż����֤��Ϣ



















#endif


