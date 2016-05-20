#include <WinSock2.h> // List front to avoid <WinSock.h> previous
#pragma comment(lib,"Ws2_32.lib")
#include "Device.h"
#include "json.h"
using namespace std;


#pragma region DeSetting

DeSetting::DeSetting()
{
	deviceName[0] = 0;
	devicePath[0] = 0;
	deviceMAC[0] = 0;
	deviceIP[0] = 0;
	readUpdate = true;
	writeForce = false;
	//serverDataSize = 0;
	//serverTimes = -1;
	//serverFrameTime = 0;
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

bool DeSetting::CopyTo(DeSetting& setting)
{
	try
	{
		strcpy_s(setting.deviceName, sizeof(setting.deviceName), deviceName);
		strcpy_s(setting.devicePath, sizeof(setting.devicePath), devicePath);
		strcpy_s(deviceMAC, sizeof(deviceMAC), setting.deviceMAC);
		strcpy_s(deviceIP, sizeof(deviceIP), setting.deviceIP);
		setting.readUpdate = readUpdate;
		setting.writeForce = writeForce;
		//setting.serverDataSize = serverDataSize;
		//setting.serverTimes = serverTimes;
		//setting.serverFrameTime = serverFrameTime;
		//setting.ServerIncludeTaskTime = ServerIncludeTaskTime;
	}
	catch (DWORD& e)
	{
		return false;
	}
	catch (exception& e)
	{
		return false;
	}
	return true;
}

bool DeSetting::CopyTo(MEGASTR& setting)
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
		root["readUpdate"] = readUpdate;
		root["writeForce"] = writeForce;
		//root["serverDataSize"] = serverDataSize;
		//root["serverTimes"] = serverTimes;
		//root["serverFrameTime"] = serverFrameTime;
		//root["ServerIncludeTaskTime"] = ServerIncludeTaskTime;
		strcpy_s(setting, sizeof(setting), writer.write(root).data());
	}
	catch (DWORD& e)
	{
		return false;
	}
	catch (exception& e)
	{
		return false;
	}
	return true;
}

bool DeSetting::CopyFrom(const DeSetting& setting)
{
	try
	{
		strcpy_s(deviceName, sizeof(deviceName), setting.deviceName);
		strcpy_s(devicePath, sizeof(devicePath), setting.devicePath);
		strcpy_s(deviceMAC, sizeof(deviceMAC), setting.deviceMAC);
		strcpy_s(deviceIP, sizeof(deviceIP), setting.deviceIP);
		readUpdate = setting.readUpdate;
		writeForce = setting.writeForce;
		//serverDataSize = setting.serverDataSize;
		//serverTimes = setting.serverTimes;
		//serverFrameTime = setting.serverFrameTime;
		//ServerIncludeTaskTime = setting.ServerIncludeTaskTime;
	}
	catch (DWORD& e)
	{
		return false;
	}
	catch (exception& e)
	{
		return false;
	}
	return true;
}

bool DeSetting::CopyFrom(const MEGASTR& setting)
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
		readUpdate = root["readUpdate"].asBool();
		writeForce = root["writeForce"].asBool();
		//serverDataSize = root["serverDataSize"].asInt();
		//serverTimes = root["serverTimes"].asInt64();
		//serverFrameTime = root["serverFrameTime"].asInt64();
		//ServerIncludeTaskTime = root["ServerIncludeTaskTime"].asBool();
	}
	catch (DWORD& e)
	{
		return false;
	}
	catch (exception& e)
	{
		return false;
	}
	return true;
}

void DeSetting::Clear()
{
	deviceName[0] = 0;
	devicePath[0] = 0;
	deviceMAC[0] = 0;
	deviceIP[0] = 0;
	//serverDataSize = 0;
	//serverTimes = -1;
	//serverFrameTime = 0;
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

int DeSetting::GetAvailable(SettingSet<MEGASTR>& deSettings)
{
	try
	{	
		DeSetting tmpSetting;
		WSADATA wsaData;
		char *ip = 0;//定义IP地址变量
		hostent *hostinfo = 0;

		//调用MAKEWORD（）获得Winsock版本的正确值，用于加载Winsock库
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0) {
			//现在是加载Winsock库，如果WSAStartup（）函数返回值为0，说明加载成功，程序可以继续
			if (gethostname(tmpSetting.deviceName,
				sizeof(tmpSetting.deviceName)) == 0)
			{
				//如果成功地将本地主机名存放入由name参数指定的缓冲区中
				hostinfo = gethostbyname(tmpSetting.deviceName);
				if (hostinfo != 0)
				{
					//这是获取主机名，如果获得主机名成功的话，将返回一个指针，指向hostinfo，hostinfo
					//为PHOSTENT型的变量，下面即将用到这个结构体
					ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
					strcpy_s(tmpSetting.deviceIP, sizeof(tmpSetting.deviceIP),ip);
				}
			}
			WSACleanup(); //卸载Winsock库，并释放所有资源
		}
		tmpSetting.CopyTo(deSettings.SettingData[0]);
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

template <typename T>
DeData<T>::DeData()
{
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

template <typename T>
DeData<T>::~DeData()
{
}

template <typename T>
DeData<T>::DeData(const DeData<T> & dataIn)
{
	CopyFrom(dataIn);
}

template <typename T>
DeData<T>& DeData<T>::operator=(const DeData<T> & dataIn)
{
	CopyFrom(dataIn);
	return *this;
}

template <typename T>
bool DeData<T>::CopyTo(DeData<T> & dataIn)
{
	try
	{
		__if_exists(T::operator=)
		{
			dataIn.data = data;
		}
		__if_not_exists(T::operator=)
		{
			memcpy_s(&dataIn.data, sizeof(dataIn.data), &data, , sizeof(data));
		}
		dataIn.data = data;
		dataIn.createTime = createTime;
		dataIn.updateTime = updateTime;
		strcpy_s(dataIn.dataTimeStr, sizeof(dataIn.dataTimeStr), dataTimeStr);
		dataIn.proceessTag = proceessTag;
		return true;
	}
	catch (DWORD& e)
	{
		return false;
	}
	catch (exception& e)
	{
		return false;
	}
}

template <typename T>
bool DeData<T>::CopyFrom(const DeData<T> & dataIn)
{
	try
	{
		__if_exists(T::operator=)
		{
			data = dataIn.data;
		}
		__if_not_exists(T::operator=)
		{
			memcpy_s(&data, sizeof(data), &dataIn.data, sizeof(dataIn.data));
		}
		createTime = dataIn.createTime;
		updateTime = dataIn.createTime;
		strcpy_s(dataTimeStr, sizeof(dataTimeStr), dataIn.dataTimeStr);
		proceessTag = dataIn.proceessTag;
		return true;
	}
	catch (DWORD& e)
	{
		return false;
	}
	catch (exception& e)
	{
		return false;
	}
}

template <typename T>
bool DeData<T>::operator==(const DeData & dataIn)
{
	try
	{
		// Duplicate object if dataTimeStr and proceessTag are same
		return (0 == strcmp(dataTimeStr, dataIn.dataTimeStr) && proceessTag== dataIn.proceessTag);
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

template <typename T>
double DeData<T>::GetLife()
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

template <typename T>
Device<T>::Device()
{
	m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_INVALID;
	p_DeSetting = 0;
}

template <typename T>
Device<T>::~Device()
{
	Unitial();
}

template <typename T>
bool Device<T>::Initial(MEGASTR & setting)
{	
	//...
	//Do some thing before this
	m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_UNITIALED;
	if (setting[0] == 0) return false;// para error
	if (p_DeSetting != 0) return false;//Already initialed
	if (p_DeSetting == 0)
	{
		p_DeSetting = new DeSetting();//override here
	}
	// copy from json setting string
	if (!p_DeSetting->CopyFrom(setting))
	{
		delete p_DeSetting;
		p_DeSetting = 0;
		return false;
	}
	//Do some thing after this
	return true;
	//...
	//m_hDevice = ...;
	//m_DeStatus = DE_INITIALED
}

template <typename T>
void Device<T>::Unitial()
{
	//...
	//Do some thing before this	
	m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_UNITIALED;
	if (p_DeSetting!=0)
	{
		delete p_DeSetting;//override here
	    p_DeSetting = 0;
	}
}

template <typename T>
bool Device<T>::Set(MEGASTR& setting)
{	
	//...
	//Do some thing before this
	m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_UNITIALED;
	if (setting[0] == 0) return false;// para error
	if (p_DeSetting != 0) return false;//Already initialed
	if (p_DeSetting == 0)
	{
		p_DeSetting = new DeSetting();//override here
	}
	// copy from json setting string
	if (!p_DeSetting->CopyFrom(setting))
	{
		return false;//Set failed
	}
	//Do some thing after this
	//...
	//m_hDevice = ...;
	//m_DeStatus = DE_INITIALED
	return true;
}

template <typename T>
bool Device<T>::ReSet(MEGASTR& setting)
{
	Unitial();
	return Initial(setting);
}

template <typename T>
bool Device<T>::Read(DeData<T>& deData, bool update/* = true*/)
{
	if (p_DeSetting == 0) return false;// Not initialed device.
	try
	{	
		if (update)
		{
			//Do something here for update from physical device
			//...
			// Recode read time.
			GetLocalTime(&m_DeData->updateTime);
			sprintf_s(m_DeData->dataTimeStr, sizeof(m_DeData->dataTimeStr),
				"%d-%d-%d %d:%d:%d:%d",
				m_DeData->updateTime.wYear,
				m_DeData->updateTime.wMonth,
				m_DeData->updateTime.wDay,
				m_DeData->updateTime.wHour,
				m_DeData->updateTime.wMinute,
				m_DeData->updateTime.wSecond,
				m_DeData->updateTime.wMilliseconds);
		}
		if (deData.CopyFrom(m_DeData))
		{
			return true;
		}
		return false;
	}
	catch (DWORD& e)
	{
		return false;
	}
	catch (exception& e)
	{
		return false;
	}
}

template<typename T>
bool Device<T>::operator >> (DeData<T>& deData)
{
	if (p_DeSetting == 0) return false;
	return Read(deData, p_DeSetting->readUpdate);
}

template <typename T>
bool Device<T>::Write(const DeData<T>& deData, bool force/* = false*/)
{
	if (p_DeSetting == 0) return false;// Not initialed device.
	try
	{
		if (deData.CopyFrom(m_DeData))
		{
			return true;
		}
		//Do something here for update into physical device
		//...
		//if (force)
		//{
		//}
		//else
		//{

		//}
		return false;
	}
	catch (DWORD& e)
	{
		return false;
	}
	catch (exception& e)
	{
		return false;
	}
}

template<typename T>
bool Device<T>::operator<<(const DeData<T>& deData)
{
	if (p_DeSetting == 0) return false;
	return Write(deData, p_DeSetting->writeForce);
}

#pragma endregion Device


#pragma region SeverSetting

ServerSetting::ServerSetting()
{
	serverDataSize = 0;
	serverTimes = -1;
	serverFrameTime = 0;
	serverQueueBlock = false;
	//serverReadUpdate = true;
	//serverWriteForce = false;
	//serverForceQueue = true;
}

ServerSetting::~ServerSetting()
{
}

ServerSetting::ServerSetting(const ServerSetting& setting)
{
	CopyFrom(setting);
}

ServerSetting::ServerSetting(const MEGASTR& setting)
{
	CopyFrom(setting);
}

ServerSetting& ServerSetting::operator=(const ServerSetting& setting)
{
	CopyFrom(setting);
	return *this;
}

ServerSetting& ServerSetting::operator=(const MEGASTR& setting)
{
	CopyFrom(setting);
	return *this;
}

bool ServerSetting::CopyTo(ServerSetting& setting)
{
	if (!DeSetting::CopyTo((DeSetting&)setting)) return false;
	setting.serverDataSize = serverDataSize;
	setting.serverTimes = serverTimes;
	setting.serverFrameTime = serverFrameTime;
	setting.serverQueueBlock = serverQueueBlock;
	return true;
}

bool ServerSetting::CopyTo(MEGASTR& setting)
{
	if (!DeSetting::CopyTo(setting)) return false;
	try
	{
		//setting[0] = 0;
		Json::FastWriter writer;
		Json::Value root;
		root["serverDataSize"] = serverDataSize;
		root["serverTimes"] = serverTimes;
		root["serverFrameTime"] = serverFrameTime;
		root["serverQueueBlock"] = serverQueueBlock;
		//root["serverReadUpdate"] = serverReadUpdate;
		//root["serverWriteForce"] = serverWriteForce;
		//root["serverForceQueue"] = serverForceQueue;
		//root["ServerIncludeTaskTime"] = ServerIncludeTaskTime;
		strcpy_s(setting, sizeof(setting), writer.write(root).data());
	}
	catch (DWORD& e)
	{
		return false;
	}
	catch (exception& e)
	{
		return false;
	}
	return true;
}

bool ServerSetting::CopyFrom(const ServerSetting& setting)
{
	if (!DeSetting::CopyFrom((DeSetting&)setting)) return false;
	serverDataSize = setting.serverDataSize;
	serverTimes = setting.serverTimes;
	serverFrameTime = setting.serverFrameTime;
	serverQueueBlock = setting.serverQueueBlock;
	return true;
}

bool ServerSetting::CopyFrom(const MEGASTR& setting)
{
	if (!DeSetting::CopyFrom(setting)) return false;
	try
	{
		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(setting, root, false))
		{
			throw DE_ERR;
		}
		serverDataSize = root["serverDataSize"].asInt();
		serverTimes = root["serverTimes"].asInt64();
		serverFrameTime = root["serverFrameTime"].asInt64();
		serverQueueBlock = root["serverQueueBlock"].asBool();
		//serverReadUpdate = root["serverReadUpdate"].asBool();
		//serverWriteForce = root["serverWriteForce"].asBool();
		//serverForceQueue = root["serverForceQueue"].asBool();
		//ServerIncludeTaskTime = root["ServerIncludeTaskTime"].asBool();
	}
	catch (DWORD& e)
	{
		return false;
	}
	catch (exception& e)
	{
		return false;
	}
	return true;
}

#pragma endregion SeverSetting


#pragma region DeviceServer


template <typename T>
bool DeviceServer<T>::Read(DeData<T>& deData, bool update/* = true*/)
{
	bool reVal = false;
	if (p_DeSetting == 0) return reVal;// Not initialed device.

	try
	{	
		if (update)
		{
			if (!((ServerSetting*)p_DeSetting)->serverQueueBlock)
			{
				if (m_DataList.Writeable()) return reVal;
			}
			m_DataList.EnterWrite();
			//EnterCriticalSection(&m_section);
			if (m_DataList.GetLength() == 0) return reVal;// Empty data.
			if (deData.CopyFrom(m_DataList.GetHead()))
			{
				reVal = true;
			}
			//Do something here for update the Queue
			//...
			if (m_DataList.GetLength() >= 0)
			{
				m_DataList.DelHead();
			}
			m_DataList.LeaveWrite();
		} 
		else
		{
			if (!((ServerSetting*)p_DeSetting)->serverQueueBlock)
			{
				if (m_DataList.Readable()) return reVal;
			}
			m_DataList.EnterRead();
			//EnterCriticalSection(&m_section);
			if (m_DataList.GetLength() == 0) return reVal;// Empty data.
			if (deData.CopyFrom(m_DataList.GetHead()))
			{
				reVal = true;
			}
			m_DataList.LeaveRead();
		}
		//LeaveCriticalSection(&m_section);
		return reVal;
	}
	catch (DWORD& e)
	{
		m_DataList.LeaveWrite();
		m_DataList.LeaveRead();
		return false;
	}
	catch (exception& e)
	{
		m_DataList.LeaveWrite();
		m_DataList.LeaveRead();
		return false;
	}
}

template <typename T>
bool DeviceServer<T>::Write(const DeData<T>& deData, bool force/* = false*/)
{
	if (p_DeSetting == 0) return false;// Not initialed device.
	try
	{
		//Do something here for update into Queue
		//...
		if (!m_DataList.Writeable()) return false;
		//
		m_DataList.EnterWrite();
		//EnterCriticalSection(&m_section);
		if (!m_DataList.GetLength() <= 0)
		{
			bool isNew = true;
			int checkedNums = 0;
			//Find if data is duplicate
			for (int i = m_DataList.GetLength(); i >= 0; i--)
			{
				if (checkedNums >= CheckMaxNums) break;
				if (TmpDeData == m_DataList.GetData(i))
				{
					isNew = false;
					break;
				}
				checkedNums++;
			}
			if (!isNew)
			{
				m_DataList.LeaveWrite();
				//LeaveCriticalSection(&m_section);
				return false;
			}
		}
		if (force)
		{
			if (m_DataList.ForcTail(deData))
			{
				// Do something here
				//...
				m_DataList.LeaveWrite();
				//LeaveCriticalSection(&m_section);
				return true;
			}
		}
		else
		{
			if (m_DataList.AddTail(deData))
			{
				// Do something here
				//...
				m_DataList.LeaveWrite();
				//LeaveCriticalSection(&m_section);
				return true;
			}
		}
		m_DataList.LeaveWrite();
		//LeaveCriticalSection(&m_section);
		return false;
	}
	catch (DWORD& e)
	{
		m_DataList.LeaveWrite();
		return false;
	}
	catch (exception& e)
	{
		m_DataList.LeaveWrite();
		return false;
	}
}

template <typename T>
DeviceServer<T>::DeviceServer(HANDLE hParent)
{
	p_Device = 0;
	m_hDevice = 0;
	m_DeStatus = DE_UNITIALED;
}

template <typename T>
DeviceServer<T>::~DeviceServer()
{
	Unitial();
	m_DataList.Unitial();
}

template <typename T>
bool DeviceServer<T>::Initial(MEGASTR & setting)
{
	ForceEnd();
	if (m_hThread != INVALID_HANDLE_VALUE) return false;//End failed
	//...
	//Do some thing before this
	m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_UNITIALED;
	if (setting[0] == 0) return false;// para error
	if (p_DeSetting != 0) return false;//Already initialed
	if (p_DeSetting == 0)
	{
		p_DeSetting = new ServerSetting();//override here
	}
	// copy from json setting string
	if (!p_DeSetting->CopyFrom(setting))
	{
		delete p_DeSetting;
		p_DeSetting = 0;
		return false;
	}
	//Do some thing after this
	//...
	
	try
	{
		EnterCriticalSection(&m_section);
		m_DataList.Unitial();
		m_DataList.Initial(MAX(((ServerSetting*)p_DeSetting)->serverDataSize,2));
		LeaveCriticalSection(&m_section);
		Create(((ServerSetting*)p_DeSetting)->serverTimes, 
			((ServerSetting*)p_DeSetting)->serverFrameTime, true);
	}
	catch (DWORD& e)
	{
		delete p_DeSetting;
		p_DeSetting = 0;
		return false;
	}
	catch (exception& e)
	{
		delete p_DeSetting;
		p_DeSetting = 0;
		return false;
	}
	m_hDevice = m_hThread;
	m_DeStatus = DE_INITIALED;
	return true;
}

template <typename T>
void DeviceServer<T>::Unitial()
{
	ForceEnd();
	DetachDevice();
EnterCriticalSection(&m_section);
	m_DataList.Unitial();
LeaveCriticalSection(&m_section);
	//...
	//Do some thing before this	
	m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_UNITIALED;
	if (p_DeSetting != 0)
	{
		delete p_DeSetting;//override here
		p_DeSetting = 0;
	}
}

template <typename T>
bool DeviceServer<T>::Set(MEGASTR & setting)
{
	Suspend();
	//...
	//Do some thing before this
	m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_UNITIALED;
	if (setting[0] == 0) return false;// para error
	if (p_DeSetting == 0) return false;//Not initialed
	// copy from json setting string
	if (!p_DeSetting->CopyFrom(setting))
	{
		return false;//Set failed
	}
	//Do some thing after this
	//...
	try
	{
		m_times = ((ServerSetting*)p_DeSetting)->serverTimes;
		m_waitTime = ((ServerSetting*)p_DeSetting)->serverFrameTime;
		m_includeTaskTime = true;
		Resume();
	}
	catch (DWORD& e)
	{
		return false;
	}
	catch (exception& e)
	{
		return false;
	}
	m_hDevice = m_hThread;
	m_DeStatus = DE_INITIALED;
	return true;
}

template <typename T>
bool DeviceServer<T>::ReSet(MEGASTR & setting)
{
	Unitial();
	return Initial(setting);
}

template <typename T>
bool DeviceServer<T>::AttachDevice(Device<T>* pDevice)
{
	try
	{
		if (p_DeSetting == 0 || m_hDevice == INVALID_HANDLE_VALUE
			|| m_DeStatus != DE_INITIALED) return false;//Server not initialed
		if (pDevice == 0) return false;//para error
		if (pDevice->m_DeStatus != Device::DE_INITIALED
			|| pDevice->p_DeSetting == 0) return false;//Device error or not initialed
		Suspend();
		EnterCriticalSection(&m_section);
		p_Device = pDevice;//Defined outside of this class
		LeaveCriticalSection(&m_section);
	}
	catch (DWORD& e)
	{
		LeaveCriticalSection(&m_section);
		return false;
	}
	catch (exception& e)
	{
		LeaveCriticalSection(&m_section);
		return false;
	}
	return  m_ThStatus == Thread::TH_SUSPEND;
}

template <typename T>
void DeviceServer<T>::DetachDevice()
{
	try
	{
		Suspend();
		EnterCriticalSection(&m_section);
		//p_Device->Unitial();//Opreate p_Device outside of this class
		p_Device = 0;
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

template <typename T>
bool DeviceServer<T>::SetDevice(MEGASTR& setting)
{
	try
	{
		if (p_DeSetting == 0 || m_hDevice == INVALID_HANDLE_VALUE
			|| m_DeStatus != DE_INITIALED) return false;//Server not initialed
		if (p_Device == 0) return false;//Device not Attached
		if (p_Device->m_DeStatus != Device::DE_INITIALED
			|| p_Device->p_DeSetting == 0) return false;//Device error or not initialed
		EnterCriticalSection(&m_section);
		Suspend();
		if (p_Device->Set(setting))
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

template <typename T>
bool DeviceServer<T>::ReSetDevice(MEGASTR& setting)
{
	try
	{
		if (p_DeSetting == 0 || m_hDevice == INVALID_HANDLE_VALUE
			|| m_DeStatus != DE_INITIALED) return false;//Server not initialed
		if (p_Device == 0) return false;//Device not Attached
		if (p_Device->m_DeStatus != Device::DE_INITIALED
			|| p_Device->p_DeSetting == 0) return false;//Device error or not initialed
		Suspend();		
		EnterCriticalSection(&m_section);
		if (p_Device->ReSet(setting))
		{
			m_DataList.Clear();
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

template <typename T>
bool DeviceServer<T>::StartServer()
{
	try
	{
		if (p_DeSetting == 0 || m_hDevice == INVALID_HANDLE_VALUE
			|| m_DeStatus != DE_INITIALED) return false;//Server not initialed
		//if (p_Device == 0) return false;//Device not Attached
		//if (p_Device->m_DeStatus != Device::DE_INITIALED
		//	|| p_Device->p_DeSetting == 0) return false;//Device error or not initialed
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

template <typename T>
bool DeviceServer<T>::StopServer()
{
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

#pragma endregion DeviceServer


#pragma region DeviceReader

#define  CheckMaxNums  8
template <typename T>
void DeviceReader<T>::Task()
{
	if (p_DeSetting == 0 || m_hDevice == INVALID_HANDLE_VALUE
		|| m_DeStatus != DE_INITIALED) return false;//Server not initialed
	if (p_Device == 0) return false;//Device not Attached
	if (p_Device->m_DeStatus != Device::DE_INITIALED
		|| p_Device->p_DeSetting == 0) return false;//Device error or not initialed

	DeData TmpDeData;
	if (p_Device->Read(TmpDeData, p_Device->p_DeSetting->readUpdate))
	{
			Write(TmpDeData, p_DeSetting->writeForce);
		//double frameTime = GetFrameTime();
		//printf("FrameTime = %f ms\n", frameTime);
	}
}

#pragma endregion DeviceReader


#pragma region DeviceWriter

template <typename T>
void DeviceWriter<T>::Task()
{
	if (p_DeSetting == 0 || m_hDevice == INVALID_HANDLE_VALUE
		|| m_DeStatus != DE_INITIALED) return false;//Server not initialed
	if (p_Device == 0) return false;//Device not Attached
	if (p_Device->m_DeStatus != Device::DE_INITIALED
		|| p_Device->p_DeSetting == 0) return false;//Device error or not initialed

	DeData TmpDeData;
	if (Read(TmpDeData, p_DeSetting->readUpdate))
	{
		p_Device->Write(TmpDeData, p_Device->p_DeSetting->writeForce);
		//double frameTime = GetFrameTime();
		//printf("FrameTime = %f ms\n", frameTime);
	}
}

#pragma endregion DeviceWriter

