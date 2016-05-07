///////////////////////////////////////////////////
/** \file Device.h
 *  \brief Base class of all devices,with it's server.
 *
 *  \note
 *
 ** Status:
 *
 *          json string
 *                │
 *                ∨     ┌─────┐
 *      DeSetting(__)─＞│DeSetting │
 *                       └─────┘
 *                           │
 *                           ∨     ┌────┐
 *      Device       Initial(&_)─＞│ Device │─＞ Watch(__)
 *                                  └────┘           │
 *                                       │                ∨
 *                                       ∨           ┌────┐
 *      DeviceServer           AddDevice(*_)          │ DeData │
 *                                                    └────┘
 *                                                         │
 *                                                         ∨
 *                        Task(){ Queue<DeData>::ForceTail(__) }
 *     
 *     DeviceServer deviceServer;
 *     DeSetting deSetting(jsonstring);
 *     Device* device = new Camera();
 *     device->Initial(&deSetting);
 *     deviceServer.AddDevice(device);
 *     deviceServer.StartDevice();
 *     ...
 *     deviceServer.StopDevice();
 *     deviceServer.DelDevice();
 * 
 *  \author Leon Contact: towanglei@163.com
 *  \copyright TMTeam
 *  \version 1.0
 *  \History:
 *     Leon 2016/04/29 10:31 build
 */
///////////////////////////////////////////////////
#pragma once
#include "Thread.h"
#include "CommonDefine.h"
#include "Queue.h"
#include <exception>

//////////////////////////////////////////////////
/** \def DE_ERR
 *  \brief Error code of device
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/05/07 17:58
 */
#ifndef DE_ERR
#define DE_OK        TMT_NORMAL
#define DE_ERR       ERR
#endif
//////////////////////////////////////////////////

//////////////////////////////////////////////////
/** \struct DeSetting
 *  \brief Base struct to store setting para.
 *  \note
 *   Decode para from joson string\n
 *   Read and store by method in derived class.
 *  \author Leon Contact: towanglei@163.com
 */
#ifndef TMTV_MEGASTRLEN
#define TMTV_MEGASTRLEN	    4096//字符串最大长度
typedef char	MEGASTR[TMTV_MEGASTRLEN];
#endif
#ifndef DESETTING
#define DESETTING
struct DeSetting
{
	SHORTSTR DeviceName = "";///< Device name
	PATHSTR DevicePath = ""; ///< Device path
	int ServerDataSize = 8; ///< Data list size
	long ServerTimes = -1; ///< Thread run times
	long ServerWaitTime = 0; ///< Thread wait time
	bool ServerIncludeTaskTime = true; ///< If task time included in wait times

	/// Default constructor
	DeSetting()
	{
		DeviceName[0] = 0;
		DevicePath[0] = 0;
		ServerDataSize = 0;
		ServerTimes = -1;
		ServerWaitTime = 0;
		ServerIncludeTaskTime = true;
	};

	/// Deep copy constructor
	DeSetting(const DeSetting& setting) 
	{
		strcpy_s(DeviceName, sizeof(DeviceName), setting.DeviceName);
		strcpy_s(DevicePath, sizeof(DevicePath), setting.DevicePath);
		ServerDataSize = setting.ServerDataSize;
		ServerTimes = setting.ServerTimes;	
		ServerWaitTime = setting.ServerWaitTime;
		ServerIncludeTaskTime = setting.ServerIncludeTaskTime;
	};
	/// Deep copy constructor
	DeSetting(const MEGASTR setting);
	/// Deep copy operator=
	DeSetting& operator=(const DeSetting& setting)
	{
		strcpy_s(DeviceName, sizeof(DeviceName), setting.DeviceName);
		strcpy_s(DevicePath, sizeof(DevicePath), setting.DevicePath);
		ServerDataSize = setting.ServerDataSize;
		ServerTimes = setting.ServerTimes;
		ServerWaitTime = setting.ServerWaitTime;
		ServerIncludeTaskTime = setting.ServerIncludeTaskTime;
		return *this;
	};
	/// Deep copy operator=
	virtual DeSetting& operator=(const MEGASTR setting);
	/// Deep copy to para
	void CopyTo(DeSetting& setting)
	{
		strcpy_s(setting.DeviceName, sizeof(setting.DeviceName), DeviceName);
		strcpy_s(setting.DevicePath, sizeof(setting.DevicePath), DevicePath);
		setting.ServerDataSize = ServerDataSize;
		setting.ServerTimes = ServerTimes;
		setting.ServerWaitTime = ServerWaitTime;
		setting.ServerIncludeTaskTime = ServerIncludeTaskTime;
	}
	/// Deep copy from para
	void CopyFrom(const DeSetting& setting)
	{
		strcpy_s(DeviceName, sizeof(DeviceName), setting.DeviceName);
		strcpy_s(DevicePath, sizeof(DevicePath), setting.DevicePath);
		ServerDataSize = setting.ServerDataSize;
		ServerTimes = setting.ServerTimes;
		ServerWaitTime = setting.ServerWaitTime;
		ServerIncludeTaskTime = setting.ServerIncludeTaskTime;
	}
	/// Deep copy to para
	virtual void CopyTo(MEGASTR setting);
	/// Deep copy from para
	virtual void CopyFrom(const MEGASTR setting);
};
#endif
///////////////////////////////////////////////////

//////////////////////////////////////////////////
/** \struct DeData
 *  \brief Base struct to store IO data.
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/05/02 11:38
 */
struct DeData 
{
	enum { DE_DATAMAXSIZE = 10240///< Data max size;
	};
	char data[DE_DATAMAXSIZE];///< Data body.
	long dataLen = 0;///< Data actual length.
	TINYSTR dataTime = "2000-01-01 00:00:00:00";///< Data time.
	DWORD dataType;///< Data type defined by each data
	HUGESTR dataReserve;///< Data reserved
	DeData()
	{
		data[0] = 0;
		dataLen = 0;
		//sprintf_s(dataTime,sizeof(dataTime), "2000-01-01 00:00:00:00");
		dataTime[0] = 0;
		dataType = 0;
		dataReserve[0] = 0;
	}
	~DeData(){}

	/// Deep copy constructor
	DeData(const DeData& dataIn)
	{
		memcpy_s(data, sizeof(data), dataIn.data, dataLen);
		dataLen = dataIn.dataLen;
		strcpy_s(dataTime, sizeof(dataTime), dataIn.dataTime);
		dataType = dataIn.dataType;
		memcpy_s(dataReserve, sizeof(dataReserve), 
			dataIn.dataReserve, sizeof(dataIn.dataReserve));
	};
	/// Deep copy operator=
	DeData& operator=(const DeData& dataIn)
	{
		memcpy_s(data, sizeof(data), dataIn.data, dataLen);
		dataLen = dataIn.dataLen;
		strcpy_s(dataTime, sizeof(dataTime), dataIn.dataTime);
		dataType = dataIn.dataType;
		memcpy_s(dataReserve, sizeof(dataReserve), 
			dataIn.dataReserve, sizeof(dataIn.dataReserve));
		return *this;
	};
	/// Deep copy to para
	void CopyTo(DeData& dataIn)
	{
		memcpy_s(dataIn.data, sizeof(dataIn.data), data, dataIn.dataLen);
		dataIn.dataLen = dataLen;
		strcpy_s(dataIn.dataTime, sizeof(dataIn.dataTime), dataTime);
		dataIn.dataType = dataType;
		memcpy_s(dataIn.dataReserve, sizeof(dataIn.dataReserve), 
			dataReserve, sizeof(dataReserve));
	}
	/// Deep copy from para
	void CopyFrom(const DeData& dataIn)
	{
		memcpy_s(data, sizeof(data), dataIn.data, dataLen);
		dataLen = dataIn.dataLen;
		strcpy_s(dataTime, sizeof(dataTime), dataIn.dataTime);
		dataType = dataIn.dataType;
		memcpy_s(dataReserve, sizeof(dataReserve), 
			dataIn.dataReserve, sizeof(dataIn.dataReserve));
	}

	/// Compare operator==
	bool operator== (const DeData& dataIn)
	{
		return strcmp(dataTime, dataIn.dataTime);
	};
};
///////////////////////////////////////////////////


///////////////////////////////////////////////////
/** \class Device 
 *  \brief Base class of all devices.
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/04/29 10:34
 */
class Device
{
public:
    /// Handle of device
	HANDLE m_hDevice;
	//////////////////////////////////////////////////
	 /** \enum Status
	 *  \brief Status of device.
	 *  \author Leon Contact: towanglei@163.com
	 */
	enum DESTATUS 
	{
		DE_INVALID = 0, ///< Invalid device e.g. not exist
		DE_UNITIALED, ///< Available device but not initialed 
		DE_INITIALED ///< Initialed device
	};
	//////////////////////////////////////////////////

	/// Status of device
	DESTATUS m_DeStatus;
	/// Setting of device
	DeSetting m_DeSetting;
	Device();
	~Device() { Unitial(); };

	///////////////////////////////////////////////////
	/** \fn  Initial
	 *  \brief Initial device
	 *  \param[in] DeSetting setting
	 *  \return bool
	 */
	virtual bool Initial(DeSetting& setting) = 0;
	//{
	//	m_hDevice = INVALID_HANDLE_VALUE;
	//	m_DeStatus = DE_INITIALED;
	//	m_DeSetting = setting;
	//	return false;
	//}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	/** \fn  Unitial
	 *  \brief Unitial device
	 */
	virtual void Unitial() = 0;
	//{
	//	m_DeStatus = DE_UNITIALED;
	//}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	/** \fn  Set
	*  \brief Set device para without stop
	*  \param[in] DeSetting setting
	*  \return bool
	*/
	virtual bool Set(DeSetting& setting) = 0;
	//{
	//	m_hDevice = INVALID_HANDLE_VALUE;
	//	m_DeStatus = DE_INVALID;
	//	m_DeSetting = setting;
	//	return false;
	//}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	/** \fn  ReSet
	*  \brief ReSet device, stop and restart
	 *  \param[in] DeSetting setting
	 *  \return bool
	 */
	virtual bool ReSet(DeSetting& setting)
	{
		Unitial();
		return Initial(setting);
	}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	/** \fn  Watch
	*  \brief Watch device
	*  \param[in] DeSetting setting
	*  \return bool
	*/
	virtual bool Watch(DeData& setting) = 0;
	///////////////////////////////////////////////////
};
///////////////////////////////////////////////////

///////////////////////////////////////////////////
/** \class DeviceServer : public Thread, public Device
 *  \brief Base class of all devices server.
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/04/29 23:09
 */
class DeviceServer : public Thread
{
public:
	Device* p_Device;
	Queue<DeData> m_DeDataList;
public:
	DeviceServer(HANDLE  hParent = 0) 
	{ 
		p_Device = 0;
	};
	~DeviceServer() 
	{
		if (p_Device!=0)
		{
			p_Device->Unitial();
			delete p_Device;
		}
		m_DeDataList.Unitial();
	};

	///////////////////////////////////////////////////
	/** \fn  AddDevice
	 *  \brief Add device to server
	 *  \param[in] DeSetting setting
	 *  \return bool
	 */
	virtual bool AddDevice(Device* device)
	{
		if (device == 0) return false;
		if (device->m_DeStatus != Device::DE_INITIALED) return false;
		EnterCriticalSection(&m_section);
		ForceEnd();
		p_Device = device;
		m_DeDataList.Initial(MAX(device->m_DeSetting.ServerDataSize,8));
		Create(device->m_DeSetting.ServerTimes, device->m_DeSetting.ServerWaitTime,
			device->m_DeSetting.ServerIncludeTaskTime);
		LeaveCriticalSection(&m_section);
		return  m_ThStatus == Thread::TH_SUSPEND;
	}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	/** \fn  DelDevice
	 *  \brief Delete device from server, 
	 *         but device object is not delete in function
	 *  \return void
	 */
	virtual void DelDevice()
	{
		EnterCriticalSection(&m_section);
		ForceEnd();
		if (p_Device == 0) return;
		p_Device->Unitial();
		p_Device = 0;
		m_DeDataList.Unitial();
		LeaveCriticalSection(&m_section);
	}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	/** \fn  StartServer
	 *  \brief Start server to run the added device
	 *  \return bool
	 */
	virtual bool StartServer()
	{
		if (p_Device == 0) return false;
		if (p_Device->m_DeStatus != Device::DE_INITIALED) return false;
		EnterCriticalSection(&m_section);
		Resume();
		LeaveCriticalSection(&m_section);
		return m_ThStatus == Thread::TH_RUNNING;
	}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	/** \fn  StopServer
	*  \brief Stop server without delete anything
	*  \return bool
	*/
	virtual bool StopServer()
	{
		if (p_Device == 0) return false;
		if (p_Device->m_DeStatus != Device::DE_INITIALED) return false;
		EnterCriticalSection(&m_section);
		Resume();
		LeaveCriticalSection(&m_section);
		return m_ThStatus==Thread::TH_SUSPEND;
	}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	/** \fn  Set
	*  \brief Set device in running server
	*  \param[in] DeSetting setting
	*  \return bool
	*/
	virtual bool Set(DeSetting& setting)
	{
		if (p_Device == 0) return false;
		if (p_Device->m_DeStatus != Device::DE_INITIALED) return false;
		EnterCriticalSection(&m_section);
		Suspend();
		p_Device->Set(setting);
		Resume();
		LeaveCriticalSection(&m_section);
		return m_ThStatus == Thread::TH_RUNNING;
	}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	/** \fn  ReSet
	*  \brief Stop and reset device in server,need StartServer()
	*  \param[in] DeSetting setting
	*  \return bool
	*/
	virtual bool ReSet(DeSetting& setting)
	{
		if (p_Device == 0) return false;
		if (p_Device->m_DeStatus != Device::DE_INITIALED) return false;
		EnterCriticalSection(&m_section);
		ForceEnd();
		p_Device->ReSet(setting);
		Create(p_Device->m_DeSetting.ServerTimes, p_Device->m_DeSetting.ServerWaitTime,
			p_Device->m_DeSetting.ServerIncludeTaskTime);
		LeaveCriticalSection(&m_section);
		return m_ThStatus == Thread::TH_SUSPEND;
	}
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	/** \fn  Task
	 *  \brief The task running the device
	 *  \note
	 *   detailed description,
	 *   detailed description.
	 *  \param[in]
	 *  \return 
	 */
	 void Task()
	{
		DeData tmpDeData;
		if (p_Device == 0) return;
		if (p_Device->m_DeStatus != Device::DE_INVALID) return;
		if (p_Device->Watch(tmpDeData))
		{
			EnterCriticalSection(&m_section);
			if (m_DeDataList.GetLength() > 0)
			{
				bool isNew = true;
				for (int i = m_DeDataList.GetLength() - 1; i >= 0; i--)
				{
					if (tmpDeData == *m_DeDataList.GetData(i))
					{
						isNew = false;
						break;
					}
				}
				if (isNew)
				{
					m_DeDataList.AddTail(tmpDeData);
				}

			}
			else
			{
				m_DeDataList.AddTail(tmpDeData);
			}
			LeaveCriticalSection(&m_section);
		}
	}
	///////////////////////////////////////////////////
};
///////////////////////////////////////////////////