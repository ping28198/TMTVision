/************************************************************************/
/* �����б�                                                                     */
/************************************************************************/
#define TMT_NORMAL (DWORD)0
#define ERR        (DWORD)1000
//�ļ������//////////////////////////////////////////////////////////////
#define FILE_NOT_EXIST			ERR+1
#define CALI_FILE_NOT_EXIST		ERR+2
#define CONFIG_FILE_NOT_EXIST	ERR+3  //�����ļ�������
//��������//////////////////////////////////////////////////////////////
#define CAM_NOT_EXIST			ERR+11
#define CAM_NUM_WRONG			ERR+12 //�������������
#define CAM_NAME_UNREAD			ERR+13 //�޷���ȡ�������
#define CAM_OPEN_FAILED			ERR+14 //���������
#define CAM_SEQUENCE_DETECT_FAILED	ERR+15//���˳���ж�ʧ��
//ͼ�������/////////////////////////////////////////////////////////////
#define GRAY_VALUE_TOO_BRIGHT	ERR+20 //ͼ��Ҷȹ���
#define GRAY_VALUE_TOO_DARK		ERR+21 //ͼ��Ҷȹ���
#define IMAGE_CAPTURE_FAILED	ERR+22 //��ȡ֡ʧ��
#define IMAGE_NOT_EXIST			ERR+23 //ͼ�񲻴���
#define FOOT_SHAPE_DETECT_FAILED ERR+30 //����ʶ�����