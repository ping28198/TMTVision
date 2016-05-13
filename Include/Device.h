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
 *      Device       Initial(&_)─＞│ Device │─＞ Sample() return
 *                                  └────┘              │
 *                                       │                   ∨
 *                                       ∨              ┌────┐
 *      DeviceServer        AttachDevice(*_)             │DeData* │
 *                                                       └────┘
 *                                                            │
 *                                                            ∨
 *                          Task(){ deque<DeData*>::push_back(__) }
 *
 *     //CamSetting:public DeSetting/n 
 *     //CamData:public DeData/n
 *     //Camera:public Device/n
 *     CamSetting camSetting(jsonstring);/n
 *     Camera camera;/n
 *     camera.Initial((DeSetting*)&camSetting);/n
 *     DeviceServer deviceServer;/n
 *     deviceServer.AttachDevice((Device*)&camera);/n
 *     //1.Active frame in thread/n
 *     deviceServer.StartServer();/n
 *     ...
 *     deviceServer.StopServer();/n
 *     //2.Or call back frame through Task() function./n
 *     ...
 *     deviceServer.DetachDevice();/n
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
#include <deque>
#include "Err_seq.h"

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
/** \struct SettingSet
 *  \brief 
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/05/10 12:38
 */
#ifndef SETTINGSET
#define SETTINGSET
template <typename T>
struct SettingSet
{ 
	enum {SETTINGSETSIZE = 32///< Setting Data max size
	};
	T SettingData[SETTINGSETSIZE];///< Setting data
	int SettingNum = 0; ///< Valid setting data number
}; 
#endif
///////////////////////////////////////////////////

///////////////////////////////////////////////////
/** \class DeSetting
 *  \brief Base struct to store setting para.
 *  \note
 *   Decode para from joson string\n
 *   Read and store by method in derived class.\n
 *  \author Leon Contact: towanglei@163.com
 */
#ifndef TMTV_MEGASTRLEN
#define TMTV_MEGASTRLEN	    4096//字符串最大长度
typedef char	MEGASTR[TMTV_MEGASTRLEN];
#endif
#ifndef DESETTING
#define DESETTING
class DeSetting
{
public:
	SHORTSTR deviceName = "";///< Device name
	PATHSTR devicePath = ""; ///< Device path
	NetIP deviceMAC = "";///< MAC address of Device
	NetIP deviceIP = "";///< IP address of host,not support multi-net
	int serverDataSize = 8; ///< Data list size
	long serverTimes = -1; ///< Thread run times
	long serverFrameTime = 0; ///< Thread wait time
	//bool ServerIncludeTaskTime = true; ///< If task time included in wait times

	/// Default constructor get PC name and physical net
	DeSetting();
	/** \fn  ~DeSetting
	*  \brief virtual destruct function, avoid delete wrong object
	*/
	virtual ~DeSetting();

	/// Deep copy constructor
	DeSetting(const DeSetting& setting);
	/// Deep copy constructor
	DeSetting(const MEGASTR& setting);
	/// Deep copy operator=
	DeSetting& operator=(const DeSetting& setting);
	/// Deep copy operator=
	virtual DeSetting& operator=(const MEGASTR& setting);
	/// Deep copy to para
	void CopyTo(DeSetting& setting);
	/// Deep copy to para
	virtual void CopyTo(MEGASTR& setting);
	/// Deep copy from para
	void CopyFrom(const DeSetting& setting);
	/// Deep copy from para
	virtual void CopyFrom(const MEGASTR& setting);
	/// Clear para
	virtual void Clear();
	/// Compare operator==
	bool operator== (const DeSetting& deSetting)
	{
		if (deviceMAC[0]==0 || deSetting.deviceMAC[0]==0)
		{
			return false;
		}
		return strcmp(deviceMAC, deSetting.deviceMAC);
	};

	/** \fn  GetAvailable
	 *  \brief Get available device list,just get host setting in base class of DeSetting
	 *  \param[in] NetIP hostIP = "" IP of the machine will be checked 
	 *  \param[out] DeSetting* deSettings, int deNums
	 *  \return static MEGASTR as json format
	 */
	static int GetAvailable(SettingSet<DeSetting>& deSettings);
};
#endif
///////////////////////////////////////////////////

//////////////////////////////////////////////////
/** \class DeData
*  \brief Base class to store template IO data
*  \note
*   Inlined methods for better performents;\n
*   Implemente copy constructor and = operator;\n
*   Recode create time in, use GetLife() to avoid dead data;\n
*   Use dataTimeStr in compare of operator ==, to avoid duplicate data.\n
*  \author Leon Contact: towanglei@163.com
*  \version 1.0
*  \date 2016/05/02 11:38
*/
class DeData
{
public:
	//T data;///< Data body, if overwrite in derived struct
	SYSTEMTIME createTime;
	SYSTEMTIME updateTime;
	TINYSTR dataTimeStr = "2000-01-01 00:00:00:00";///< Data time.
	
	/// Default constructor
	DeData();
	/** \fn  ~DeData
	*  \brief virtual destruct function, avoid delete wrong object
	*/
	virtual ~DeData();

	/// Deep copy constructor
	DeData(const DeData& dataIn);
	/// Deep copy operator=
	DeData& operator=(const DeData& dataIn);
	/// Deep copy to para
	virtual void CopyTo(DeData& dataIn);
	/// Deep copy from para
	virtual void CopyFrom(const DeData& dataIn);

	/** \fn  operator==
	*  \brief Compare operator==, to avoid duplicate data, must override
	*  \param[in] DeData& dataIn = (DeData&)dataObject
	*  \return bool
	*/
	virtual bool operator== (const DeData& dataIn);

	/// Get life of the object, to avoid dead data
	double GetLife();
};
///////////////////////////////////////////////////

///////////////////////////////////////////////////
/** \class Device 
 *  \brief Base class of all devices.
*  \note
*   Inlined methods for better performents;\n
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/04/29 10:34
 */
class Device
{
public:
    /// Handle of device
	HANDLE m_hDevice;
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
	/// Status of device
	DESTATUS m_DeStatus;
	/// Setting of device
	DeSetting* p_DeSetting = 0;

	/// Default constructor
	Device();
	/** \fn  ~Device
	*  \brief virtual destruct function, avoid delete wrong object
	*/
	virtual ~Device();
	/** \fn  Initial
	 *  \brief Initial device
	 *  \param[in] DeSetting* pSetting = new DeSetting(MEGASTR& setting)
	 *  \return bool
	 */
	virtual bool Initial(DeSetting* pSetting); 
	/** \fn  Unitial
	 *  \brief Unitial device
	 */
	virtual void Unitial();
	/** \fn  Set
	 *  \brief Set device para without stop
	 *  \param[in] DeSetting* pSetting = new DeSetting(MEGASTR& setting)
	 *  \return bool
	 */
	virtual bool Set(DeSetting* pSetting);
	/** \fn  ReSet
	 *  \brief ReSet device, stop and restart
	 *  \param[in] DeSetting* pSetting = new DeSetting(MEGASTR& setting)
	 *  \return bool
	 */
	virtual bool ReSet(DeSetting* pSetting);

	/** \fn  Sample
	*  \brief Sample device, overwrite this function in delivered class
	*  \return Self newed DeData* = new DeData() or 0 if failed.
	*/
	virtual DeData* Sample();
};
///////////////////////////////////////////////////

///////////////////////////////////////////////////
/** \class DeviceServer : public Thread, include Device
 *  \brief Base class of all devices server.
*  \note
*   Inlined methods for better performents;\n
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/04/29 23:09
 */
class DeviceServer : public Thread
{
public:
	Device* p_Device; ///< Device object pointer
	std::deque <DeData*> m_DataList; ///< Data list deque push front pop back, iterate front to back
	int m_DataListMaxNum; ///< Data list max number in deque

	/// Actual frame time(ms), decided by driver or server, -1 means unused
	double GetFrameTime()
	{
		return MAX((double)m_waitTime, m_taskTime);
	}
	/// Actual sample time(ms), decided by driver or server, -1 means unused
	double GetSampleTime()
	{
		return m_taskTime;
	}

	/// Set expected frame time(ms)
	void SetFrameTime(long frameTime)
	{
		m_waitTime = frameTime;
	}
	/// Set expected sample time(ms)
	virtual void SetSampleTime(long sampleTime) {}

public:
	/// Default constructor
	DeviceServer(HANDLE  hParent = 0);
	/// Default destructor
	~DeviceServer();

	/** \fn  AttachDevice
	 *  \brief Add device to server
	 *  \param[in] Device* pDevice = (Device*)&deviceObj;
	 *  \return bool
	 */
	virtual bool AttachDevice(Device* pDevice);
	/** \fn  DetachDevice
	 *  \brief Delete device from server, 
	 *         but device object is not delete in function
	 *  \return void
	 */
	virtual void DetachDevice();
	/** \fn  StartServer
	 *  \brief Start server to run the added device
	 *  \return bool
	 */
	virtual bool StartServer();
	/** \fn  StopServer
	*  \brief Stop server without delete anything
	*  \return bool
	*/
	virtual bool StopServer();
	/** \fn  Set
	*  \brief Set device in running server
	*  \param[in] DeSetting* pSetting = (DeSetting*)&setting;
	*  \return bool
	*/
	virtual bool Set(DeSetting* pSetting);
	/** \fn  ReSet
	*  \brief Stop and reset device in server,need StartServer()
	*  \param[in] DeSetting* pSetting = (DeSetting*)&setting;
	*  \return bool
	*/
	virtual bool ReSet(DeSetting* pSetting);
	/** \fn  Task
	 *  \brief The task running the device
	 *  \return 
	 */
	void Task();
};
///////////////////////////////////////////////////