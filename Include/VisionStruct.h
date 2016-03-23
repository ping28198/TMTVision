#pragma once
#include "CommandDefine.h"


struct Tmtv_CameraInfo //�����Ϣ
{
	int Indexnum;//������
	char CameraName[TMTV_MAXSTRLEN];//������Ƶ���Ϣ
	char CameraPath[TMTV_MAXSTRLEN];//�����Ŀ¼·��
	int CameraPos[8];//x,y,z���꣬����
	int WarnningLevel;//����ȼ�
	char Reservechar[TMTV_MAXSTRLEN];//����
};

struct Tmtv_DefectInfo	//ȱ����Ϣ
{
	int DefectNum=0;//ȱ������
	int DefectPos[TMTV_MAXDEFECTNUM][8];//left,top,width,height,type,level,..
};

struct Tmtv_ImageInfo	//ͼ����Ϣ���������˷����������
{
	int CheckCode = TMTV_CHECKCODE;//��֤�룬��ֹͨ�Ÿ���
	Tmtv_CameraInfo mCameraInfo;
	Tmtv_DefectInfo mDefectInfo;
	char ImagePath[TMTV_MAXSTRLEN];//�ļ�·��
	int IsWarnning=0;//�Ƿ��о���
	int IsVIP=0;//�����Ƿ����ص�
	char Reservechar[TMTV_MAXSTRLEN];//����
};

struct Tmtv_AskInfo//ָ����Ϣ��������˷�����������
{
	int CheckCode = TMTV_CHECKCODE;//��֤�룬��ֹͨ�Ÿ���
	int Asktype;//�������ͣ�100������CameraIndex�����101���ر�CameraIndex�����102��������CameraIndex����������ݣ�103��
	int CameraIndex;//���������
	int CheckCode;//У���룬������������֮��
};
