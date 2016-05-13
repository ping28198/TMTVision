#include <WinSock2.h> // List front to avoid <WinSock.h> previous
#pragma comment(lib,"Ws2_32.lib")
#include "Device.h"
#include "json.h"
#define DEBUG_CMD
using namespace std;


#pragma region DeSetting

DeSetting::DeSetting()
{
#ifdef DEBUG_CMD
    printf_s("DeSetting::DeSetting()\n");
#endif
	deviceName[0] = 0;
	devicePath[0] = 0;
	deviceMAC[0] = 0;
	deviceIP[0] = 0;
	serverDataSize = 0;
	serverTimes = -1;
	serverFrameTime = 0;
	//ServerIncludeTaskTime = true;
	try
	{
		WSADATA wsaData;
		char *ip = 0;//定义IP地址变量
		hostent *hostinfo = 0;
		//调用MAKEWORD（）获得Winsock版本的正确值，用于加载Winsock库
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0) {
			//现在是加载Winsock库，如果WSAStartup（）函数返回值为0，说明加载成功，程序可以继续
			if (gethostname(deviceName,sizeof(deviceName)) == 0)
			{
				//如果成功地将本地主机名存放入由name参数指定的缓冲区中
				hostinfo = gethostbyname(deviceName);
				if (hostinfo != 0)
				{
					//这是获取主机名，如果获得主机名成功的话，将返回一个指针，指向hostinfo，hostinfo
					//为PHOSTENT型的变量，下面即将用到这个结构体
					ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
					strcpy_s(deviceIP, sizeof(deviceIP),ip);
				}
			}
			WSACleanup(); //卸载Winsock库，并释放所有资源
		}
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

DeSetting::~DeSetting()
{
#ifdef DEBUG_CMD
	printf_s("DeSetting::~DeSetting()\n");
#endif
}

DeSetting::DeSetting(const DeSetting & setting)
{
	CopyFrom(setting);
}

DeSetting::DeSetting(const MEGASTR& setting)
{
	CopyFrom(setting);
}

DeSetting & DeSetting::operator=(const DeSetting & setting)
{
	CopyFrom(setting);
	return *this;
}

DeSetting & DeSetting::operator=(const MEGASTR& setting)
{
	CopyFrom(setting);
	return *this;
}

void DeSetting::CopyTo(DeSetting& setting)
{
	try
	{
		strcpy_s(setting.deviceName, sizeof(setting.deviceName), deviceName);
		strcpy_s(setting.devicePath, sizeof(setting.devicePath), devicePath);
		strcpy_s(deviceMAC, sizeof(deviceMAC), setting.deviceMAC);
		strcpy_s(deviceIP, sizeof(deviceIP), setting.deviceIP);
		setting.serverDataSize = serverDataSize;
		setting.serverTimes = serverTimes;
		setting.serverFrameTime = serverFrameTime;
		//setting.ServerIncludeTaskTime = ServerIncludeTaskTime;
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

void DeSetting::CopyTo(MEGASTR& setting)
{
	try
	{
		setting[0] = 0;
		Json::FastWriter writer;
		Json::Value root;
		root["deviceName"] = deviceName;
		root["devicePath"] = devicePath;
		root["deviceMAC"] = deviceMAC;
		root["deviceIP"] = deviceIP;
		root["serverDataSize"] = serverDataSize;
		root["serverTimes"] = serverTimes;
		root["serverFrameTime"] = serverFrameTime;
		//root["ServerIncludeTaskTime"] = ServerIncludeTaskTime;
		strcpy_s(setting, sizeof(setting), writer.write(root).data());
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

void DeSetting::CopyFrom(const DeSetting& setting)
{
	try
	{
		strcpy_s(deviceName, sizeof(deviceName), setting.deviceName);
		strcpy_s(devicePath, sizeof(devicePath), setting.devicePath);
		strcpy_s(deviceMAC, sizeof(deviceMAC), setting.deviceMAC);
		strcpy_s(deviceIP, sizeof(deviceIP), setting.deviceIP);
		serverDataSize = setting.serverDataSize;
		serverTimes = setting.serverTimes;
		serverFrameTime = setting.serverFrameTime;
		//ServerIncludeTaskTime = setting.ServerIncludeTaskTime;
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

void DeSetting::CopyFrom(const MEGASTR& setting)
{
	try
	{
		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(setting, root, false))
		{
			throw DE_ERR;
		}
		strcpy_s(deviceName, sizeof(deviceName), root["deviceName"].asString().data());
		strcpy_s(devicePath, sizeof(devicePath), root["devicePath"].asString().data());
		strcpy_s(deviceMAC, sizeof(deviceMAC), root["deviceMAC"].asString().data());
		strcpy_s(deviceIP, sizeof(deviceIP), root["deviceIP"].asString().data());
		serverDataSize = root["serverDataSize"].asInt();
		serverTimes = root["serverTimes"].asInt64();
		serverFrameTime = root["serverFrameTime"].asInt64();
		//ServerIncludeTaskTime = root["ServerIncludeTaskTime"].asBool();
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

void DeSetting::Clear()
{
	deviceName[0] = 0;
	devicePath[0] = 0;
	deviceMAC[0] = 0;
	deviceIP[0] = 0;
	serverDataSize = 0;
	serverTimes = -1;
	serverFrameTime = 0;
	//ServerIncludeTaskTime = true;
	try
	{
		WSADATA wsaData;
		char *ip = 0;//定义IP地址变量
		hostent *hostinfo = 0;
		//调用MAKEWORD（）获得Winsock版本的正确值，用于加载Winsock库
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0) {
			//现在是加载Winsock库，如果WSAStartup（）函数返回值为0，说明加载成功，程序可以继续
			if (gethostname(deviceName, sizeof(deviceName)) == 0)
			{
				//如果成功地将本地主机名存放入由name参数指定的缓冲区中
				hostinfo = gethostbyname(deviceName);
				if (hostinfo != 0)
				{
					//这是获取主机名，如果获得主机名成功的话，将返回一个指针，指向hostinfo，hostinfo
					//为PHOSTENT型的变量，下面即将用到这个结构体
					ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
					strcpy_s(deviceIP, sizeof(deviceIP), ip);
				}
			}
			WSACleanup(); //卸载Winsock库，并释放所有资源
		}
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

int DeSetting::GetAvailable(SettingSet<DeSetting>& deSettings)
{
	try
	{	
		WSADATA wsaData;
		char *ip = 0;//定义IP地址变量
		hostent *hostinfo = 0;

		//调用MAKEWORD（）获得Winsock版本的正确值，用于加载Winsock库

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0) {
			//现在是加载Winsock库，如果WSAStartup（）函数返回值为0，说明加载成功，程序可以继续
			if (gethostname(deSettings.SettingData[0].deviceName,
				sizeof(deSettings.SettingData[0].deviceName)) == 0) 
			{
				//如果成功地将本地主机名存放入由name参数指定的缓冲区中
				hostinfo = gethostbyname(deSettings.SettingData[0].deviceName);
				if (hostinfo != 0)
				{
					//这是获取主机名，如果获得主机名成功的话，将返回一个指针，指向hostinfo，hostinfo
					//为PHOSTENT型的变量，下面即将用到这个结构体
					ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
					strcpy_s(deSettings.SettingData[0].deviceIP, sizeof(deSettings.SettingData[0].deviceIP),
						ip);
				}
			}
			WSACleanup(); //卸载Winsock库，并释放所有资源
		}
		deSettings.SettingNum = 1;
		return deSettings.SettingNum;
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

#pragma endregion DeSetting


#pragma region DeData

DeData::DeData()
{
#ifdef DEBUG_CMD
	printf_s("DeData::DeData()\n");
#endif
	try
	{
		GetLocalTime(&createTime);
		updateTime = createTime;
		sprintf_s(dataTimeStr, sizeof(dataTimeStr),
			"%d-%d-%d %d:%d:%d:%d",
			createTime.wYear,
			createTime.wMonth,
			createTime.wDay,
			createTime.wHour,
			createTime.wMinute,
			createTime.wSecond,
			createTime.wMilliseconds);
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

DeData::~DeData()
{
#ifdef DEBUG_CMD
	printf_s("DeData::~DeData()\n");
#endif
}

DeData::DeData(const DeData & dataIn)
{
	CopyFrom(dataIn);
}
DeData & DeData::operator=(const DeData & dataIn)
{
	CopyFrom(dataIn);
	return *this;
}

void DeData::CopyTo(DeData & dataIn)
{
	try
	{
		dataIn.createTime = createTime;
		dataIn.updateTime = updateTime;
		strcpy_s(dataIn.dataTimeStr, sizeof(dataIn.dataTimeStr), dataTimeStr);
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

void DeData::CopyFrom(const DeData & dataIn)
{
	try
	{
		createTime = dataIn.createTime;
		updateTime = dataIn.createTime;
		strcpy_s(dataTimeStr, sizeof(dataTimeStr), dataIn.dataTimeStr);
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

bool DeData::operator==(const DeData & dataIn)
{
#ifdef DEBUG_CMD
	printf_s("DeData::operator==(const DeData & dataIn)\n");
#endif
	try
	{
		return 0 == strcmp(dataTimeStr, dataIn.dataTimeStr);
	}
	catch (DWORD& e)
	{
		return false;
	}
	catch (exception& e)
	{
		return false;
	}
	return false;
};

double DeData::GetLife()
{
	try
	{
		SYSTEMTIME nowTime;
		GetLocalTime(&nowTime);
		double millissecondsNow = 0;
		millissecondsNow = MAX(nowTime.wMilliseconds, 0);
		millissecondsNow += 1000 * MAX(nowTime.wSecond, 0);
		millissecondsNow += 60000 * MAX(nowTime.wMinute, 0);
		millissecondsNow += 3600000 * MAX(nowTime.wHour, 0);
		millissecondsNow += 24 * 3600000 * MAX(nowTime.wDay, 0);
		double millissecondsCreate = 0;
		millissecondsCreate = MAX(createTime.wMilliseconds, 0);
		millissecondsCreate += 1000 * MAX(createTime.wSecond, 0);
		millissecondsCreate += 60000 * MAX(createTime.wMinute, 0);
		millissecondsCreate += 3600000 * MAX(createTime.wHour, 0);
		millissecondsCreate += 24 * 3600000 * MAX(createTime.wDay, 0);
		return millissecondsNow - millissecondsCreate;
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
};

#pragma endregion DeData


#pragma region Device

Device::Device()
{
#ifdef DEBUG_CMD
	printf_s("Device::Device()\n");
#endif
	m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_INVALID;
	p_DeSetting = 0;
}

Device::~Device()
{
#ifdef DEBUG_CMD
	printf_s("Device::~Device()\n");
#endif
	Unitial();
}

bool Device::Initial(DeSetting * pSetting)
{
#ifdef DEBUG_CMD
	printf_s("Device::Initial(DeSetting* pSetting)\n");
#endif
	m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_INITIALED;
	if (pSetting == 0)
	{
		return false;
	}
	if (p_DeSetting == 0)
	{
		p_DeSetting = new DeSetting();//override here
	}
	// Deep copy,override here
	*p_DeSetting = *pSetting;
	return true;
}

void Device::Unitial()
{
#ifdef DEBUG_CMD
	printf_s("Device::Unitial()\n");
#endif
	m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_UNITIALED;
	if (p_DeSetting!=0)
	{
		delete p_DeSetting;//override here
	    p_DeSetting = 0;
	}
}

bool Device::Set(DeSetting * pSetting)
{
#ifdef DEBUG_CMD
	printf_s("Device::Set(DeSetting* pSetting)\n");
#endif
	m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_UNITIALED;
	if (pSetting == 0)
	{
		return false;
	}
	if (p_DeSetting == 0)
	{
		p_DeSetting = new DeSetting();//override here
	}
	// Deep copy
	*p_DeSetting = *pSetting;
	// Do something here
	//m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_INITIALED;
	return true;
}

bool Device::ReSet(DeSetting * pSetting)
{
#ifdef DEBUG_CMD
	printf_s("Device::ReSet(DeSetting* pSetting)\n");
#endif
	Unitial();
	return Initial(pSetting);
}

DeData* Device::Sample()
{
	DeData* pData = 0;
	try
	{
		pData = new DeData();// Overwrite this DeData type in derived struct
		// Do something here

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

#pragma endregion Device


#pragma region DeviceServer

DeviceServer::DeviceServer(HANDLE hParent)
{
#ifdef DEBUG_CMD
	printf_s("DeviceServer::DeviceServer(HANDLE hParent)\n");
#endif
	p_Device = 0;
}

DeviceServer::~DeviceServer()
{
#ifdef DEBUG_CMD
	printf_s("DeviceServer::~DeviceServer()\n");
#endif
	if (p_Device != 0)
	{
		p_Device->Unitial();
		delete p_Device;
	}
	m_DataList.clear();
};

bool DeviceServer::AttachDevice(Device* pDevice)
{
#ifdef DEBUG_CMD
	printf_s("DeviceServer::AttachDevice(Device* pDevice)\n");
#endif
	try
	{
		if (pDevice == 0) return false;
		if (pDevice->m_DeStatus != Device::DE_INITIALED) return false;
		if (pDevice->p_DeSetting == 0) return false;
		EnterCriticalSection(&m_section);
		ForceEnd();
		p_Device = pDevice;
		m_DataListMaxNum = MAX(pDevice->p_DeSetting->serverDataSize, 4);
		Create(pDevice->p_DeSetting->serverTimes, pDevice->p_DeSetting->serverFrameTime, true);
		LeaveCriticalSection(&m_section);
		return  m_ThStatus == Thread::TH_SUSPEND;
	}
	catch (DWORD& e)
	{
		LeaveCriticalSection(&m_section);
	}
	catch (exception& e)
	{
		LeaveCriticalSection(&m_section);
	}
}

void DeviceServer::DetachDevice()
{
#ifdef DEBUG_CMD
	printf_s("DeviceServer::DetachDevice()\n");
#endif
	try
	{
		EnterCriticalSection(&m_section);
		ForceEnd();
		if (p_Device == 0) return;
		p_Device->Unitial();
		p_Device = 0;
		m_DataList.clear();
		LeaveCriticalSection(&m_section);
	}
	catch (DWORD& e)
	{
		LeaveCriticalSection(&m_section);
	}
	catch (exception& e)
	{
		LeaveCriticalSection(&m_section);
	}
}

bool DeviceServer::StartServer()
{
#ifdef DEBUG_CMD
	printf_s("DeviceServer::StartServer()\n");
#endif
	try
	{
		if (p_Device == 0) return false;
		if (p_Device->m_DeStatus != Device::DE_INITIALED) return false;
		if (p_Device->p_DeSetting) return false;
		EnterCriticalSection(&m_section);
		Resume();
		LeaveCriticalSection(&m_section);
		return m_ThStatus == Thread::TH_RUNNING;
	}
	catch (DWORD& e)
	{
		LeaveCriticalSection(&m_section);
	}
	catch (exception& e)
	{
		LeaveCriticalSection(&m_section);
	}
}

bool DeviceServer::StopServer()
{
#ifdef DEBUG_CMD
	printf_s("DeviceServer::StopServer()\n");
#endif
	try
	{
		//if (p_Device == 0) return false;
		//if (p_Device->m_DeStatus != Device::DE_INITIALED) return false;
		EnterCriticalSection(&m_section);
		Suspend();
		LeaveCriticalSection(&m_section);
		return m_ThStatus == Thread::TH_SUSPEND;
	}
	catch (DWORD& e)
	{
		LeaveCriticalSection(&m_section);
	}
	catch (exception& e)
	{
		LeaveCriticalSection(&m_section);
	}
}

bool DeviceServer::Set(DeSetting* pSetting)
{
#ifdef DEBUG_CMD
	printf_s("DeviceServer::Set(DeSetting* pSetting)\n");
#endif
	try
	{
		if (p_Device == 0) return false;
		if (p_Device->m_DeStatus != Device::DE_INITIALED) return false;
		if (p_Device->p_DeSetting == 0) return false;
		EnterCriticalSection(&m_section);
		Suspend();
		if (p_Device->Set(pSetting))
		{
			Resume();
		}
		LeaveCriticalSection(&m_section);
		return m_ThStatus == Thread::TH_RUNNING;
	}
	catch (DWORD& e)
	{
		LeaveCriticalSection(&m_section);
	}
	catch (exception& e)
	{
		LeaveCriticalSection(&m_section);
	}
}

bool DeviceServer::ReSet(DeSetting* pSetting)
{
#ifdef DEBUG_CMD
	printf_s("DeviceServer::ReSet(DeSetting* pSetting)\n");
#endif
	try
	{
		if (p_Device == 0) return false;
		//if (p_Device->m_DeStatus != Device::DE_INITIALED) return false;
		//if (p_Device->p_DeSetting == 0) return false;
		EnterCriticalSection(&m_section);
		ForceEnd();
		if (p_Device->ReSet(pSetting))
		{
			m_DataList.clear();
			m_DataListMaxNum = MAX(p_Device->p_DeSetting->serverDataSize, 4);
			Create(p_Device->p_DeSetting->serverTimes, p_Device->p_DeSetting->serverFrameTime, true);
		}
		LeaveCriticalSection(&m_section);
		return m_ThStatus == Thread::TH_SUSPEND;
	}
	catch (DWORD& e)
	{
		LeaveCriticalSection(&m_section);
	}
	catch (exception& e)
	{
		LeaveCriticalSection(&m_section);
	}
}

#define  CheckMaxNums  8
void DeviceServer::Task()
{
	if (p_Device == 0) return;
	if (p_Device->m_DeStatus != Device::DE_INITIALED) return;
	if (p_Device->p_DeSetting == 0) return;

	DeData* pTmpDeData = 0;
	pTmpDeData = p_Device->Sample();
	if (pTmpDeData != 0)
	{
		EnterCriticalSection(&m_section);
		if (!m_DataList.empty())
		{
			bool isNew = true;
			int checkedNums = 0;
			if (m_DataList.size() >= m_DataListMaxNum) return;
			//使用迭代器指针遍历数据是否重复
			deque<DeData*>::iterator *pIter = new deque<DeData*>::iterator;
			if (NULL == pIter)
			{
				return;
			}
			for (*pIter = m_DataList.begin(); *pIter != m_DataList.end(); (*pIter)++)
			{
				if (checkedNums >= CheckMaxNums)
				{
					break;
				}
				if (***pIter == *pTmpDeData)
				{
					isNew = false;
					break;
				}
				checkedNums++;
			}
			if (NULL != pIter)
			{
				delete pIter;
				pIter = NULL;
			}

			if (isNew)
			{
				m_DataList.push_front(pTmpDeData);
			}

		}
		else
		{
			m_DataList.push_front(pTmpDeData);
		}
		LeaveCriticalSection(&m_section);
		//double frameTime = GetFrameTime();
		//printf("FrameTime = %f ms\n", frameTime);
	}
}

#pragma endregion DeviceServer