#pragma once
#include "CommonInclude.h"
#include"mysql.h"

#pragma comment( lib, "libmysql.lib")

#define TMT_DB_DATABASE			"tmt_database"
#define TMT_DB_TABLE_CAM		"camera"
#define TMT_DB_TABLE_CLIENT		"client_info"
#define TMT_DB_TABLE_USER		"user"

#define TMT_DB_CAM_ID			"id"
#define TMT_DB_CAM_CAMNUM		"cam_num"
#define TMT_DB_CAM_CAMNAME		"cam_name"
#define TMT_DB_CAM_FOLDER		"cam_folder"
#define TMT_DB_CAM_IPADDR		"server_ip"
#define TMT_DB_CAM_LOCATION_LAT	"location_lat"
#define TMT_DB_CAM_LOCATION_LNG	"location_lng"
#define TMT_DB_CAM_WARNINGLEVEL	"warning_level"
#define TMT_DB_CAM_CAMSTATUS	"cam_status"
#define TMT_DB_CAM_ALGO_MASK	"algo_mask"
#define TMT_DB_CAM_ALGO_STATUS	"algo_status"

#define TMT_DB_IMG_ID			"id"
#define TMT_DB_IMG_PATH			"image_path"
#define TMT_DB_IMG_GRABTIME		"grab_time"
#define TMT_DB_IMG_DEFECTSNUM	"defects_num"
#define TMT_DB_IMG_DEFECTSPOS	"defects_pos"
#define TMT_DB_IMG_HEIGHT		"height"
#define TMT_DB_IMG_WIDTH		"width"
#define TMT_DB_IMG_CAMID		"cam_id"

#define TMT_DB_CLIENT_ID		"id"
#define TMT_DB_CLIENT_IPADDR	"ip_address"
#define TMT_DB_CLIENT_PORT		"port"
#define TMT_DB_CLIENT_STATUS	"status"

#define TMT_DB_USER_ID			"id"
#define TMT_DB_USER_NAME		"name"
#define TMT_DB_USER_PASSWORD	"password"
#define TMT_DB_USER_CAM_WATCH	"cam_watch"
#define TMT_DB_USER_WECHARD_ID	"wechat_id"
#define TMT_DB_USER_AUTHORYLV	"authority_level"

struct DbManagerSetting
{
public:
	SendServerSetting mSendSetting;
	ReceiveServerSetting mRecvSetting;
	int m_Sleeptime=0;
	NetIP MysqlServerHost = { 0 };
	int MysqlServerPort = 0;
	//DbManagerSetting& operator= (const DbManagerSetting& DbMSetting)
	//{
	//}
};


class CDatabaseManager : public Thread
{
public://��ʼ������
	CDatabaseManager();
	~CDatabaseManager();
	void Initial();
	void DbMDestory();

public://���ݿ����
	bool ConnectDb();
	bool DisConnectDb();
	//************************************
	// ����:  �����������ݿⷵ��id��������ھͷ��ش��ڵ����id
	// ˵��:  
	// ����:  CDatabaseManager::AddCamToDb
	// Access:    public 
	// ����ֵ:   int  // camid
	// ����:   Tmtv_CameraInfo & mCam  //
	//************************************
	int AddCamToDb(Tmtv_CameraInfo& mCam);
	//************************************
	// ����: ���������Ϣ
	// ˵��:   �������Ƿ���ڣ�����ö��ֵ
	// ����:  CDatabaseManager::CheckCamInfo
	// Access:    public 
	// ����ֵ:   int  // 
	// ����:   Tmtv_CameraInfo & mCam  //
	//************************************
	bool DelCamFromDB(int mCamID);
	int CheckCamInfo(Tmtv_CameraInfo& mCam);
	//************************************
	// ����:  ��ӿͻ���
	// ˵��:  
	// ����:  CDatabaseManager::InsertClientInfoToDb
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   const Tmt_ClientInfo & mClientInfo  //
	//************************************
	bool InsertClientInfoToDb(const Tmt_ClientInfo & mClientInfo);
	//************************************
	// ����:  �������ݿ��пͻ���״̬
	// ˵��:  
	// ����:  CDatabaseManager::UpdateClientStatusDb
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   const Tmt_ClientInfo & mClientInfo  //
	//************************************
	bool UpdateClientStatusDb(const Tmt_ClientInfo & mClientInfo);
	//************************************
	// ����:  ����ͼƬ��Ϣ�����ݿ�
	// ˵��:  
	// ����:  CDatabaseManager::InsertImgInfoToDb
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   Tmtv_ImageInfo & mImginfo  //
	//************************************
	bool InsertImgInfoToDb(Tmtv_ImageInfo& mImginfo);

	//************************************
	// ����:  ��ȡidָ�����������
	// ˵��:  
	// ����:  CDatabaseManager::GetCaminfoFromDb
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   Tmtv_CameraInfo & mCam  //
	//************************************
	bool GetCaminfoFromDb(Tmtv_CameraInfo& mCam);
	//************************************
	// ����:  ���������Ϣ
	// ˵��:  
	// ����:  CDatabaseManager::UpdateCamInfoDb
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   Tmtv_CameraInfo & mCam  //
	//************************************
	bool UpdateCamInfoDb(Tmtv_CameraInfo& mCam);
	//************************************
	// ����:  ��ȡָ�����id������ͼƬ
	// ˵��:  
	// ����:  CDatabaseManager::GetLastImgInfoFrmDb
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   Tmtv_ImageInfo & mImg  //
	//************************************
	bool GetLastImgInfoFrmDb(Tmtv_ImageInfo& mImg);
	//************************************
	// ����:  ��ȡָ�����id��ָ�����ڵ�����ͼƬ
	// ˵��:  ���ڸ�ʽ��2016-04-20��
	// ����:  CDatabaseManager::GetImginfoByDate
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   vector<Tmtv_ImageInfo> & mimgVec  //
	// ����:   int Camid  //
	// ����:   const char * mdate  //
	//************************************
	bool GetImginfoByDate(vector<Tmtv_ImageInfo> &mimgVec,int Camid,const char* Date);

	//************************************
	// ����: ��ȡһ��ʱ���֮�� ָ�����id��ͼƬ��Ϣ
	// ˵��:  ʱ�����Ӧ�������ڣ�����: 2016-04-26 12:04:56 
	// ����:  CDatabaseManager::GetImginfoByTime
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   vector<Tmtv_ImageInfo> & mimgVec  //
	// ����:   int Camid  //
	// ����:   const char * mtime_old  //
	// ����:   const char * mtime_new  //
	//************************************
	bool GetImginfoByTime(vector<Tmtv_ImageInfo> &mimgVec, int Camid, const char* mtime_old, const char* mtime_new);

	//************************************
	// ����:  ��ȡ�״̬�Ŀͻ�����Ϣ
	// ˵��:  
	// ����:  CDatabaseManager::GetActiveClientInfoFrmDb
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   vector<Tmt_ClientInfo> & mClientVec  //
	//************************************
	bool GetActiveClientInfoFrmDb(vector<Tmt_ClientInfo> &mClientVec);

	//************************************
	// ����:  ��ȡ����������κ�״̬�ģ�
	// ˵��:  
	// ����:  CDatabaseManager::GetAllCamInfoFrmDb
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   vector<Tmtv_CameraInfo> & mCamVec  //
	//************************************
	bool GetAllCamInfoFrmDb(vector<Tmtv_CameraInfo> &mCamVec);

	//************************************
	// ����:  ��ȡ����״̬�����
	// ˵��:  
	// ����:  CDatabaseManager::GetRunningCamFrmDb
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   vector<Tmtv_CameraInfo> & mCamVec  //
	//************************************
	bool GetRunningCamFrmDb(vector<Tmtv_CameraInfo> &mCamVec);

	//************************************
	// ����:  ��ȡ��������״̬�ĵ����������ͼƬ
	// ˵��:  
	// ����:  CDatabaseManager::GetLastImginfoAllCam
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   vector<Tmtv_ImageInfo> & mimgVec  //
	//************************************
	bool GetLastImginfoAllCam(vector<Tmtv_ImageInfo> &mimgVec);

	//************************************
	// ����:  ����û�
	// ˵��:  
	// ����:  CDatabaseManager::AddUserinfo
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   Tmt_UserInfo & mUser  //
	//************************************
	bool AddUserinfo(Tmt_UserInfo& mUser);

	//************************************
	// ����:  ����û����Ƿ����
	// ˵��:  
	// ����:  CDatabaseManager::CheckUserName
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   Tmt_UserInfo & mUser  //
	//************************************
	bool CheckUserName(Tmt_UserInfo& mUser);

	//************************************
	// ����:  ���û��������û���Ϣ
	// ˵��:  
	// ����:  CDatabaseManager::UpdateUserinfo
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   Tmt_UserInfo & mUser  //
	//************************************
	bool UpdateUserinfo(Tmt_UserInfo& mUser);
	
	//************************************
	// ����:  ���û�����ȡ�û���Ϣ
	// ˵��:  
	// ����:  CDatabaseManager::GetUserinfo
	// Access:    public 
	// ����ֵ:   bool  // 
	// ����:   Tmt_UserInfo & mUser  //
	//************************************
	bool GetUserinfo(Tmt_UserInfo& mUser);


public://��Ա����������
	bool LoadSetting();
	bool SaveSetting();

public://��Ϣ����
	bool ResponseAsk(MessageItem &pMsgItem, Tmtv_BaseNetMessage &src_msg, int mType);

public://�̺߳���
	void Task();




private://˽�к���
	bool GetYearMonth(char* pData, int Datalength);
	int GetDefectsPosFromJson(int DfPos[][8], char *JsonStr, int DefectsNum);
	int ConvertDefectsToJson(char* JsonStr, int bufferlength, int DfPos[][8], int DefectsNum);
	int GetDefectsPosFromStr(char* str,int DfPos[][8]);
	bool CreatImgTable();
private://˽�б���
	MYSQL m_mysql;
	int m_DbStatus;
	vector<Tmt_ClientInfo> m_ClientInfoVec;
	SendServer m_SendServer;
	ReceiveServer m_ReceiveServer;
	DbManagerSetting m_Setting;








public://ö����
	enum DbStatus{
		DB_DISCONNECT=0,
		DB_CONNECT,
		DB_ERROR,
	};
	enum CheckStatus {
		CamCheckOK=0,
		CamIndexExist=1,
		CamNameExist,
		CamPathExist,
	};








};

class DbServerLogger
{
public:
	static Logger mLogger;

};