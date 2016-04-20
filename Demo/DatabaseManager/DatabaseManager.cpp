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
	EnterCriticalSection(&m_section);
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
		LeaveCriticalSection(&m_section);
		return false;
	}
	LeaveCriticalSection(&m_section);
	return true;
}

bool CDatabaseManager::DisConnectDb()
{
	EnterCriticalSection(&m_section);
	mysql_close(&m_mysql);
	DbServerLogger::mLogger.TraceInfo("关闭数据库！");
	m_DbStatus = DB_DISCONNECT;
	LeaveCriticalSection(&m_section);
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
	EnterCriticalSection(&m_section);
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
		LeaveCriticalSection(&m_section);
		if (res != NULL) mysql_free_result(res);
		return rt;
	}
	sprintf(sqlcommand, "insert into camera (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)"
		" values('%d','%s','%s','%s','%d','%d','%d','%d','%s','%d')",
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
		LeaveCriticalSection(&m_section);
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
	LeaveCriticalSection(&m_section);
	return rt;
}
int CDatabaseManager::CheckCamInfo(Tmtv_CameraInfo& mCam)
{
	EnterCriticalSection(&m_section);
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
			LeaveCriticalSection(&m_section);
			return CheckStatus::CamIndexExist;
		}
	}
	if (res != NULL) mysql_free_result(res);
	LeaveCriticalSection(&m_section);
	return CheckStatus::CamCheckOK;
}

bool CDatabaseManager::GetClientInfoFrmDb()
{
	EnterCriticalSection(&m_section);
	char sqlcommand[256];
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	Tmt_ClientInfo mClient;
	sprintf(sqlcommand, "select * from %s where %s > 0",TMT_DB_TABLE_CLIENT,TMT_DB_CLIENT_STATUS);
	mysql_query(&m_mysql, sqlcommand);
	res = mysql_store_result(&m_mysql);
	if (res)
	{
		m_ClientInfoVec.clear();
		row = mysql_fetch_row(res);
		while (row)
		{
			sprintf(mClient.mIpAddr, row[1]);
			mClient.mport = atoi(row[2]);
			mClient.status = atoi(row[3]);
			row = mysql_fetch_row(res);
		}
	}
	if (res != NULL) mysql_free_result(res);
	LeaveCriticalSection(&m_section);
	return true;
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
	EnterCriticalSection(&m_section);
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
			LeaveCriticalSection(&m_section);
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
	LeaveCriticalSection(&m_section);
	return true;
}

bool CDatabaseManager::UpdateClientStatusDb(const Tmt_ClientInfo & mClientInfo)
{
	
	char sqlcommand[256];
	sprintf(sqlcommand, "update %s set %s = '%d' where %s = '%s' and %s = '%d'",TMT_DB_TABLE_CLIENT,
		TMT_DB_CLIENT_STATUS, mClientInfo.status, TMT_DB_CLIENT_IPADDR, mClientInfo.mIpAddr, TMT_DB_CLIENT_PORT, mClientInfo.mport);
	EnterCriticalSection(&m_section);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	LeaveCriticalSection(&m_section);
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
	sprintf(sqlcommand, "update %s set %s = '%s', %s = '%s', %s = '%s', %s = '%d', %s = '%d' , %s = '%d', %s = '%d' , %s = '%s', %s = '%d' where %s = '%d'", TMT_DB_TABLE_CAM,
		TMT_DB_CAM_CAMNAME, mCam.CameraName, TMT_DB_CAM_FOLDER, mCam.CameraPath, TMT_DB_CAM_IPADDR, mCam.CameraHost,
		TMT_DB_CAM_LOCATION_LAT, mCam.CameraPos[0], TMT_DB_CAM_LOCATION_LNG, mCam.CameraPos[1], TMT_DB_CAM_WARNINGLEVEL, mCam.AlgorithmInfo.WarnningLevel,
		TMT_DB_CAM_CAMSTATUS, mCam.Status, TMT_DB_CAM_ALGO_MASK, mCam.AlgorithmInfo.MaskImgPath, TMT_DB_CAM_ALGO_STATUS, mCam.AlgorithmInfo.mAlgoStatus,
		TMT_DB_CAM_ID,mCam.Indexnum);
	EnterCriticalSection(&m_section);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	LeaveCriticalSection(&m_section);
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
		}
	}
	if (res != NULL) mysql_free_result(res);
	return true;
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