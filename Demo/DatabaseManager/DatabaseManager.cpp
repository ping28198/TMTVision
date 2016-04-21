#include "stdafx.h"
#include "DatabaseManager.h"

CDatabaseManager::CDatabaseManager()
{
	m_DbStatus = DB_DISCONNECT;
}

CDatabaseManager::~CDatabaseManager()
{
	
}
void CDatabaseManager::Initial()
{
	InitializeCriticalSection(&m_section);
	mysql_init(&m_mysql);
	ConnectDb();
	//DisConnectDb();
}


bool CDatabaseManager::ConnectDb()
{
	bool IsOk = mysql_real_connect(&m_mysql, LOCAL_HOST, "root", "bkdtmt", "tmt_database", 3306, NULL, 0);
	if (IsOk)
	{
		DbServerLogger::mLogger.TraceInfo("链接数据库成功！");
		m_DbStatus = DB_CONNECT;
	}
	else
	{
		DbServerLogger::mLogger.TraceInfo("链接数据库失败！");
		m_DbStatus = DB_DISCONNECT;
		return false;
	}
	return true;
}

bool CDatabaseManager::DisConnectDb()
{
	mysql_close(&m_mysql);
	DbServerLogger::mLogger.TraceInfo("关闭数据库！");
	m_DbStatus = DB_DISCONNECT;
	return true;
}

int CDatabaseManager::AddCamToDb(Tmtv_CameraInfo &mCam)
{
	MYSQL_RES *res=NULL;
	MYSQL_ROW row;
	MYSQL_FIELD *mfield=NULL;
	unsigned long cols;
	unsigned long rows;
	char sqlcommand[256];
	char a[256] = { 0 };
	//正式版添加
	int rt = -1;
	sprintf(sqlcommand, "select * from %s where %s = '%s' and %s = '%s' and %s = '%s'", TMT_DB_TABLE_CAM,
		TMT_DB_CAM_CAMNAME, mCam.CameraName, TMT_DB_CAM_FOLDER, mCam.CameraPath, TMT_DB_CAM_IPADDR, mCam.CameraHost);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	if (IsOk==0)
	{
		res = mysql_store_result(&m_mysql);
		if (res)
		{
			row = mysql_fetch_row(res);
			while (row)
			{
				rt = atoi(row[0]);
				row = mysql_fetch_row(res);
			}
		}
	}
	if (rt>0)
	{
		if (res != NULL) mysql_free_result(res);
		return rt;
	}
	sprintf(sqlcommand, "insert into camera (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)"
		" values('%d','%s','%s','%s','%f','%f','%d','%d','%s','%d')",
		TMT_DB_CAM_CAMNUM,TMT_DB_CAM_CAMNAME,TMT_DB_CAM_FOLDER,TMT_DB_CAM_IPADDR,TMT_DB_CAM_LOCATION_LAT,TMT_DB_CAM_LOCATION_LNG,
		TMT_DB_CAM_WARNINGLEVEL,TMT_DB_CAM_CAMSTATUS,TMT_DB_CAM_ALGO_MASK,TMT_DB_CAM_ALGO_STATUS,
		mCam.Indexnum,mCam.CameraName,mCam.CameraPath,mCam.CameraHost,mCam.CameraPos[0],mCam.CameraPos[1],
		mCam.AlgorithmInfo.WarnningLevel,mCam.Status,mCam.AlgorithmInfo.MaskImgPath,mCam.AlgorithmInfo.WarnningLevel);
	
	IsOk = mysql_query(&m_mysql, sqlcommand);
	int b = mysql_errno(&m_mysql);
	if (IsOk!=0)
	{
		DbServerLogger::mLogger.TraceWarning("添加相机出错!");
		m_DbStatus = DB_ERROR;
		return -1;
	}
	sprintf(sqlcommand, "select * from %s where %s='%s'",TMT_DB_TABLE_CAM, TMT_DB_CAM_CAMNAME,mCam.CameraName);
	IsOk = mysql_query(&m_mysql, sqlcommand);

	if (IsOk==0)
	{
		res = mysql_store_result(&m_mysql);
		if (res)
		{
			row = mysql_fetch_row(res);
			while (row)
			{
				rt = atoi(row[0]);
				row = mysql_fetch_row(res);
			}
		}
	}
	if (res != NULL) mysql_free_result(res);
	return rt;
}
int CDatabaseManager::CheckCamInfo(Tmtv_CameraInfo& mCam)
{
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	MYSQL_FIELD *mfield = NULL;
	unsigned long cols;
	unsigned long rows;
	char sqlcommand[256];
	//sprintf(sqlcommand, "select * from %s where %s = %d",TMT_DB_TABLE_CAM,TMT_DB_CAM_ID,mCam.Indexnum);
	//mysql_query(&m_mysql, sqlcommand);
	//res = mysql_store_result(&m_mysql);
	//if (res)
	//{
	//	if (mysql_num_rows(res) != 0)
	//	{
	//		mysql_free_result(res);
	//		LeaveCriticalSection(&m_section);
	//		return CheckStatus::CamIndexExist;
	//	}
	//}
	//int b = mysql_errno(&m_mysql);
	//if(res!=NULL) mysql_free_result(res);
	sprintf(sqlcommand, "select * from %s where %s = '%s'", TMT_DB_TABLE_CAM,TMT_DB_CAM_CAMNAME, mCam.CameraName);
	mysql_query(&m_mysql, sqlcommand);
	res = mysql_store_result(&m_mysql);
	if (res)
	{
		if (mysql_num_rows(res) != 0)
		{
			mysql_free_result(res);
			LeaveCriticalSection(&m_section);
			return CheckStatus::CamIndexExist;
		}
	}
	if (res != NULL) mysql_free_result(res);
	sprintf(sqlcommand, "select * from %s where %s = '%s' and %s = '%s'", 
		TMT_DB_TABLE_CAM,TMT_DB_CAM_IPADDR, mCam.CameraHost, TMT_DB_CAM_FOLDER, mCam.CameraPath);
						 
	mysql_query(&m_mysql, sqlcommand);
	res = mysql_store_result(&m_mysql);
	if (res)
	{
		if (mysql_num_rows(res) != 0)
		{
			mysql_free_result(res);
			return CheckStatus::CamIndexExist;
		}
	}
	if (res != NULL) mysql_free_result(res);
	return CheckStatus::CamCheckOK;
}

bool CDatabaseManager::GetActiveClientInfoFrmDb(vector<Tmt_ClientInfo> &mClientVec)
{
	char sqlcommand[256];
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	Tmt_ClientInfo mClient;
	sprintf(sqlcommand, "select * from %s where %s > 0",TMT_DB_TABLE_CLIENT,TMT_DB_CLIENT_STATUS);
	mysql_query(&m_mysql, sqlcommand);
	res = mysql_store_result(&m_mysql);
	if (res)
	{
		mClientVec.swap(vector<Tmt_ClientInfo>());
		row = mysql_fetch_row(res);
		while (row)
		{
			sprintf(mClient.mIpAddr, row[1]);
			mClient.mport = atoi(row[2]);
			mClient.status = atoi(row[3]);
			mClientVec.push_back(mClient);
			row = mysql_fetch_row(res);
		}
	}
	if (res != NULL) mysql_free_result(res);
	return true;
}

bool CDatabaseManager::GetAllCamInfoFrmDb(vector<Tmtv_CameraInfo> &mCamVec)
{
	char sqlcommand[256];
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	Tmtv_CameraInfo mCam;
	sprintf(sqlcommand, "select * from %s where 1=1", TMT_DB_TABLE_CAM);
	mysql_query(&m_mysql, sqlcommand);
	res = mysql_use_result(&m_mysql);
	if (res)
	{
		mCamVec.swap(vector<Tmtv_CameraInfo>());
		row = mysql_fetch_row(res);
		while (row)
		{
			mCam.Indexnum = atoi(row[0]);
			strcpy(mCam.CameraName, row[2]);
			strcpy(mCam.CameraPath, row[3]);
			strcpy(mCam.CameraHost, row[4]);
			_CRT_DOUBLE crt_dbl;
			_atodbl(&crt_dbl, row[5]);
			mCam.CameraPos[0] = crt_dbl.x;
			_atodbl(&crt_dbl, row[6]);
			mCam.CameraPos[1] = crt_dbl.x;
			mCam.AlgorithmInfo.WarnningLevel = atoi(row[7]);
			mCam.Status = atoi(row[8]);
			strcpy(mCam.AlgorithmInfo.MaskImgPath, row[9]);
			mCam.AlgorithmInfo.mAlgoStatus = atoi(row[10]);
			mCamVec.push_back(mCam);
			row = mysql_fetch_row(res);
		}
	}
	return true;
}

bool CDatabaseManager::GetRunningCamFrmDb(vector<Tmtv_CameraInfo> &mCamVec)
{
	char sqlcommand[256];
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	Tmtv_CameraInfo mCam;
	sprintf(sqlcommand, "select * from %s where %s = %d", TMT_DB_TABLE_CAM,TMT_DB_CAM_CAMSTATUS,Tmtv_CameraInfo::TMTV_RUNNINGCAM);
	mysql_query(&m_mysql, sqlcommand);
	res = mysql_use_result(&m_mysql);
	if (res)
	{
		mCamVec.swap(vector<Tmtv_CameraInfo>());
		row = mysql_fetch_row(res);
		while (row)
		{
			mCam.Indexnum = atoi(row[0]);
			strcpy(mCam.CameraName, row[2]);
			strcpy(mCam.CameraPath, row[3]);
			strcpy(mCam.CameraHost, row[4]);
			_CRT_DOUBLE crt_dbl;
			_atodbl(&crt_dbl, row[5]);
			mCam.CameraPos[0] = crt_dbl.x;
			_atodbl(&crt_dbl, row[6]);
			mCam.CameraPos[1] = crt_dbl.x;
			mCam.AlgorithmInfo.WarnningLevel = atoi(row[7]);
			mCam.Status = atoi(row[8]);
			strcpy(mCam.AlgorithmInfo.MaskImgPath, row[9]);
			mCam.AlgorithmInfo.mAlgoStatus = atoi(row[10]);
			mCamVec.push_back(mCam);
			row = mysql_fetch_row(res);
		}
	}
	return true;
}

bool CDatabaseManager::GetLastImginfoAllCam(vector<Tmtv_ImageInfo> &mimgVec)
{
	vector<Tmtv_CameraInfo> mCamVec;
	GetRunningCamFrmDb(mCamVec);
	vector<Tmtv_CameraInfo>::iterator it;
	Tmtv_ImageInfo mImg;
	mimgVec.swap(vector<Tmtv_ImageInfo>());
	for (it = mCamVec.begin(); it != mCamVec.end();it++)
	{
		mImg.mCamId = it->Indexnum;
		if (GetLastImgInfoFrmDb(mImg))
		{
			mimgVec.push_back(mImg);
		}
	}
	return true;
}

bool CDatabaseManager::AddUserinfo(Tmt_UserInfo &mUser)
{
	if (CheckUserName(mUser)) return false;
	char sqlcommand[256];
	sprintf(sqlcommand, "insert into %s(%s,%s,%s,%s,%s) values(%s,%s,%s,%s,%d)", TMT_DB_TABLE_USER,
		TMT_DB_USER_NAME, TMT_DB_USER_PASSWORD, TMT_DB_USER_CAM_WATCH, TMT_DB_USER_WECHARD_ID, TMT_DB_USER_AUTHORYLV,
		mUser.UserName, mUser.PassWord, mUser.mCamWatch, mUser.mWeChat_id, mUser.AuthorityLevel);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	return !IsOk;
}

bool CDatabaseManager::CheckUserName(Tmt_UserInfo& mUser)
{
	char sqlcommand[256];
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	sprintf(sqlcommand, "select * from %s where %s = %s", TMT_DB_TABLE_USER, TMT_DB_USER_NAME, mUser.UserName);
	mysql_query(&m_mysql, sqlcommand);
	res = mysql_store_result(&m_mysql);
	if (res)
	{
		row = mysql_fetch_row(res);
		if (row)
		{
			return true;
		}
	}
	return false;
}

bool CDatabaseManager::UpdateUserinfo(Tmt_UserInfo& mUser)
{
	char sqlcommand[512];
	sprintf(sqlcommand, "update %s set %s = '%s',%s = '%d',%s = '%s',%s = '%s',%s = '%s' where %s = '%s'", TMT_DB_TABLE_USER,
		TMT_DB_USER_PASSWORD, mUser.PassWord, TMT_DB_USER_AUTHORYLV, mUser.AuthorityLevel,
		TMT_DB_USER_CAM_WATCH,mUser.mCamWatch, TMT_DB_USER_WECHARD_ID,mUser.mWeChat_id,
		TMT_DB_USER_NAME,mUser.UserName);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	return !IsOk;
}

bool CDatabaseManager::GetUserinfo(Tmt_UserInfo& mUser)
{
	char sqlcommand[512];
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	sprintf(sqlcommand, "select * from %s where %s = '%s'", TMT_DB_USER_ID, TMT_DB_USER_NAME, mUser.UserName);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	res = mysql_store_result(&m_mysql);
	if (res)
	{
		row = mysql_fetch_row(res);
		if (row)
		{
			sprintf(mUser.PassWord, row[2]);
			if (row[3]) sprintf(mUser.mCamWatch, row[3]);
			if (row[4]) sprintf(mUser.mWeChat_id, row[4]);
			if (row[5]) mUser.AuthorityLevel = atoi(row[5]);
			if(res) mysql_free_result(res);
			return true;
		}
	}
	return false;
}

int CDatabaseManager::GetDefectsPosFromStr(char* str, int DfPos[][8])
{
	char *pos = str;
	char *pos1 = strchr(pos, ',');
	char mstr[16] = { 0 };
	int length = 0;
	int i = 0;
	while (pos1)
	{
		length = (pos1 - pos > 15) ? 15 : (pos1 - pos);
		memcpy_s(mstr, 16, pos, length);
		mstr[length] = 0;
		DfPos[i / 4][i % 4] = atoi(mstr);
		i++;
		pos = pos1 + 1;
		pos1 = strchr(pos, ',');
	}
	return i/4;
}

bool CDatabaseManager::InsertClientInfoToDb(const Tmt_ClientInfo & mClientInfo)
{
	char sqlcommand[256];
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	Tmt_ClientInfo mClient;
	sprintf(sqlcommand, "select * from %s where %s = '%s' and %s = '%d'",TMT_DB_TABLE_CLIENT,
		TMT_DB_CLIENT_IPADDR,mClientInfo.mIpAddr,TMT_DB_CLIENT_PORT,mClientInfo.mport);
	mysql_query(&m_mysql, sqlcommand);
	res = mysql_store_result(&m_mysql);
	if (res)
	{
		row = mysql_fetch_row(res);
		if (row)
		{
			if (res != NULL) mysql_free_result(res);
			return false;
		}
	}
	if (res != NULL) mysql_free_result(res);
	sprintf(sqlcommand, "insert into %s (%s,%s,%s) values('%s','%d','%d')",TMT_DB_TABLE_CLIENT,
		TMT_DB_CLIENT_IPADDR,TMT_DB_CLIENT_PORT,TMT_DB_CLIENT_STATUS,
		mClientInfo.mIpAddr,mClientInfo.mport,mClientInfo.status);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	if (IsOk!=0)
	{
		DbServerLogger::mLogger.TraceWarning("插入客户端信息到数据库失败！");
	}
	return true;
}

bool CDatabaseManager::UpdateClientStatusDb(const Tmt_ClientInfo & mClientInfo)
{
	
	char sqlcommand[256];
	sprintf(sqlcommand, "update %s set %s = '%d' where %s = '%s' and %s = '%d'",TMT_DB_TABLE_CLIENT,
		TMT_DB_CLIENT_STATUS, mClientInfo.status, TMT_DB_CLIENT_IPADDR, mClientInfo.mIpAddr, TMT_DB_CLIENT_PORT, mClientInfo.mport);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	return !IsOk;
}

bool CDatabaseManager::InsertImgInfoToDb(Tmtv_ImageInfo& mImginfo)
{
	char sqlcommand[512] = {0};
	char defectpos[256] = {0};
	int IsOk = 0;
	char mdate[64] = { 0 };
	GetYearMonth(mdate, 64);
	char tablename[128] = { 0 };
	sprintf(tablename, "cam%d_image_%s", mImginfo.mCamId, mdate);
	CreatImgTable(mImginfo.mCamId);
	char tmpstr[64] = { 0 };
	for (int i = 0; i < mImginfo.mDefectInfo.DefectNum; i++)
	{
		sprintf(tmpstr, "%d,%d,%d,%d,", mImginfo.mDefectInfo.DefectPos[i][0], mImginfo.mDefectInfo.DefectPos[i][1],
			mImginfo.mDefectInfo.DefectPos[i][2], mImginfo.mDefectInfo.DefectPos[i][3]);
		strcat(defectpos, tmpstr);
	}
	sprintf(sqlcommand, "insert into %s (%s,%s,%s,%s,%s,%s,%s) values('%s','%s',%d,'%s',%d,%d,%d)", tablename,
		TMT_DB_IMG_PATH, TMT_DB_IMG_GRABTIME, TMT_DB_IMG_DEFECTSNUM, TMT_DB_IMG_DEFECTSPOS, TMT_DB_IMG_HEIGHT, TMT_DB_IMG_WIDTH, TMT_DB_IMG_CAMID,
		mImginfo.ImagePath, mImginfo.GrabTime, mImginfo.mDefectInfo.DefectNum,defectpos,mImginfo.mDefectInfo.ImgHeight, mImginfo.mDefectInfo.ImgWidth, mImginfo.mCamId);
	IsOk = mysql_query(&m_mysql, sqlcommand);
	return !IsOk;
}

bool CDatabaseManager::UpdateCamInfoDb(Tmtv_CameraInfo& mCam)
{
	char sqlcommand[512];
	sprintf(sqlcommand, "update %s set %s = '%s', %s = '%s', %s = '%s', %s = '%f', %s = '%f' , %s = '%d', %s = '%d' , %s = '%s', %s = '%d' where %s = '%d'", TMT_DB_TABLE_CAM,
		TMT_DB_CAM_CAMNAME, mCam.CameraName, TMT_DB_CAM_FOLDER, mCam.CameraPath, TMT_DB_CAM_IPADDR, mCam.CameraHost,
		TMT_DB_CAM_LOCATION_LAT, mCam.CameraPos[0], TMT_DB_CAM_LOCATION_LNG, mCam.CameraPos[1], TMT_DB_CAM_WARNINGLEVEL, mCam.AlgorithmInfo.WarnningLevel,
		TMT_DB_CAM_CAMSTATUS, mCam.Status, TMT_DB_CAM_ALGO_MASK, mCam.AlgorithmInfo.MaskImgPath, TMT_DB_CAM_ALGO_STATUS, mCam.AlgorithmInfo.mAlgoStatus,
		TMT_DB_CAM_ID,mCam.Indexnum);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	if (IsOk)
	{
		return false;
	}
	return true;
}

bool CDatabaseManager::GetLastImgInfoFrmDb(Tmtv_ImageInfo& mImg)
{
	char sqlcommand[512];
	char mdate[64] = { 0 };
	GetYearMonth(mdate, 64);
	char tablename[128] = { 0 };
	char defectpos[256] = { 0 };
	sprintf(tablename, "cam%d_image_%s", mImg.mCamId, mdate);
	sprintf(sqlcommand, "select * from %s where %s = (select max(%s) from %s)",
		tablename,TMT_DB_IMG_ID, TMT_DB_IMG_ID, tablename);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	res = mysql_store_result(&m_mysql);
	if (res)
	{
		row = mysql_fetch_row(res);
		if (row)
		{
			if(row[1]) strcpy(mImg.ImagePath, row[1]);
			if (row[2]) strcpy(mImg.GrabTime, row[2]);
			if (row[4]) mImg.mDefectInfo.DefectNum = GetDefectsPosFromStr(row[4], mImg.mDefectInfo.DefectPos);
			if (row[5]) mImg.mDefectInfo.ImgWidth = atoi(row[5]);
			if (row[6]) mImg.mDefectInfo.ImgHeight = atoi(row[6]);
			if (res != NULL) mysql_free_result(res);
			return true;
		}
	}
	if (res != NULL) mysql_free_result(res);
	return false;
}

bool CDatabaseManager::GetImginfoByDate(vector<Tmtv_ImageInfo> &mimgVec, int Camid,const char* mdate)
{
	TINYSTR mdate_tmp = {0};
	char a[2] = {0};
	if (mdate == NULL) return false;
	int i = 0;
	while (mdate[i]!= '\0')
	{
		if (isalnum(mdate[i]))
		{
			a[0] = mdate[i];
			strcat(mdate_tmp,a);
		}
		i++;
		if (i==TMTV_TINYSTRLEN) break;
	}
	mdate_tmp[6] = '\0';
	char sqlcommand[512] = { 0 };
	sprintf(sqlcommand, "select * from cam%d_image_%s where %s between '%s 00:00:00' and '%s 23:59:59'", Camid, mdate_tmp,TMT_DB_IMG_GRABTIME,mdate, mdate);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	Tmtv_ImageInfo mImg;
	mimgVec.swap(vector<Tmtv_ImageInfo>());
	res = mysql_use_result(&m_mysql);
	if (res)
	{
		row = mysql_fetch_row(res);
		while (row)
		{
			if (row[1]) strcpy(mImg.ImagePath, row[1]);
			if (row[2]) strcpy(mImg.GrabTime, row[2]);
			if (row[4]) mImg.mDefectInfo.DefectNum = GetDefectsPosFromStr(row[4], mImg.mDefectInfo.DefectPos);
			if (row[5]) mImg.mDefectInfo.ImgWidth = atoi(row[5]);
			if (row[6]) mImg.mDefectInfo.ImgHeight = atoi(row[6]);
			mimgVec.push_back(mImg);
			row = mysql_fetch_row(res);
		} 
	}
	return true;
}

bool CDatabaseManager::CreatImgTable(int CamId)
{
	char sqlcommand[512] = { 0 };
	char mdate[64] = { 0 };
	GetYearMonth(mdate, 64);
	char tablename[128] = { 0 };
	sprintf(tablename, "cam%d_image_%s", CamId, mdate);
	sprintf(sqlcommand, "CREATE TABLE IF NOT EXISTS %s("
		"%s int(11) NOT NULL AUTO_INCREMENT,"
		"%s varchar(255) DEFAULT NULL,"
		"%s datetime DEFAULT NULL,"
		"%s int(11) DEFAULT NULL,"
		"%s varchar(255) DEFAULT NULL,"
		"%s double DEFAULT NULL,"
		"%s double DEFAULT NULL,"
		"%s int(11) DEFAULT NULL,"
		"PRIMARY KEY(%s))",
		tablename,
		TMT_DB_IMG_ID,
		TMT_DB_IMG_PATH,
		TMT_DB_IMG_GRABTIME,
		TMT_DB_IMG_DEFECTSNUM,
		TMT_DB_IMG_DEFECTSPOS,
		TMT_DB_IMG_WIDTH,
		TMT_DB_IMG_HEIGHT,
		TMT_DB_IMG_CAMID,
		TMT_DB_IMG_ID
		);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	return !IsOk;
}

void CDatabaseManager::Task()
{

}

bool CDatabaseManager::GetYearMonth(char* pData, int Datalength)
{
	wchar_t tmp[32];
	CCommonFunc::GetNowDate(tmp, 32);
	CCommonFunc::UnicodeToAnsi(tmp, pData, Datalength);
	pData[6] = '\0';
	return true;
}

Logger DbServerLogger::mLogger("DbServerLog\\");