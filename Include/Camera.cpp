#include "Camera.h"
#include "json.h"
#define DEBUG_CMD
using namespace std;


#pragma region CamSetting

CamSetting::CamSetting()
{
#ifdef DEBUG_CMD
	printf_s("CamSetting::CamSetting()\n");
#endif
	//CameraMAC[0] = 0;
	for (int i = 0; i < 8; i++)
	{
		cameraPos[i] = -1;
	}
	//cameraMaxWidth = 1920;
	//cameraMaxHeight = 1080;
	//cameraTop = 0;
	//cameraLeft = 0;
	cameraWidth = cameraMaxWidth;
	cameraHeight = cameraMaxHeight;
	//cameraExposure = -1;
};

CamSetting::~CamSetting()
{
#ifdef DEBUG_CMD
	printf_s("CamSetting::~CamSetting()\n");
#endif	
};

CamSetting::CamSetting(const CamSetting & setting)
{
	CopyFrom(setting);
}

CamSetting::CamSetting(const MEGASTR& setting)
{
	CopyFrom(setting);
}

CamSetting & CamSetting::operator=(const CamSetting & setting)
{
	CopyFrom(setting);
	return *this;
}

CamSetting & CamSetting::operator=(const MEGASTR& setting)
{
	CopyFrom(setting);
	return *this;
}

void CamSetting::CopyTo(CamSetting& setting)
{
	DeSetting::CopyTo((DeSetting&)setting);
	try
	{
		//strcpy_s(setting.CameraMAC, sizeof(setting.CameraMAC), CameraMAC);
		for (int i = 0; i < 8; i++)
		{
			setting.cameraPos[i] = cameraPos[i];
		}
		setting.cameraMaxWidth = cameraMaxWidth;
		setting.cameraMaxHeight = cameraMaxHeight;
		setting.cameraTop = cameraTop;
		setting.cameraLeft = cameraLeft;
		setting.cameraWidth = cameraWidth;
		setting.cameraHeight = cameraHeight;
		setting.cameraExposure = cameraExposure;
		strcpy_s(setting.imageType, sizeof(setting.imageType), imageType);
		strcpy_s(setting.imageFormat, sizeof(setting.imageFormat), imageFormat);
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

void CamSetting::CopyTo(MEGASTR& setting)
{
	DeSetting::CopyTo(setting);
	try
	{
		Json::FastWriter writer;
		Json::Value root;
		root["cameraName"] = cameraName;
		root["cameraPath"] = cameraPath;
		root["cameraHostMAC"] = cameraHostMAC;
		root["cameraHostIP"] = cameraHostIP;
		root["serverDataSize"] = serverDataSize;
		root["serverTimes"] = serverTimes;
		root["serverFrameTime"] = serverFrameTime;
		//root["ServerIncludeTaskTime"] = ServerIncludeTaskTime;
		//root["CameraMAC"] = CameraMAC;
		Json::Value sub;
		for (int i = 0; i < 8; i++)
		{
			sub[i] = cameraPos[i];
		}
		root["cameraPos"] = sub;
		root["cameraMaxWidth"] = cameraMaxWidth;
		root["cameraMaxHeight"] = cameraMaxHeight;
		root["cameraTop"] = cameraTop;
		root["cameraLeft"] = cameraLeft;
		root["cameraWidth"] = cameraWidth;
		root["cameraHeight"] = cameraHeight;
		root["cameraExposure"] = cameraExposure;
		strcpy_s(setting, sizeof(setting), writer.write(root).data());
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

void CamSetting::CopyFrom(const CamSetting& setting)
{
	DeSetting::CopyFrom((const DeSetting&)setting);
	try
	{
		//strcpy_s(CameraMAC, sizeof(CameraMAC), setting.CameraMAC);
		for (int i = 0; i < 8; i++)
		{
			cameraPos[i] = setting.cameraPos[i];
		}
		cameraMaxWidth = setting.cameraMaxWidth;
		cameraMaxHeight = setting.cameraMaxHeight;
		cameraTop = setting.cameraTop;
		cameraLeft = setting.cameraLeft;
		cameraWidth = setting.cameraWidth;
		cameraHeight = setting.cameraHeight;
		cameraExposure = setting.cameraExposure;
		strcpy_s(imageType, sizeof(imageType), setting.imageType);
		strcpy_s(imageFormat, sizeof(imageFormat), setting.imageFormat);
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

void CamSetting::CopyFrom(const MEGASTR& setting)
{
	DeSetting::CopyFrom(setting);
	try
	{
		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(setting, root, false))
		{
			throw DE_ERR;
		}
		strcpy_s(cameraName, sizeof(cameraName), root["cameraName"].asString().data());
		strcpy_s(cameraPath, sizeof(cameraPath), root["cameraPath"].asString().data());
		strcpy_s(cameraHostMAC, sizeof(cameraHostMAC), root["cameraHostMAC"].asString().data());
		strcpy_s(cameraHostIP, sizeof(cameraHostIP), root["cameraHostIP"].asString().data());
		serverDataSize = root["serverDataSize"].asInt();
		serverTimes = root["serverTimes"].asInt64();
		serverFrameTime = root["serverFrameTime"].asInt64();
		//ServerIncludeTaskTime = root["ServerIncludeTaskTime"].asBool();

		//strcpy_s(CameraMAC, sizeof(CameraMAC), root["CameraMAC"].asString().data());
		Json::Value sub = root["cameraPos"];
		for (int i = 0; i < 8; ++i)
		{
			if (i<sub.size())
			{
				cameraPos[i] = sub[i].asDouble();
			} 
			else
			{
				cameraPos[i] = -1;
			}
		}
		cameraMaxWidth = root["cameraMaxWidth"].asInt();
		cameraMaxHeight = root["cameraMaxHeight"].asInt();
		cameraTop = root["cameraTop"].asInt();
		cameraLeft = root["cameraLeft"].asInt();
		cameraWidth = root["cameraWidth"].asInt();
		cameraHeight = root["cameraHeight"].asInt();
		cameraExposure = root["cameraExposure"].asInt64();
		strcpy_s(imageType, sizeof(imageType), root["imageType"].asString().data());
		strcpy_s(imageFormat, sizeof(imageFormat), root["imageFormat"].asString().data());
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

#pragma endregion CamSetting


#pragma region CamData

CamData::CamData()
{
#ifdef DEBUG_CMD
	printf_s("CamData::CamData()\n");
#endif
}

CamData::~CamData()
{
#ifdef DEBUG_CMD
	printf_s("CamData::~CamData()\n");
#endif
}

CamData::CamData(const CamData & dataIn)
{
	CopyFrom(dataIn);
};

CamData & CamData::operator=(const CamData & dataIn)
{
	CopyFrom(dataIn);
	return *this;
};

void CamData::CopyTo(CamData & dataIn)
{
	DeData::CopyTo((DeData&)dataIn);
}

void CamData::CopyFrom(const CamData & dataIn)
{
	DeData::CopyFrom((DeData&)dataIn);
}

bool CamData::operator==(const DeData & dataIn)
{
#ifdef DEBUG_CMD
	printf_s("CamData::operator==(const DeData & dataIn)\n");
#endif
	// Do something here
	return DeData::operator==(dataIn);
}

#pragma endregion CamData


#pragma region Camera

Camera::Camera()
{
#ifdef DEBUG_CMD
	printf_s("Camera::Camera()\n");
#endif
}

Camera::~Camera()
{
#ifdef DEBUG_CMD
	printf_s("Camera::~Camera()\n");
#endif
	Unitial();
}

bool Camera::Initial(DeSetting* pSetting)
{
#ifdef DEBUG_CMD
	printf_s("Camera::Initial(DeSetting* pSetting)\n");
#endif
	m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_INITIALED;
	if (pSetting == 0)
	{
		return false;
	}
	if (p_DeSetting == 0)
	{
		p_DeSetting = new CamSetting();//override here
	}
	// Deep copy,override here
	*p_DeSetting = *((CamSetting*)pSetting);
	return true;
}

void Camera::Unitial()
{
#ifdef DEBUG_CMD
	printf_s("Camera::Unitial()\n");
#endif
	//try
	//{
	//	// Do something here
	//}
	//catch (DWORD& e)
	//{
	//}
	//catch (exception& e)
	//{
	//}
	Device::Unitial();
	//m_hDevice = INVALID_HANDLE_VALUE;
	//m_DeStatus = DE_UNITIALED;
	//if (p_DeSetting != 0)
	//{
	//	delete p_DeSetting;//override here
	//	p_DeSetting = 0;
	//}
}

bool Camera::Set(DeSetting* pSetting)
{
#ifdef DEBUG_CMD
	printf_s("Camera::Set(DeSetting* pSetting)\n");
#endif
	m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_UNITIALED;
	if (pSetting == 0)
	{
		return false;
	}
	if (p_DeSetting == 0)
	{
		p_DeSetting = new CamSetting();//override here
	}
	// Deep copy
	*p_DeSetting = *pSetting;
	// Do something here
	//m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_INITIALED;
	return true;
}

bool Camera::ReSet(DeSetting* pSetting)
{
#ifdef DEBUG_CMD
	printf_s("Camera::ReSet(DeSetting* pSetting)\n");
#endif
	Unitial();
	//try
	//{
	//	// Do something here
	//}
	//catch (DWORD& e)
	//{
	//}
	//catch (exception& e)
	//{
	//}
	return Initial(pSetting);
}

DeData* Camera::Sample()
{
	DeData* pData = 0;
	try
	{
		pData = new CamData();// Overwrite this DeData type in derived struct
		// Do something here
		//((CamData*)pData)->data[0] = 1;
		//((CamData*)pData)->data[1] = 2;
		//((CamData*)pData)->data[2] = 4;
		//((CamData*)pData)->data[3] = 8;
		// Recode sample time.
		GetLocalTime(&pData->updateTime);
		sprintf_s(pData->dataTimeStr, sizeof(pData->dataTimeStr),
			"%d-%d-%d %d:%d:%d:%d",
			pData->updateTime.wYear,
			pData->updateTime.wMonth,
			pData->updateTime.wDay,
			pData->updateTime.wHour,
			pData->updateTime.wMinute,
			pData->updateTime.wSecond,
			pData->updateTime.wMilliseconds);
		//printf("%d %d %d %d %s\n", ((CamData*)pData)->data[0], ((CamData*)pData)->data[1],
		//	((CamData*)pData)->data[2], ((CamData*)pData)->data[3], pData->dataTimeStr);
	}
	catch (DWORD& e)
	{
		if (pData != 0)
		{
			delete pData;
			pData = 0;
		}
	}
	catch (exception& e)
	{
		if (pData != 0)
		{
			delete pData;
			pData = 0;
		}
	}
	return pData;
}

#pragma endregion Camera

