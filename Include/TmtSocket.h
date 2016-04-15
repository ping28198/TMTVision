
///<proj_info>
//==============================================================================
// ��Ŀ�� : ͼ����ƽ̨
// �ļ��� : TmtSocket.h
// ��  �� : TMT
// ��  ; : ���������紫����
// ��  Ȩ : TMT
//==============================================================================
///</proj_info>

///<ver_info>
// �汾��¼	
//==============================================================================
//�汾��  ������Ա      ʱ��        ����
//1.0     ����ƽ      2016-03-29   ����ģ��
//1.1     ����        2016-03-31   ����߳�
//==============================================================================
///</ver_info>



///<header_info>
//==============================================================================
#pragma once
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"WS2_32.lib")
#include "Thread.h"
#include "CommonDefine.h"
//==============================================================================
///</header_info>

///<class_info>
//==============================================================================
//����ͨѶSocket������
class TmtSocket
{
public:
	TmtSocket();
	~TmtSocket();

	//************************************
	// ����:    ��ȡ������е�һ����Ч������IP��ַ
	// ����:  TmtSocket::GetAvailableNetMac
	// Access:    public static 
	// ����ֵ:   bool \\
	// ����: NetIP & localIP  \\IP��ַ��Сд������-�ţ�
	//************************************
	static bool GetAvailableNetIP(NetIP& localIP);

	//************************************
	// ����:    ����Ŀ���ַ
	// ����:  TmtSocket::SetSendAddr
	// Access:    public 
	// ����ֵ:   bool \\
	// ����: int remoteRecvPort  \\Ŀ��IP
	// ����: char * remoteRecvIp  \\Ŀ��˿�
	// ����: int localSendPort  \\�ҵĶ˿�
	// ����: char * localSendIP  \\�ҵ�IP
	//************************************
	bool SetSendAddr(int remoteRecvPort,char* remoteRecvIp,
		int localSendPort=0,char* localSendIP=NULL);//�������������ʹ��Ĭ��, ��ʹ��ϵͳָ����ip���������port����
 
	//************************************
	// ����:    ���ý��յ�ַ
	// ����:  TmtSocket::SetRecvAddr
	// Access:    public 
	// ����ֵ:   bool \\
	// ����: int localRecvPort  \\�Լ��Ķ˿�
	// ����: char * localRecvIP  \\ip��ַ,λnullʱ����������
	//************************************
	bool SetRecvAddr( int localRecvPort, char* localRecvIP = NULL);

	//************************************
	// ����:    ���ÿ�ѡ����
	// ����:  TmtSocket::SetOption
	// Access:    public 
	// ����ֵ:   bool \\
	// ����: DWORD flag  \\ö��   0 0 0 0 0 0 0 0
    //                           �����ظ��󶨩���������־λ
	//************************************
	enum { RECV_NOWAIT = 1, ADDR_REUSE = 2 };
	bool SetOption(DWORD flag);

	//��ȡsocket����״̬
	DWORD GetStatus();

	//////////////////////////////////////////////////////////////////////////
	//������ָ��, ��Ҫ�������ݳ���
	int SendMsg(void *pBuffer,size_t MsgLength);
	//////////////////////////////////////////////////////////////////////////
	//������ָ��, ����������, ���յ�����Ϣ����, �Է��˿ں�, �Է�ip
	int RecvMsg(void *pBuffer, size_t bufLength, int *pRemoteSendPort = NULL, char* pRemoteSendIp = NULL);

	int ReSet();//����Ҫ��������
	int ReleaseSocket();//�ֶ��ͷ�socket
public:
	enum { enNoSendOrRecv = 0, enSendOK = 1, enRecvOK = 2, enSendAndRecvOK = 3};
	DWORD m_SkStatus;
	bool m_SKInitialOK;
	int m_OptionFlag;
	int m_RemoteRecvPort;
	NetIP m_RemoteRecvIP;
	int m_LocalSendPort;
	NetIP m_LocalSendIP;
	int m_LocalRecvPort;
	NetIP m_LocalRecvIP;
private:
	//int err;
	//bool m_IsNormal;
	WSADATA wsadata;
	SOCKET sock_recv;
	SOCKET sock_send;
	SOCKADDR_IN Rc_MyAddr;
	SOCKADDR_IN Rc_SrcAddr;
	SOCKADDR_IN Sd_DstAddr;
	SOCKADDR_IN Sd_MyAddr;

public:
	//���Ժ���,��ʾ������Ϣ
	//method=0 xml ������ʽ���, ������
	//1 ��̸�ʽ���,ֻ�����������
	//2 ���¸�ʽ���,�����������Ԫ��
	//�ֱ�����ɫ��Χ: 40--49        ����ɫ: 30--39
	//	40 : ��                           30 : ��
	//	41 : ��                           31 : ��
	//	42 : ��                           32 : ��
	//	43 : ��                           33 : ��
	//	44 : ��                           34 : ��
	//	45 : ��                           35 : ��
	//	46 : ����                         36 : ����
	//	47 : ��ɫ                         37 : ��ɫ
	void ToString(MEGAWSTR &string, int method = 0, int color = 32);
};
//==============================================================================
///</class_info>


///<class_info>
//==============================================================================
//����ͨѶSocket������
class TmtSocketServer :public Thread, public TmtSocket
{
public:
	//�๦��
	char *pBuffer;
	enum { MINBUFFERSIZE = 256 ,MAXBUFFERSIZE = 10240};
	int m_BufferSize;
public:
	TmtSocketServer(int bufferSize = MAXBUFFERSIZE);
	~TmtSocketServer();
	bool Initial(int remoteRecvPort, char * remoteRecvIp, 
		         int localRecvPort, char* localRecvIP, 
		         int localSendPort = 0, char * localSendIP = NULL, DWORD optionFlag = 1);
	bool Unitial();
	//Socket����
public:
	void Task(void);
	virtual void ServerProcess(int msgLen)=0;

public:
	//���Ժ���,��ʾ������Ϣ
	//method=0 xml ������ʽ���, ������
	//1 ��̸�ʽ���,ֻ�����������
	//2 ���¸�ʽ���,�����������Ԫ��
	//�ֱ�����ɫ��Χ: 40--49        ����ɫ: 30--39
	//	40 : ��                           30 : ��
	//	41 : ��                           31 : ��
	//	42 : ��                           32 : ��
	//	43 : ��                           33 : ��
	//	44 : ��                           34 : ��
	//	45 : ��                           35 : ��
	//	46 : ����                         36 : ����
	//	47 : ��ɫ                         37 : ��ɫ
	void ToString(MEGAWSTR &string, int method = 0, int color = 32);
};
//==============================================================================
///</class_info>