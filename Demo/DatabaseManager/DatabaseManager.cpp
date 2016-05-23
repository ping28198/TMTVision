#include "stdafx.h"
#include "DatabaseManager.h"
#include "DatabaseManagerDlg.h"

CDatabaseManager::CDatabaseManager(void *pParent): m_SendServer(this), m_ReceiveServer(this)
{
	m_DbStatus = DB_DISCONNECT;
	this->pParent = (CDatabaseManagerDlg *)pParent;
}

CDatabaseManager::~CDatabaseManager()
{
	DbMDestory();
}
void CDatabaseManager::Initial()
{
	InitializeCriticalSection(&m_section);
	LoadSetting();
	mysql_init(&m_mysql);
	ConnectDb();
	//DisConnectDb();
	m_ReceiveServer.Initial(m_Setting.mRecvSetting);
	m_ReceiveServer.Create();
	m_ReceiveServer.Resume();
	m_SendServer.Initial(m_Setting.mSendSetting);
	m_SendServer.Create();
	m_SendServer.Resume();
	NetWorkResetCount = 0;
	Create();
	Resume();
	EnterCriticalSection(&m_section);
	pParent->UpdateDbData();
	LeaveCriticalSection(&m_section);
}


void CDatabaseManager::DbMDestory()
{
	SaveSetting();
	Destroy();
	m_ReceiveServer.Destroy();
	m_SendServer.Destroy();
	DisConnectDb();
}



bool CDatabaseManager::ConnectDb()
{
	bool IsOk = mysql_real_connect(&m_mysql,m_Setting.MysqlServerHost, "ren", "tmt123", "tmt_database", m_Setting.MysqlServerPort, NULL, 0);
	if (IsOk)
	{
		DbServerLogger::mLogger.TraceInfo("链接数据库成功！");
		m_DbStatus = DB_CONNECT;
	}
	else
	{
		int a = mysql_errno(&m_mysql);
		
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
	char sqlcommand[512];
	char a[512] = { 0 };
	//正式版添加
	int rt = -1;
	sprintf_s(sqlcommand, 512,"select * from %s where %s = '%s' and %s = '%s' and %s = '%s'", TMT_DB_TABLE_CAM,
		TMT_DB_CAM_CAMNAME, mCam.CameraName, TMT_DB_CAM_FOLDER, mCam.CameraPath, TMT_DB_CAM_IPADDR, mCam.CameraHost);
	
	ModifySqlCommand(sqlcommand);

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
		mCam.Indexnum = rt;
		return rt;
	}
	sprintf_s(sqlcommand,512, "insert into camera (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)"
		" values('%d','%s','%s','%s','%f','%f','%d','%d','%s','%d')",
		TMT_DB_CAM_CAMNUM,TMT_DB_CAM_CAMNAME,TMT_DB_CAM_FOLDER,TMT_DB_CAM_IPADDR,TMT_DB_CAM_LOCATION_LAT,TMT_DB_CAM_LOCATION_LNG,
		TMT_DB_CAM_WARNINGLEVEL,TMT_DB_CAM_CAMSTATUS,TMT_DB_CAM_ALGO_MASK,TMT_DB_CAM_ALGO_STATUS,
		mCam.Indexnum,mCam.CameraName,mCam.CameraPath,mCam.CameraHost,mCam.CameraPos[0],mCam.CameraPos[1],
		mCam.AlgorithmInfo.WarnningLevel,mCam.Status,mCam.AlgorithmInfo.MaskImgPath,mCam.AlgorithmInfo.WarnningLevel);
	
	ModifySqlCommand(sqlcommand);
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
	mCam.Indexnum = rt;
	return rt;
}

bool CDatabaseManager::DelCamFromDB(int mCamID)
{
	char sqlcommand[256];
	sprintf_s(sqlcommand, 256, "select * from %s where %s = '%d'", TMT_DB_TABLE_CAM, TMT_DB_CAM_ID, mCamID);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	if (IsOk==0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int CDatabaseManager::CheckCamInfo(Tmtv_CameraInfo& mCam)
{
	MYSQL_RES *res = NULL;
	MYSQL_FIELD *mfield = NULL;
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
	sprintf_s(sqlcommand,256, "select * from %s where %s = '%s'", TMT_DB_TABLE_CAM,TMT_DB_CAM_CAMNAME, mCam.CameraName);
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
	sprintf_s(sqlcommand, 256,"select * from %s where %s = '%s' and %s = '%s'", 
		TMT_DB_TABLE_CAM,TMT_DB_CAM_IPADDR, mCam.CameraHost, TMT_DB_CAM_FOLDER, mCam.CameraPath);
	ModifySqlCommand(sqlcommand);
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
	sprintf_s(sqlcommand,256, "select * from %s where %s > 0",TMT_DB_TABLE_CLIENT,TMT_DB_CLIENT_STATUS);
	mysql_query(&m_mysql, sqlcommand);
	res = mysql_store_result(&m_mysql);
	if (res)
	{
		mClientVec.swap(vector<Tmt_ClientInfo>());
		row = mysql_fetch_row(res);
		while (row)
		{
			mClient.ClientID = atoi(row[0]);
			sprintf_s(mClient.mIpAddr,TMTV_IPSTRLEN, row[1]);
			mClient.mport = atoi(row[2]);
			mClient.status = atoi(row[3]);
			mClientVec.push_back(mClient);
			row = mysql_fetch_row(res);
		}
	}
	if (res != NULL) mysql_free_result(res);
	return true;
}

bool CDatabaseManager::GetAllClientInfoFrmDb(vector<Tmt_ClientInfo> &mClientVec)
{
	mClientVec.clear();
	char sqlcommand[256];
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	Tmt_ClientInfo mClient;
	sprintf_s(sqlcommand, 256, "select * from %s where 1=1", TMT_DB_TABLE_CLIENT);
	mysql_query(&m_mysql, sqlcommand);
	res = mysql_store_result(&m_mysql);
	if (res)
	{
		mClientVec.swap(vector<Tmt_ClientInfo>());
		row = mysql_fetch_row(res);
		while (row)
		{
			mClient.ClientID = atoi(row[0]);
			sprintf_s(mClient.mIpAddr, TMTV_IPSTRLEN, row[1]);
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
	sprintf_s(sqlcommand,256, "select * from %s where 1=1", TMT_DB_TABLE_CAM);
	mysql_query(&m_mysql, sqlcommand);
	res = mysql_use_result(&m_mysql);
	if (res)
	{
		mCamVec.swap(vector<Tmtv_CameraInfo>());
		row = mysql_fetch_row(res);
		while (row)
		{
			mCam.Indexnum = atoi(row[0]);
			strcpy_s(mCam.CameraName, TMTV_SHORTSTRLEN,row[2]);
			strcpy_s(mCam.CameraPath,TMTV_PATHSTRLEN, row[3]);
			strcpy_s(mCam.CameraHost,TMTV_IPSTRLEN, row[4]);
			_CRT_DOUBLE crt_dbl;
			_atodbl(&crt_dbl, row[5]);
			mCam.CameraPos[0] = crt_dbl.x;
			_atodbl(&crt_dbl, row[6]);
			mCam.CameraPos[1] = crt_dbl.x;
			mCam.AlgorithmInfo.WarnningLevel = atoi(row[7]);
			mCam.Status = atoi(row[8]);
			strcpy_s(mCam.AlgorithmInfo.MaskImgPath,TMTV_PATHSTRLEN, row[9]);
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
	sprintf_s(sqlcommand,256, "select * from %s where %s = %d", TMT_DB_TABLE_CAM,TMT_DB_CAM_CAMSTATUS,Tmtv_CameraInfo::TMTV_RUNNINGCAM);
	mysql_query(&m_mysql, sqlcommand);
	res = mysql_use_result(&m_mysql);
	if (res)
	{
		mCamVec.swap(vector<Tmtv_CameraInfo>());
		row = mysql_fetch_row(res);
		while (row)
		{
			mCam.Indexnum = atoi(row[0]);
			strcpy_s(mCam.CameraName,TMTV_SHORTSTRLEN, row[2]);
			strcpy_s(mCam.CameraPath,TMTV_PATHSTRLEN, row[3]);
			strcpy_s(mCam.CameraHost,TMTV_IPSTRLEN, row[4]);
			_CRT_DOUBLE crt_dbl;
			_atodbl(&crt_dbl, row[5]);
			mCam.CameraPos[0] = crt_dbl.x;
			_atodbl(&crt_dbl, row[6]);
			mCam.CameraPos[1] = crt_dbl.x;
			mCam.AlgorithmInfo.WarnningLevel = atoi(row[7]);
			mCam.Status = atoi(row[8]);
			strcpy_s(mCam.AlgorithmInfo.MaskImgPath, TMTV_PATHSTRLEN,row[9]);
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
	sprintf_s(sqlcommand,256, "insert into %s(%s,%s,%s,%s,%s) values(%s,%s,%s,%s,%d)", TMT_DB_TABLE_USER,
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
	sprintf_s(sqlcommand,256, "select * from %s where %s = %s", TMT_DB_TABLE_USER, TMT_DB_USER_NAME, mUser.UserName);
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
	sprintf_s(sqlcommand,512, "update %s set %s = '%s',%s = '%d',%s = '%s',%s = '%s' where %s = '%s'", TMT_DB_TABLE_USER,
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
	sprintf_s(sqlcommand,512, "select * from %s where %s = '%s'", TMT_DB_USER_ID, TMT_DB_USER_NAME, mUser.UserName);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	res = mysql_store_result(&m_mysql);
	if (res)
	{
		row = mysql_fetch_row(res);
		if (row)
		{
			sprintf_s(mUser.PassWord, TMTV_LONGSTRLEN,row[2]);
			if (row[3]) sprintf(mUser.mCamWatch, row[3]);
			if (row[4]) sprintf(mUser.mWeChat_id, row[4]);
			if (row[5]) mUser.AuthorityLevel = atoi(row[5]);
			if(res) mysql_free_result(res);
			return true;
		}
	}
	return false;
}

bool CDatabaseManager::LoadSetting()
{

	Config mCfg;
	try
	{
		mCfg.ReadFile("setting\\DbmanagerSetting.ini");
	}
	catch (Config::File_not_found m)
	{
		DbServerLogger::mLogger.TraceWarning("打开配置文件%s失败，将使用默认配置！", m.filename.c_str());
	}
	NetIP netip;
	TmtSocket::GetAvailableNetIP(netip);
	string mstr = mCfg.Read("SendIp", string(netip));
	strcpy_s(m_Setting.mSendSetting.m_LocalSendIP, TMTV_IPSTRLEN, mstr.c_str());
	mstr = mCfg.Read("RecvIp", string(netip));
	strcpy_s(m_Setting.mRecvSetting.m_LocalRecvIP, TMTV_IPSTRLEN, mstr.c_str());
	mstr = mCfg.Read("MysqlServerHost", string("Localhost"));
	strcpy_s(m_Setting.MysqlServerHost, TMTV_IPSTRLEN, mstr.c_str());
	m_Setting.MysqlServerPort = mCfg.Read("MysqlServerPort", 3306);
	m_Setting.m_Sleeptime = mCfg.Read("SleepTime", 0);
	m_Setting.mRecvSetting.m_LocalRecvPort = mCfg.Read("RecvPort", TMT_DBSERVER_RECVPORT);
	m_Setting.mSendSetting.m_LocalSendPort = mCfg.Read("SendPort", TMT_DBSERVER_SENDPORT);
	return true;
}

bool CDatabaseManager::SaveSetting()
{
	Config mCfg;
	try
	{
		mCfg.ReadFile("setting\\DbmanagerSetting.ini");
	}
	catch (Config::File_not_found m)
	{
		DbServerLogger::mLogger.TraceWarning("配置文件%s不存在，将创建配置文件！", m.filename.c_str());
	}
	mCfg.Add("SendIp", string(m_Setting.mSendSetting.m_LocalSendIP));
	mCfg.Add("RecvIp", string(m_Setting.mRecvSetting.m_LocalRecvIP));
	mCfg.Add("SleepTime", m_Setting.m_Sleeptime);
	mCfg.Add("RecvPort", m_Setting.mRecvSetting.m_LocalRecvPort);
	mCfg.Add("SendPort", m_Setting.mSendSetting.m_LocalSendPort);
	mCfg.Add("MysqlServerPort", m_Setting.MysqlServerPort);
	mCfg.Add("MysqlServerHost", m_Setting.MysqlServerHost);

	//string mstr = string(m_Setting.m_SendIp);
	return true;
}

bool CDatabaseManager::ResponseAsk(MessageItem &mMsgItem,Tmtv_BaseNetMessage &src_msg, int mType)
{
	Tmtv_BaseNetMessage mSendMsg;
	mSendMsg.hDstHandle = src_msg.hSrcHandle;
	mSendMsg.MsgType = mType;
	strcpy_s(mSendMsg.dstAddr, TMTV_IPSTRLEN, src_msg.mAddr);
	mSendMsg.dstPort = src_msg.mPort;
	strcpy_s(mSendMsg.mAddr, TMTV_IPSTRLEN, m_Setting.mRecvSetting.m_LocalRecvIP);
	mSendMsg.mPort = m_Setting.mRecvSetting.m_LocalRecvPort;
	memcpy_s(mMsgItem.p_Buffer, MessageItem::MAXMSGSIZE, &mSendMsg, sizeof(Tmtv_BaseNetMessage));
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
	char sqlcommand[256];
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	sprintf_s(sqlcommand, 256,"select * from %s where %s = '%s' and %s = '%d'",TMT_DB_TABLE_CLIENT,
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
	sprintf_s(sqlcommand,256, "insert into %s (%s,%s,%s) values('%s','%d','%d')",TMT_DB_TABLE_CLIENT,
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
	sprintf_s(sqlcommand,256, "update %s set %s = '%d' where %s = '%s' and %s = '%d'",TMT_DB_TABLE_CLIENT,
		TMT_DB_CLIENT_STATUS, mClientInfo.status, TMT_DB_CLIENT_IPADDR, mClientInfo.mIpAddr, TMT_DB_CLIENT_PORT, mClientInfo.mport);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	return !IsOk;
}

bool CDatabaseManager::InsertImgInfoToDb(Tmtv_ImageInfo& mImginfo)
{
	char sqlcommand[512] = {0};
	LONGSTR defectpos = {0};
	int IsOk = 0;
	char mdate[64] = { 0 };
	GetYearMonth(mdate, 64);
	char tablename[128] = { 0 };
	sprintf_s(tablename,128, "image_%s", mdate);
	CreatImgTable();
	//char tmpstr[64] = { 0 };
	//for (int i = 0; i < mImginfo.mDefectInfo.DefectNum; i++)
	//{
	//	sprintf_s(tmpstr, 64,"%d,%d,%d,%d,", mImginfo.mDefectInfo.DefectPos[i][0], mImginfo.mDefectInfo.DefectPos[i][1],
	//		mImginfo.mDefectInfo.DefectPos[i][2], mImginfo.mDefectInfo.DefectPos[i][3]);
	//	strcat_s(defectpos, 256,tmpstr);
	//}//使用json
	ConvertDefectsToJson(defectpos, TMTV_LONGSTRLEN, mImginfo.mDefectInfo.DefectPos, mImginfo.mDefectInfo.DefectNum);
	sprintf_s(sqlcommand,512, "insert into %s (%s,%s,%s,%s,%s,%s,%s) values('%s','%s',%d,'%s',%d,%d,%d)", tablename,
		TMT_DB_IMG_PATH, TMT_DB_IMG_GRABTIME, TMT_DB_IMG_DEFECTSNUM, TMT_DB_IMG_DEFECTSPOS, TMT_DB_IMG_HEIGHT, TMT_DB_IMG_WIDTH, TMT_DB_IMG_CAMID,
		mImginfo.ImagePath, mImginfo.GrabTime, mImginfo.mDefectInfo.DefectNum,defectpos,mImginfo.mDefectInfo.ImgHeight, mImginfo.mDefectInfo.ImgWidth, mImginfo.mCamId);
	ModifySqlCommand(sqlcommand);
	IsOk = mysql_query(&m_mysql, sqlcommand);
	return !IsOk;
}

bool CDatabaseManager::GetCaminfoFromDb(Tmtv_CameraInfo& mCam)
{
	char sqlcommand[512] = { 0 };
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	sprintf_s(sqlcommand, 256, "select * from %s where %s = '%d'", TMT_DB_TABLE_CAM, TMT_DB_CAM_ID, mCam.Indexnum);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	res = mysql_store_result(&m_mysql);
	if (res)
	{
		row = mysql_fetch_row(res);
		if (row)
		{
			strcpy_s(mCam.CameraName, TMTV_SHORTSTRLEN, row[2]);
			strcpy_s(mCam.CameraPath, TMTV_PATHSTRLEN, row[3]);
			strcpy_s(mCam.CameraHost, TMTV_IPSTRLEN, row[4]);
			_CRT_DOUBLE crt_dbl;
			_atodbl(&crt_dbl, row[5]);
			mCam.CameraPos[0] = crt_dbl.x;
			_atodbl(&crt_dbl, row[6]);
			mCam.CameraPos[1] = crt_dbl.x;
			mCam.AlgorithmInfo.WarnningLevel = atoi(row[7]);
			mCam.Status = atoi(row[8]);
			strcpy_s(mCam.AlgorithmInfo.MaskImgPath, TMTV_PATHSTRLEN, row[9]);
			mCam.AlgorithmInfo.mAlgoStatus = atoi(row[10]);
			return true;
		}
	}
	return false;
}

bool CDatabaseManager::UpdateCamInfoDb(Tmtv_CameraInfo& mCam)
{
	char sqlcommand[512];
	sprintf_s(sqlcommand,512, "update %s set %s = '%s', %s = '%s', %s = '%s', %s = '%f', %s = '%f' , %s = '%d', %s = '%d' , %s = '%s', %s = '%d' where %s = '%d'", TMT_DB_TABLE_CAM,
		TMT_DB_CAM_CAMNAME, mCam.CameraName, TMT_DB_CAM_FOLDER, mCam.CameraPath, TMT_DB_CAM_IPADDR, mCam.CameraHost,
		TMT_DB_CAM_LOCATION_LAT, mCam.CameraPos[0], TMT_DB_CAM_LOCATION_LNG, mCam.CameraPos[1], TMT_DB_CAM_WARNINGLEVEL, mCam.AlgorithmInfo.WarnningLevel,
		TMT_DB_CAM_CAMSTATUS, mCam.Status, TMT_DB_CAM_ALGO_MASK, mCam.AlgorithmInfo.MaskImgPath, TMT_DB_CAM_ALGO_STATUS, mCam.AlgorithmInfo.mAlgoStatus,
		TMT_DB_CAM_ID,mCam.Indexnum);
	ModifySqlCommand(sqlcommand);
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
	sprintf_s(tablename,64, "image_%s",mdate);
	//sprintf_s(sqlcommand,512, "select * from %s where %s = (select max(%s) from %s)",
	//	tablename,TMT_DB_IMG_ID, TMT_DB_IMG_ID, tablename);
	sprintf_s(sqlcommand, 512, "select * from %s where %s = '%d' order by %s desc limit 1",
		tablename, TMT_DB_IMG_CAMID, mImg.mCamId, TMT_DB_IMG_ID);
	int IsOk = mysql_query(&m_mysql, sqlcommand);
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	res = mysql_store_result(&m_mysql);
	if (res)
	{
		row = mysql_fetch_row(res);
		if (row)
		{
			if(row[1]) strcpy_s(mImg.ImagePath,TMTV_PATHSTRLEN, row[1]);
			if (row[2]) strcpy_s(mImg.GrabTime, TMTV_TINYSTRLEN, row[2]);
			if (row[4]) mImg.mDefectInfo.DefectNum = GetDefectsPosFromJson(mImg.mDefectInfo.DefectPos, row[4], 0);//GetDefectsPosFromStr(row[4], mImg.mDefectInfo.DefectPos);//使用json
			if (row[5]) mImg.mDefectInfo.ImgWidth = atoi(row[5]);
			if (row[6]) mImg.mDefectInfo.ImgHeight = atoi(row[6]);
			if (res != NULL) mysql_free_result(res);
			return true;
		}
	}
	if (res != NULL) mysql_free_result(res);
	return false;
}

bool CDatabaseManager::GetImginfoByDate(vector<Tmtv_ImageInfo> &mimgVec, int Camid,const char* Date)
{
	if (Date == NULL) return false;
	SHORTSTR mdate = { 0 };
	int j = 0;
	for (size_t i = 0; i < strlen(Date); i++)
	{
		if (isdigit(Date[i]) != 0)
		{
			mdate[j] = Date[i];
			j++;
		}
	}
	mdate[8] = 0;
	char sqlcommand[512] = { 0 };
	sprintf_s(sqlcommand,512, "select * from image_%s where %s = '%d'", mdate,TMT_DB_IMG_CAMID,Camid);
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
			if (row[1]) strcpy_s(mImg.ImagePath, TMTV_PATHSTRLEN, row[1]);
			if (row[2]) strcpy_s(mImg.GrabTime, TMTV_TINYSTRLEN,row[2]);
			if (row[4]) mImg.mDefectInfo.DefectNum = GetDefectsPosFromJson(mImg.mDefectInfo.DefectPos, row[4], 0);//GetDefectsPosFromStr(row[4], mImg.mDefectInfo.DefectPos);//使用json
			if (row[5]) mImg.mDefectInfo.ImgWidth = atoi(row[5]);
			if (row[6]) mImg.mDefectInfo.ImgHeight = atoi(row[6]);
			mimgVec.push_back(mImg);
			row = mysql_fetch_row(res);
		} 
	}
	return true;
}

bool CDatabaseManager::GetImginfoByTime(vector<Tmtv_ImageInfo> &mimgVec, int Camid, const char* mtime_old, const char* mtime_new)
{
	SHORTSTR mdate = {0};
	int j = 0;
	for (size_t i = 0; i < strlen(mtime_new) ; i++)
	{
		if (isdigit(mtime_new[i])!=0)
		{
			mdate[j] = mtime_new[i];
			j++;
		}
	}
	mdate[8] = 0;
	char sqlcommand[512] = { 0 };
	sprintf_s(sqlcommand, 512, "select * from image_%s where %s = '%d' and %s between '%s' and '%s'", mdate, 
		TMT_DB_IMG_CAMID, Camid,TMT_DB_IMG_GRABTIME,mtime_old,mtime_new);
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
			if (row[1]) strcpy_s(mImg.ImagePath, TMTV_PATHSTRLEN, row[1]);
			if (row[2]) strcpy_s(mImg.GrabTime, TMTV_TINYSTRLEN, row[2]);
			if (row[4]) mImg.mDefectInfo.DefectNum = GetDefectsPosFromJson(mImg.mDefectInfo.DefectPos, row[4], 0);//GetDefectsPosFromStr(row[4], mImg.mDefectInfo.DefectPos);//使用json
			if (row[5]) mImg.mDefectInfo.ImgWidth = atoi(row[5]);
			if (row[6]) mImg.mDefectInfo.ImgHeight = atoi(row[6]);
			mimgVec.push_back(mImg);
			row = mysql_fetch_row(res);
		}
	}
	return true;
}

bool CDatabaseManager::CreatImgTable()
{
	char sqlcommand[512] = { 0 };
	char mdate[64] = { 0 };
	GetYearMonth(mdate, 64);
	char tablename[128] = { 0 };
	sprintf_s(tablename,64, "image_%s", mdate);
	sprintf_s(sqlcommand,512, "CREATE TABLE IF NOT EXISTS %s("
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

bool CDatabaseManager::ModifySqlCommand(char* pCmd)
{
	string mstr(pCmd);
	int index0 = 0;
	int index1 = 0;
	index1 = mstr.find('\\', index0);
	while (index1>0)
	{
		mstr.replace(index1, 1, "\\\\");
		index0 = index1 + 2;
		index1 = mstr.find('\\', index0);
	}
	strcpy_s(pCmd,mstr.length()+1, mstr.c_str());
	return true;
}

void CDatabaseManager::Task()
{

	NetWorkResetCount++;
	if (NetWorkResetCount>10)
	{
		if ((m_SendServer.GetSendServerStatus() & SendServer::enSendOK) != SendServer::enSendOK)
		{
			DbServerLogger::mLogger.TraceInfo("SendServer异常，正在重启！");
			m_SendServer.ResetSocket();
		}
		if ((m_ReceiveServer.GetReceiveStatus() & ReceiveServer::enRecvOK) != ReceiveServer::enRecvOK)
		{
			DbServerLogger::mLogger.TraceInfo("ReceiveServer异常，正在重启！");
			m_ReceiveServer.ResetSocket();
		}
		NetWorkResetCount = 0;
	}
	EnterCriticalSection(&(m_ReceiveServer.m_section));
	bool isOK = m_ReceiveServer.PullMsg(tmpMsgItem);
	LeaveCriticalSection(&(m_ReceiveServer.m_section));
	if (!isOK)
	{
		Sleep(100);
		return;
	}
	//DbServerLogger::mLogger.TraceInfo("数据库管理收到消息！");
	Tmtv_BaseNetMessage* pBaseMsg = (Tmtv_BaseNetMessage*)(tmpMsgItem.p_Buffer);
	Tmtv_CameraInfo* pCam;
	Tmtv_ImageInfo* pImg;
	Tmt_ClientInfo* pClient;
	Tmtv_AlgorithmInfo* pAlgo;
	int b;
	vector<Tmt_ClientInfo>::iterator itvc;
	vector<Tmtv_ImageInfo>::iterator itimg;
	vector<Tmtv_CameraInfo>::iterator itcam;
	Tmtv_BaseNetMessage mBaseMsg;
	switch (pBaseMsg->MsgType)
	{
	case Tmtv_BaseNetMessage::TMTV_ADDCAM:
		DbServerLogger::mLogger.TraceInfo("请求添加相机！");
		if (pBaseMsg->ElementCount != 1 || pBaseMsg->ElementLength != sizeof(Tmtv_CameraInfo)) return;
		pCam = (Tmtv_CameraInfo*)(tmpMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage));
		if (AddCamToDb(*pCam)>0)
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_ADDCAM_OK);//消息发送给请求方
			memcpy_s(tmpSendMsgItem.p_Buffer+sizeof(Tmtv_BaseNetMessage), sizeof(Tmtv_CameraInfo), pCam,sizeof(Tmtv_CameraInfo));
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 1;
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = sizeof(Tmtv_CameraInfo);
			m_SendServer.PushMsg(tmpSendMsgItem);
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->hDstHandle = 0;//消息传给客户端
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->hSrcHandle = 0;
			for (itvc = m_ClientInfoVec.begin(); itvc != m_ClientInfoVec.end();itvc++)
			{
				strcpy_s(((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->dstAddr, TMTV_IPSTRLEN, itvc->mIpAddr);
				((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->dstPort = itvc->mport;
				m_SendServer.PushMsg(tmpSendMsgItem);
			}
			pParent->UpdateDbData();
		}
		else
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_ADDCAM_FAIL);//失败消息传给请求方
			memcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage), sizeof(Tmtv_CameraInfo), pCam, sizeof(Tmtv_CameraInfo));
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 1;
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = sizeof(Tmtv_CameraInfo);
			m_SendServer.PushMsg(tmpSendMsgItem);
		}
		break;
	case Tmtv_BaseNetMessage::TMTV_DELCAM:
		pCam = (Tmtv_CameraInfo*)(tmpMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage));
		pCam->Indexnum = pBaseMsg->m_Param;
		if (!GetCaminfoFromDb(*pCam)) 
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_DELCAM_FAIL);
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 0;
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = 0;
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->m_Param = pBaseMsg->m_Param;
			m_SendServer.PushMsg(tmpSendMsgItem);
			break;
		}
		if (pCam->Status != Tmtv_CameraInfo::TMTV_STOPEDCAM)
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_DELCAM_FAIL);
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 0;
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = 0;
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->m_Param = pBaseMsg->m_Param;
			m_SendServer.PushMsg(tmpSendMsgItem);
			break;
		}
		if (!DelCamFromDB(pCam->Indexnum))
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_DELCAM_FAIL);
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 0;
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = 0;
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->m_Param = pBaseMsg->m_Param;
			m_SendServer.PushMsg(tmpSendMsgItem);
			break;
		}
		ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_DELCAM_OK);//失败消息传给请求方
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 0;
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = 0;
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->m_Param = pBaseMsg->m_Param;
		m_SendServer.PushMsg(tmpSendMsgItem);
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->hDstHandle = 0;//消息传给客户端
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->hSrcHandle = 0;
		for (itvc = m_ClientInfoVec.begin(); itvc != m_ClientInfoVec.end(); itvc++)
		{
			strcpy_s(((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->dstAddr, TMTV_IPSTRLEN, itvc->mIpAddr);
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->dstPort = itvc->mport;
			m_SendServer.PushMsg(tmpSendMsgItem);
		}
		pParent->UpdateDbData();
		break;
	case Tmtv_BaseNetMessage::TMTV_GETCAM:
		pCam= (Tmtv_CameraInfo*)(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage));
		pCam->Indexnum = pBaseMsg->m_Param;
		if (GetCaminfoFromDb(*pCam))
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_GETCAM_OK);//成功的消息传给请求方
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 1;
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = sizeof(Tmtv_CameraInfo);
		}
		else
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_GETCAM_FAIL);//失败消息传给请求方
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 0;
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = 0;
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->m_Param = pBaseMsg->m_Param;
		}
		m_SendServer.PushMsg(tmpSendMsgItem);
		break;
	case Tmtv_BaseNetMessage::TMTV_SETCAM:
		if (pBaseMsg->ElementCount != 1 || pBaseMsg->ElementLength != sizeof(Tmtv_CameraInfo)) return;
		pCam = (Tmtv_CameraInfo*)(tmpMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage));
		if (UpdateCamInfoDb(*pCam))
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_GETCAM_OK);//成功的消息传给请求方
		}
		else
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_GETCAM_FAIL);//失败消息传给请求方
		}
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 1;
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = sizeof(Tmtv_CameraInfo);
		memcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage), sizeof(Tmtv_CameraInfo), pCam, sizeof(Tmtv_CameraInfo));
		m_SendServer.PushMsg(tmpSendMsgItem);
		pParent->UpdateDbData();
		break;
	case Tmtv_BaseNetMessage::TMTV_CHECKCAM:
		if (pBaseMsg->ElementCount != 1 || pBaseMsg->ElementLength != sizeof(Tmtv_CameraInfo)) return;
		pCam = (Tmtv_CameraInfo*)(tmpMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage));
		b = CheckCamInfo(*pCam);
		if (b == CamCheckOK)
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_CHECKCAM_OK);
			memcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage), sizeof(Tmtv_CameraInfo), pCam, sizeof(Tmtv_CameraInfo));
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 1;
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = sizeof(Tmtv_CameraInfo);
			m_SendServer.PushMsg(tmpSendMsgItem);
		}
		else
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_CHECKCAM_FAIL);
			//memcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage), sizeof(Tmtv_CameraInfo), pCam, sizeof(Tmtv_CameraInfo));
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 1;
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = 64;
			switch (b)
			{
			case CheckStatus::CamNameExist:
				strcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage), 64, "相机描述重复！");
				break;
			case CheckStatus::CamPathExist:
				strcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage), 64, "相机目录重复！");
				break;
			default:
				strcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage), 64, "未知错误！");
				break;
			}
		}
		break;
	case Tmtv_BaseNetMessage::TMTV_SNAPED:
		if (pBaseMsg->ElementCount != 1 || pBaseMsg->ElementLength != sizeof(Tmtv_ImageInfo)) return;
		pImg = (Tmtv_ImageInfo*)(tmpMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage));
		InsertImgInfoToDb(*pImg);
		for (itvc = m_ClientInfoVec.begin(); itvc != m_ClientInfoVec.end();itvc++)
		{
			mBaseMsg.MsgType = Tmtv_BaseNetMessage::TMTV_SNAPED;
			strcpy_s(mBaseMsg.dstAddr, TMTV_IPSTRLEN, itvc->mIpAddr);
			mBaseMsg.dstPort = itvc->mport;
			strcpy_s(mBaseMsg.mAddr, TMTV_IPSTRLEN, m_Setting.mRecvSetting.m_LocalRecvIP);
			mBaseMsg.mPort = m_Setting.mRecvSetting.m_LocalRecvPort;
			mBaseMsg.ElementCount = 1;
			mBaseMsg.ElementLength = sizeof(Tmtv_ImageInfo);
			memcpy_s(tmpSendMsgItem.p_Buffer, sizeof(Tmtv_BaseNetMessage), &mBaseMsg, sizeof(Tmtv_BaseNetMessage));
			memcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage), sizeof(Tmtv_ImageInfo), pImg, sizeof(Tmtv_ImageInfo));
			m_SendServer.PushMsg(tmpSendMsgItem);
		};
		break;
	case Tmtv_BaseNetMessage::TMTV_DETECTED:
		if (pBaseMsg->ElementCount != 1 || pBaseMsg->ElementLength != sizeof(Tmtv_ImageInfo)) return;
		pImg = (Tmtv_ImageInfo*)(tmpMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage));
		InsertImgInfoToDb(*pImg);
		for (itvc = m_ClientInfoVec.begin(); itvc != m_ClientInfoVec.end(); itvc++)
		{
			mBaseMsg.MsgType = Tmtv_BaseNetMessage::TMTV_DETECTED;
			strcpy_s(mBaseMsg.dstAddr, TMTV_IPSTRLEN, itvc->mIpAddr);
			mBaseMsg.dstPort = itvc->mport;
			strcpy_s(mBaseMsg.mAddr, TMTV_IPSTRLEN, m_Setting.mRecvSetting.m_LocalRecvIP);
			mBaseMsg.mPort = m_Setting.mRecvSetting.m_LocalRecvPort;
			mBaseMsg.ElementCount = 1;
			mBaseMsg.ElementLength = sizeof(Tmtv_ImageInfo);
			memcpy_s(tmpSendMsgItem.p_Buffer, sizeof(Tmtv_BaseNetMessage), &mBaseMsg, sizeof(Tmtv_BaseNetMessage));
			memcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage), sizeof(Tmtv_ImageInfo), pImg, sizeof(Tmtv_ImageInfo));
			m_SendServer.PushMsg(tmpSendMsgItem);
		};

		break;
	case Tmtv_BaseNetMessage::TMTV_GETRANGEIMG:
		if (pBaseMsg->ElementCount != 2) return;
		GetImginfoByTime(mImgVec, pBaseMsg->m_Param, tmpMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage),
			tmpMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage) + pBaseMsg->ElementLength);
		ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_GETRANGEIMG_OK);
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = (mImgVec.size()>((MessageItem::MAXMSGSIZE-sizeof(Tmtv_BaseNetMessage))/sizeof(Tmtv_ImageInfo)-2))?
			(MessageItem::MAXMSGSIZE / sizeof(Tmtv_ImageInfo) - 5): mImgVec.size();
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = (mImgVec.size()>0)?sizeof(Tmtv_ImageInfo):0;
		b = 0;
		for (itimg = mImgVec.begin(); itimg != mImgVec.end(); itimg++)
		{
			memcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage) + b*sizeof(Tmtv_ImageInfo), sizeof(Tmtv_ImageInfo),
				&(*itimg), sizeof(Tmtv_ImageInfo));
			b++;
			if (b == ((MessageItem::MAXMSGSIZE - sizeof(Tmtv_BaseNetMessage)) / sizeof(Tmtv_ImageInfo) - 2))
			{
				m_SendServer.PushMsg(tmpSendMsgItem);
				b = 0;
			}
		}
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = b;
		m_SendServer.PushMsg(tmpSendMsgItem);
		break;
	case Tmtv_BaseNetMessage::TMTV_GETALLIMG:
		if (GetLastImginfoAllCam(mImgVec))
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_GETALLIMG_OK);
		}
		else
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_GETALLIMG_FAIL);
		}
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = (mImgVec.size() > ((MessageItem::MAXMSGSIZE - sizeof(Tmtv_BaseNetMessage)) / sizeof(Tmtv_ImageInfo) - 2)) ?
			(MessageItem::MAXMSGSIZE / sizeof(Tmtv_ImageInfo) - 5) : mImgVec.size();
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = (mImgVec.size() > 0) ? sizeof(Tmtv_ImageInfo) : 0;
		b = 0;
		for (itimg = mImgVec.begin(); itimg != mImgVec.end(); itimg++)
		{
			memcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage) + b*sizeof(Tmtv_ImageInfo), sizeof(Tmtv_ImageInfo),
				&(*itimg), sizeof(Tmtv_ImageInfo));
			b++;
			if (b == ((MessageItem::MAXMSGSIZE - sizeof(Tmtv_BaseNetMessage)) / sizeof(Tmtv_ImageInfo) - 2))
			{
				m_SendServer.PushMsg(tmpSendMsgItem);
				b = 0;
			}
		}
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = b;
		m_SendServer.PushMsg(tmpSendMsgItem);
		break;
	case Tmtv_BaseNetMessage::TMTV_GETALLCAM:
		if (GetAllCamInfoFrmDb(mCamVec))
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_GETALLCAM_OK);
		}
		else
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_GETALLCAM_FAIL);
		}
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = (mCamVec.size() > ((MessageItem::MAXMSGSIZE - sizeof(Tmtv_BaseNetMessage)) / sizeof(Tmtv_CameraInfo) - 2)) ?
			(MessageItem::MAXMSGSIZE / sizeof(Tmtv_CameraInfo) - 5) : mCamVec.size();
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = (mCamVec.size() > 0) ? sizeof(Tmtv_CameraInfo) : 0;
		b = 0;
		for (itcam = mCamVec.begin(); itcam != mCamVec.end(); itcam++)
		{
			memcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage) + b*sizeof(Tmtv_CameraInfo), sizeof(Tmtv_CameraInfo),
				&(*itcam), sizeof(Tmtv_CameraInfo));
			b++;
			if (b == ((MessageItem::MAXMSGSIZE - sizeof(Tmtv_BaseNetMessage)) / sizeof(Tmtv_CameraInfo) - 2))
			{
				m_SendServer.PushMsg(tmpSendMsgItem);
				b = 0;
			}
		}
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = b;
		m_SendServer.PushMsg(tmpSendMsgItem);	
		break;
	case Tmtv_BaseNetMessage::TMTV_STARTCAM:
		pCam = (Tmtv_CameraInfo*)(tmpMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage));
		pCam->Indexnum = pBaseMsg->m_Param;
		
		if (GetCaminfoFromDb(*pCam))
		{
			pCam->Status = Tmtv_CameraInfo::TMTV_RUNNINGCAM;
			if (UpdateCamInfoDb(*pCam))
			{
				ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STARTCAM_OK);
			}
			else
			{
				ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STARTCAM_FAIL);
			}
		}
		else
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STARTCAM_FAIL);
		}
		m_SendServer.PushMsg(tmpSendMsgItem);
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->hDstHandle = 0;//消息传给客户端
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->hSrcHandle = 0;
		for (itvc = m_ClientInfoVec.begin(); itvc != m_ClientInfoVec.end(); itvc++)
		{
			strcpy_s(((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->dstAddr, TMTV_IPSTRLEN, itvc->mIpAddr);
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->dstPort = itvc->mport;
			m_SendServer.PushMsg(tmpSendMsgItem);
		}
		pParent->UpdateDbData();
		break;
	case Tmtv_BaseNetMessage::TMTV_STOPCAM:
		pCam = (Tmtv_CameraInfo*)(tmpMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage));
		pCam->Indexnum = pBaseMsg->m_Param;

		if (GetCaminfoFromDb(*pCam))
		{
			pCam->Status = Tmtv_CameraInfo::TMTV_STOPEDCAM;
			if (UpdateCamInfoDb(*pCam))
			{
				ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STOPCAM_OK);
			}
			else
			{
				ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STOPCAM_FAIL);
			}
		}
		else
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STOPCAM_FAIL);
		}
		m_SendServer.PushMsg(tmpSendMsgItem);
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->hDstHandle = 0;//消息传给客户端
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->hSrcHandle = 0;
		for (itvc = m_ClientInfoVec.begin(); itvc != m_ClientInfoVec.end(); itvc++)
		{
			strcpy_s(((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->dstAddr, TMTV_IPSTRLEN, itvc->mIpAddr);
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->dstPort = itvc->mport;
			m_SendServer.PushMsg(tmpSendMsgItem);
		}
		pParent->UpdateDbData();
		break;
	case Tmtv_BaseNetMessage::TMTV_STARTALGO:
		pCam = (Tmtv_CameraInfo*)(tmpMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage));
		pCam->Indexnum = pBaseMsg->m_Param;

		if (GetCaminfoFromDb(*pCam))
		{
			pCam->AlgorithmInfo.mAlgoStatus = Tmtv_AlgorithmInfo::TMTV_STARTWARN;
			if (UpdateCamInfoDb(*pCam))
			{
				ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STARTALGO_OK);
				((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->hDstHandle = 0;//消息传给客户端
				((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->hSrcHandle = 0;
				for (itvc = m_ClientInfoVec.begin(); itvc != m_ClientInfoVec.end(); itvc++)
				{
					strcpy_s(((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->dstAddr, TMTV_IPSTRLEN, itvc->mIpAddr);
					((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->dstPort = itvc->mport;
					m_SendServer.PushMsg(tmpSendMsgItem);
				}
			}
			else
			{
				ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STARTALGO_FAIL);
			}
		}
		else
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STARTALGO_FAIL);
		}
		m_SendServer.PushMsg(tmpSendMsgItem);
		pParent->UpdateDbData();
		break;
	case Tmtv_BaseNetMessage::TMTV_STOPCALGO:
		pCam = (Tmtv_CameraInfo*)(tmpMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage));
		pCam->Indexnum = pBaseMsg->m_Param;

		if (GetCaminfoFromDb(*pCam))
		{
			pCam->AlgorithmInfo.mAlgoStatus = Tmtv_AlgorithmInfo::TMTV_NOWARN;
			if (UpdateCamInfoDb(*pCam))
			{
				ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STARTALGO_OK);
				((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->hDstHandle = 0;//消息传给客户端
				((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->hSrcHandle = 0;
				for (itvc = m_ClientInfoVec.begin(); itvc != m_ClientInfoVec.end(); itvc++)
				{
					strcpy_s(((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->dstAddr, TMTV_IPSTRLEN, itvc->mIpAddr);
					((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->dstPort = itvc->mport;
					m_SendServer.PushMsg(tmpSendMsgItem);
				}
			}
			else
			{
				ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STARTALGO_FAIL);
			}
		}
		else
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STARTALGO_FAIL);
		}
		m_SendServer.PushMsg(tmpSendMsgItem);
		pParent->UpdateDbData();
		break;
	case Tmtv_BaseNetMessage::TMTV_SETALGO:
		if (pBaseMsg->ElementCount != 1 || pBaseMsg->ElementLength != sizeof(Tmtv_AlgorithmInfo)) return;
		pAlgo = (Tmtv_AlgorithmInfo*)(pBaseMsg + sizeof(Tmtv_BaseNetMessage));
		pCam = (Tmtv_CameraInfo*)(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage));
		pCam->Indexnum = pBaseMsg->m_Param;
		if (GetCaminfoFromDb(*pCam))
		{
			memcpy_s(&(pCam->AlgorithmInfo), sizeof(Tmtv_AlgorithmInfo), pAlgo, sizeof(Tmtv_AlgorithmInfo));
			if (UpdateCamInfoDb(*pCam))
			{
				ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_SETALGO_OK);
				((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 1;
				((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = sizeof(Tmtv_AlgorithmInfo);
				((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->m_Param = pBaseMsg->m_Param;
				memcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage), sizeof(Tmtv_AlgorithmInfo), pAlgo, sizeof(Tmtv_AlgorithmInfo));
				m_SendServer.PushMsg(tmpSendMsgItem);
				((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->hDstHandle = 0;//消息传给客户端
				((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->hSrcHandle = 0;
				for (itvc = m_ClientInfoVec.begin(); itvc != m_ClientInfoVec.end(); itvc++)
				{
					strcpy_s(((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->dstAddr, TMTV_IPSTRLEN, itvc->mIpAddr);
					((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->dstPort = itvc->mport;
					m_SendServer.PushMsg(tmpSendMsgItem);
				}
			}
			else
			{
				ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STARTALGO_FAIL);
				((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 1;
				((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = sizeof(Tmtv_AlgorithmInfo);
				((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->m_Param = pBaseMsg->m_Param;
				memcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage), sizeof(Tmtv_AlgorithmInfo), pAlgo, sizeof(Tmtv_AlgorithmInfo));
				m_SendServer.PushMsg(tmpSendMsgItem);
			}
		}
		else
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STARTALGO_FAIL);
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 1;
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = sizeof(Tmtv_AlgorithmInfo);
			((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->m_Param = pBaseMsg->m_Param;
			memcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage), sizeof(Tmtv_AlgorithmInfo), pAlgo, sizeof(Tmtv_AlgorithmInfo));
			m_SendServer.PushMsg(tmpSendMsgItem);
		}
		pParent->UpdateDbData();
		break;
	case Tmtv_BaseNetMessage::TMTV_ADDCLIENT:
		if(pBaseMsg->ElementCount !=1 || pBaseMsg->ElementLength!=sizeof(Tmt_ClientInfo)) break;
		pClient = (Tmt_ClientInfo*)(tmpMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage));
		InsertClientInfoToDb(*pClient);
		pClient->status = Tmt_ClientInfo::TMT_CLIENT_RUNNING;
		if (UpdateClientStatusDb(*pClient))
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_ADDCLIENT_OK);
		}
		else
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_ADDCLIENT_FAIL);
		}
		memcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage), sizeof(Tmt_ClientInfo), pClient, sizeof(Tmt_ClientInfo));
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 1;
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = sizeof(Tmt_ClientInfo);
		m_SendServer.PushMsg(tmpSendMsgItem);

		DbServerLogger::mLogger.TraceInfo("注册客户端成功！%s:%d", pClient->mIpAddr, pClient->mport);
		GetActiveClientInfoFrmDb(m_ClientInfoVec);
		pParent->UpdateDbData();
		break;
	case Tmtv_BaseNetMessage::TMTV_STOPCLIENT:
		if (pBaseMsg->ElementCount != 1 || pBaseMsg->ElementLength != sizeof(Tmt_ClientInfo)) break;
		pClient = (Tmt_ClientInfo*)(tmpMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage));
		InsertClientInfoToDb(*pClient);
		pClient->status = Tmt_ClientInfo::TMT_CLIENT_CLOSE;
		if (UpdateClientStatusDb(*pClient))
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STOPCLIENT_OK);
		}
		else
		{
			ResponseAsk(tmpSendMsgItem, *pBaseMsg, Tmtv_BaseNetMessage::TMTV_STOPCLIENT_FAIL);
		}
		memcpy_s(tmpSendMsgItem.p_Buffer + sizeof(Tmtv_BaseNetMessage), sizeof(Tmt_ClientInfo), pClient, sizeof(Tmt_ClientInfo));
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementCount = 1;
		((Tmtv_BaseNetMessage*)(tmpSendMsgItem.p_Buffer))->ElementLength = sizeof(Tmt_ClientInfo);
		m_SendServer.PushMsg(tmpSendMsgItem);
		DbServerLogger::mLogger.TraceInfo("注销客户端成功！%s:%d", pClient->mIpAddr, pClient->mport);
		GetActiveClientInfoFrmDb(m_ClientInfoVec);
		pParent->UpdateDbData();
		break;
	default:
		break;
	}
}

bool CDatabaseManager::GetYearMonth(char* pData, int Datalength)
{
	wchar_t tmp[32];
	CCommonFunc::GetNowDate(tmp, 32);
	CCommonFunc::UnicodeToAnsi(tmp, pData, Datalength);
	pData[8] = '\0';
	return true;
}

int CDatabaseManager::GetDefectsPosFromJson(int DfPos[][8], char *JsonStr, int DefectsNum)
{
	Json::Reader reader;
	Json::Value root;
	reader.parse(JsonStr, JsonStr + strlen(JsonStr), root, false);
	Json::Value add_value = root["defects"];
	for (int i = 0; i < add_value.size(); ++i)
	{
		Json::Value temp_value = add_value[i];
		DfPos[i][0]=temp_value["x"].asInt();
		DfPos[i][1] = temp_value["y"].asInt();
		DfPos[i][2] = temp_value["width"].asInt();
		DfPos[i][3] = temp_value["height"].asInt();
	}
	return add_value.size();
}

int CDatabaseManager::ConvertDefectsToJson(char* JsonStr,int bufferlength, int DfPos[][8], int DefectsNum)
{
	Json::Value root;
	Json::Value& add_value = root["defects"];
	Json::Value append_value;
	char mchar[16] = { 0 };
	for (int i = 0; i < DefectsNum; i++)
	{
		sprintf_s(mchar, "%d", DfPos[i][0]);
		append_value["x"] = DfPos[i][0];//mchar;
		sprintf_s(mchar, "%d", DfPos[i][1]);
		append_value["y"] = DfPos[i][1];// mchar;
		sprintf_s(mchar, "%d", DfPos[i][2]);
		append_value["width"] = DfPos[i][2];// mchar;
		sprintf_s(mchar, "%d", DfPos[i][3]);
		append_value["height"] = DfPos[i][3];// mchar;
		add_value.append(append_value);
	}
	Json::FastWriter writer;
	std::string json_append_file = writer.write(root);
	strcpy_s(JsonStr, bufferlength, json_append_file.c_str());
	return DefectsNum;
}

Logger DbServerLogger::mLogger("DbServerLog\\");



