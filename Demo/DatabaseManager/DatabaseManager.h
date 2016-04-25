#pragma once
#include "CommonInclude.h"
#include"mysql.h"
#include "TmtConfig.h"
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
	//DbManagerSetting& operator= (const DbManagerSetting& DbMSetting)
	//{
	//}
};


class CDatabaseManager : public Thread
{
public://初始化函数
	CDatabaseManager();
	~CDatabaseManager();
	void Initial();


public://数据库操作
	bool ConnectDb();
	bool DisConnectDb();
	//************************************
	// 作用:  添加相机到数据库返回id，如果存在就返回存在的相机id
	// 说明:  
	// 名称:  CDatabaseManager::AddCamToDb
	// Access:    public 
	// 返回值:   int  // camid
	// 参数:   Tmtv_CameraInfo & mCam  //
	//************************************
	int AddCamToDb(Tmtv_CameraInfo& mCam);
	//************************************
	// 作用: 插入相机信息
	// 说明:   检查相机是否存在，返回枚举值
	// 名称:  CDatabaseManager::CheckCamInfo
	// Access:    public 
	// 返回值:   int  // 
	// 参数:   Tmtv_CameraInfo & mCam  //
	//************************************
	int CheckCamInfo(Tmtv_CameraInfo& mCam);
	//************************************
	// 作用:  添加客户端
	// 说明:  
	// 名称:  CDatabaseManager::InsertClientInfoToDb
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   const Tmt_ClientInfo & mClientInfo  //
	//************************************
	bool InsertClientInfoToDb(const Tmt_ClientInfo & mClientInfo);
	//************************************
	// 作用:  更新数据库中客户端状态
	// 说明:  
	// 名称:  CDatabaseManager::UpdateClientStatusDb
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   const Tmt_ClientInfo & mClientInfo  //
	//************************************
	bool UpdateClientStatusDb(const Tmt_ClientInfo & mClientInfo);
	//************************************
	// 作用:  插入图片信息到数据库
	// 说明:  
	// 名称:  CDatabaseManager::InsertImgInfoToDb
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   Tmtv_ImageInfo & mImginfo  //
	//************************************
	bool InsertImgInfoToDb(Tmtv_ImageInfo& mImginfo);
	//************************************
	// 作用:  更新相机信息
	// 说明:  
	// 名称:  CDatabaseManager::UpdateCamInfoDb
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   Tmtv_CameraInfo & mCam  //
	//************************************
	bool UpdateCamInfoDb(Tmtv_CameraInfo& mCam);
	//************************************
	// 作用:  获取指定相机id的最新图片
	// 说明:  
	// 名称:  CDatabaseManager::GetLastImgInfoFrmDb
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   Tmtv_ImageInfo & mImg  //
	//************************************
	bool GetLastImgInfoFrmDb(Tmtv_ImageInfo& mImg);
	//************************************
	// 作用:  获取指定相机id，指定日期的所有图片
	// 说明:  日期格式（2016-04-20）
	// 名称:  CDatabaseManager::GetImginfoByDate
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   vector<Tmtv_ImageInfo> & mimgVec  //
	// 参数:   int Camid  //
	// 参数:   const char * mdate  //
	//************************************
	bool GetImginfoByDate(vector<Tmtv_ImageInfo> &mimgVec,int Camid,const char* mdate);

	//************************************
	// 作用:  获取活动状态的客户端信息
	// 说明:  
	// 名称:  CDatabaseManager::GetActiveClientInfoFrmDb
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   vector<Tmt_ClientInfo> & mClientVec  //
	//************************************
	bool GetActiveClientInfoFrmDb(vector<Tmt_ClientInfo> &mClientVec);

	//************************************
	// 作用:  获取所有相机（任何状态的）
	// 说明:  
	// 名称:  CDatabaseManager::GetAllCamInfoFrmDb
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   vector<Tmtv_CameraInfo> & mCamVec  //
	//************************************
	bool GetAllCamInfoFrmDb(vector<Tmtv_CameraInfo> &mCamVec);

	//************************************
	// 作用:  获取运行状态的相机
	// 说明:  
	// 名称:  CDatabaseManager::GetRunningCamFrmDb
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   vector<Tmtv_CameraInfo> & mCamVec  //
	//************************************
	bool GetRunningCamFrmDb(vector<Tmtv_CameraInfo> &mCamVec);

	//************************************
	// 作用:  获取所有运行状态的的相机的最新图片
	// 说明:  
	// 名称:  CDatabaseManager::GetLastImginfoAllCam
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   vector<Tmtv_ImageInfo> & mimgVec  //
	//************************************
	bool GetLastImginfoAllCam(vector<Tmtv_ImageInfo> &mimgVec);

	//************************************
	// 作用:  添加用户
	// 说明:  
	// 名称:  CDatabaseManager::AddUserinfo
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   Tmt_UserInfo & mUser  //
	//************************************
	bool AddUserinfo(Tmt_UserInfo& mUser);

	//************************************
	// 作用:  检查用户名是否存在
	// 说明:  
	// 名称:  CDatabaseManager::CheckUserName
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   Tmt_UserInfo & mUser  //
	//************************************
	bool CheckUserName(Tmt_UserInfo& mUser);

	//************************************
	// 作用:  依用户名更新用户信息
	// 说明:  
	// 名称:  CDatabaseManager::UpdateUserinfo
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   Tmt_UserInfo & mUser  //
	//************************************
	bool UpdateUserinfo(Tmt_UserInfo& mUser);
	
	//************************************
	// 作用:  依用户名获取用户信息
	// 说明:  
	// 名称:  CDatabaseManager::GetUserinfo
	// Access:    public 
	// 返回值:   bool  // 
	// 参数:   Tmt_UserInfo & mUser  //
	//************************************
	bool GetUserinfo(Tmt_UserInfo& mUser);

public://成员函数，常用
	bool LoadSetting();
	bool SaveSetting();

public://消息函数
	bool ResponseAsk(Tmtv_BaseNetMessage &msg, int mType);

public://线程函数
	void Task();




private://私有函数
	bool GetYearMonth(char* pData, int Datalength);
	
	int GetDefectsPosFromStr(char* str,int DfPos[][8]);
	bool CreatImgTable();
private://私有变量
	MYSQL m_mysql;
	int m_DbStatus;
	vector<Tmt_ClientInfo> m_ClientInfoVec;
	SendServer m_SendServer;
	ReceiveServer m_ReceiveServer;
	DbManagerSetting m_Setting;








public://枚举量
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