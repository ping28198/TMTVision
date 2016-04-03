
///<proj_info>
//==============================================================================
// ��Ŀ�� ��ͼ����ƽ̨
// �ļ��� ��TmtSocket.h
// ��  �� ��TMT
// ��  ; �����������紫����
// ��  Ȩ ��TMT
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
	//////////////////////////////////////////////////////////////////////////
	//����Ŀ���ַ
	//Ŀ��IP��Ŀ��˿ڣ��ҵĶ˿ڣ��ҵ�IP
	bool SetSendAddr(int remoteRecvPort,char* remoteRecvIp,
		int localSendPort=0,char* localSendIP=NULL);//�������������ʹ��Ĭ�ϣ���ʹ��ϵͳָ����ip���������port����
	//////////////////////////////////////////////////////////////////////////
	//���ý��յ�ַ
	//�Լ��Ķ˿�,ip��ַ,λnullʱ����������
	bool SetRecvAddr( int localRecvPort, char* localRecvIP = NULL);
	//////////////////////////////////////////////////////////////////////////
	//�ο�Tmtv_SocketOptionö��   0 0 0 0 0 0 0 0
	//                            �����ظ��󶨩���������־λ
	enum {RECV_NOWAIT = 1,ADDR_REUSE = 2};
	bool SetOption(DWORD flag);//���ÿ�ѡ����
	//////////////////////////////////////////////////////////////////////////
	//������ָ�룬��Ҫ�������ݳ���
	int SendMsg(void *pBuffer,size_t MsgLength);
	//////////////////////////////////////////////////////////////////////////
	//������ָ�룬���������ȣ����յ�����Ϣ���ȣ��Է��˿ںţ��Է�ip
	int RecvMsg(void *pBuffer, size_t bufLength, int *pRemoteSendPort = NULL, char* pRemoteSendIp = NULL);

	int ReSet();//����Ҫ��������
	int ReleaseSocket();//�ֶ��ͷ�socket
public:
	enum { enNoSendOrRecv = 0, enSendOK= 1, enRecvOK = 2, enSendAndRecvOK = 3};
	DWORD m_SkStatus;
	bool m_SKInitialOK;
	int m_OptionFlag;
	int m_RemoteRecvPort;
	char m_RemoteRecvIp;
	int m_LocalSendPort;
	char m_LocalSendIP;
	int m_LocalRecvPort;
	char m_LocalRecvIP;
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
	bool Initial(int remotePort, char* remoteIp, int localRecvPort, char* localRecvIP = NULL,int localPort = 0, char* localIP = NULL, DWORD optionFlag = 1);
	bool Unitial();
	//Socket����
public:
	void Task(void);
	virtual void ServerProcess(int msgLen)=0;
};
//==============================================================================
///</class_info>