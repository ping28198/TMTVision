///<proj_info>
//==============================================================================
// ��Ŀ�� �����ܼ��
// �ļ��� ��Logger.h
// ��  �� ������ƽ
// ��  ; ���ṩд��־���ܣ�֧�ֶ��̣߳�֧�ֿɱ��β���������֧��д��־���������
//          �ӿڣ�SetLogLevel������д��־����
//          TraceKeyInfo��������־����д�ؼ���Ϣ
//          TraceError��д������Ϣ
//          TraceWarning��д������Ϣ
//          TraceInfo��дһ����Ϣ
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

#ifndef LOGGER_H_
#define LOGGER_H_
#include <Windows.h>
#include <stdio.h>
#include "CommonDefine.h"
//��־�������ʾ��Ϣ
static const char * KEYINFOPREFIX = " KeyInfo: ";
static const char * ERRORPREFIX = " Error: ";
static const char * WARNINGPREFIX = " Warning: ";
static const char * INFOPREFIX = " Info: ";
static const int MAX_STR_LEN = 1024;
//��־����ö��
typedef enum tagEnumLogLevel
{
	LogLevelAll = 0,    //������Ϣ��д��־
	LogLevelMid,        //д���󡢾�����Ϣ
	LogLevelNormal,     //ֻд������Ϣ
	LogLevelStop        //��д��־
}EnumLogLevel;

/*
    * ������Logger
    * ���ã��ṩд��־���ܣ�֧�ֶ��̣߳�֧�ֿɱ��β���������֧��д��־���������
    * �ӿڣ�SetLogLevel������д��־����
            TraceKeyInfo��������־����д�ؼ���Ϣ
            TraceError��д������Ϣ
            TraceWarning��д������Ϣ
            TraceInfo��дһ����Ϣ
*/
class Logger
{
public:
    //Ĭ�Ϲ��캯��
    Logger();
    //���캯��
    Logger(const char * strLogPath, EnumLogLevel nLogLevel = LogLevelNormal);
    //��������
    virtual ~Logger();
public:
    //д�ؼ���Ϣ
    void TraceKeyInfo(const char * strInfo, ...);
    //д������Ϣ
    void TraceError(const char* strInfo, ...);
    //д������Ϣ
    void TraceWarning(const char * strInfo, ...);
    //дһ����Ϣ
    void TraceInfo(const char * strInfo, ...);
    //����д��־����
    void SetLogLevel(EnumLogLevel nLevel);
private:
    //д�ļ�����
    void Trace(const char * strInfo);
    //��ȡ��ǰϵͳʱ��
    char * GetCurrentTime();
    //������־�ļ�����
    void GenerateLogName();
    //������־·��
    void CreateLogPath();
private:
    //д��־�ļ���
    FILE * m_pFileStream;
    //д��־����
    EnumLogLevel m_nLogLevel;
    //��־��·��
    char m_strLogPath[MAX_STR_LEN];
    //��־������
    char m_strCurLogName[MAX_STR_LEN];
    //�߳�ͬ�����ٽ�������
    CRITICAL_SECTION m_cs;
};
 
#endif