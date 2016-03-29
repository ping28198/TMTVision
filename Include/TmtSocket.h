
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
//�汾��  ������Ա      ʱ��      ����
//1.0     ����ƽ      2016-03-29   ����ģ��
//
//==============================================================================
///</ver_info>




#pragma once
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"WS2_32.lib")

enum Tmtv_SocketOption
{
	ADDR_REUSE=1,//����󶨵�����ʹ�õĶ˿�
	RECV_NOWAIT,//����ʱ������

};

class TmtSocket
{
public:
	TmtSocket();
	~TmtSocket();
	//////////////////////////////////////////////////////////////////////////
	//����Ŀ���ַ
	//Ŀ��IP��Ŀ��˿ڣ��ҵĶ˿ڣ��ҵ�IP
	bool SetSendAddr(char* DstIp,int DstPort,int MyPort=0,char* MyIp=NULL);//�������������ʹ��Ĭ�ϣ���ʹ��ϵͳָ����ip���������port����
	//////////////////////////////////////////////////////////////////////////
	//���ý��յ�ַ
	//�Լ��Ķ˿�,ip��ַ,λnullʱ����������
	bool SetRecvAddr( int MyPort, char* MyIp = NULL);
	//////////////////////////////////////////////////////////////////////////
	//�ο�Tmtv_SocketOptionö��
	bool SetOption(int flag);//���ÿ�ѡ����
	//////////////////////////////////////////////////////////////////////////
	//������ָ�룬��Ҫ�������ݳ���
	int SendMsg(void *pBuffer,size_t MsgLength);
	//////////////////////////////////////////////////////////////////////////
	//������ָ�룬���������ȣ����յ�����Ϣ���ȣ��Է��˿ںţ��Է�ip
	int RecvMsg(void *pBuffer, size_t bufLength, int *SrcPort = NULL, char* SrcIp = NULL);

	int ReSet();//����Ҫ��������
	int ReleaseSocket();//�ֶ��ͷ�socket
private:
	int err;
	bool m_IsNormal;

	WSADATA wsadata;

	SOCKET sock_recv;
	SOCKET sock_send;
	SOCKADDR_IN Rc_MyAddr;
	SOCKADDR_IN Rc_SrcAddr;
	SOCKADDR_IN Sd_DstAddr;
	SOCKADDR_IN Sd_MyAddr;
};

