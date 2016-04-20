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
	// 返回值:   int  // 
	// 参数:   Tmtv_CameraInfo & mCam  //
	//************************************
	int AddCamToDb(Tmtv_CameraInfo& mCam);
	//************************************
	// 作用:  检查相机是否存在，返回枚举值
	// 说明:  
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
	// 作用:  获取指定ID的最新图片
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



public://线程函数
	void Task();

private://私有函数
	bool GetYearMonth(char* pData, int Datalength);
	bool GetClientInfoFrmDb();
	int GetDefectsPosFromStr(char* str,int DfPos[][8]);
	bool CreatImgTable(int CamId);
private://私有变量
	MYSQL m_mysql;
	int m_DbStatus;
	vector<Tmt_ClientInfo> m_ClientInfoVec;










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