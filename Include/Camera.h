///////////////////////////////////////////////////
/** \file Camera.h
 *  \brief  Base class of all Cameras
 *  \author Leon Contact: towanglei@163.com
 *  \copyright TMTeam
 *  \version 1.0
 *  \History:
 *     Leon 2016/05/09 0:10 build\n
 */
///////////////////////////////////////////////////

#pragma once
#include "Device.h"

//////////////////////////////////////////////////
/** \class CamSetting
*  \brief Struct to store Camera setting para.
*  \note
*   Decode para from joson string\n
*   Read and store by method in derived class.
*  \author Leon Contact: towanglei@163.com
*/
#ifndef TMTV_MEGASTRLEN
#define TMTV_MEGASTRLEN	    4096//字符串最大长度
typedef char	MEGASTR[TMTV_MEGASTRLEN];
#endif
#ifndef CAMSETTING
#define CAMSETTING
class CamSetting: public DeSetting
{
public:
	// Rename variables' defines to compatible with old version
	SHORTSTR& cameraName = deviceName;///< Device name
	PATHSTR& cameraPath = devicePath; ///< Default path to save image/video
	NetIP& cameraHostMAC = deviceMAC;///< MAC address of Camera host
	NetIP& cameraHostIP = deviceIP;///< IP address of Camera Host

	//NetIP CameraMAC;///< MAC address of Camera
	double cameraPos[8];///< Location of Camera
	int cameraMaxWidth = 1920;///< Max width of Camera image
	int cameraMaxHeight = 1080;///< Max height  of Camera image
	int cameraTop = 0;///< Top of Camera image
	int cameraLeft = 0;///< Left of Camera image
	int cameraWidth = 1920;///< Width of Camera image
	int cameraHeight = 1080;///< Height of Camera image
	long cameraExposure = -1;///< Exposure of Camera -1 means auto

	TINYSTR imageType = "Gray";///< Tpye of Image such as RGB Gray
	TINYSTR imageFormat = "jpg";///< Format of Image such as jpg

	/// Default constructor
	CamSetting();
	/** \fn  ~CamSetting
	*  \brief virtual destruct function, avoid delete wrong object
	*/
	virtual ~CamSetting();

	/// Deep copy constructor
	CamSetting(const CamSetting& setting);
	/// Deep copy constructor
	CamSetting(const MEGASTR& setting);
	/// Deep copy operator=
	CamSetting& operator=(const CamSetting& setting);
	/// Deep copy operator=
	virtual CamSetting& operator=(const MEGASTR& setting);
	/// Deep copy to para
	void CopyTo(CamSetting& setting);
	/// Deep copy to para
	virtual void CopyTo(MEGASTR& setting);
	/// Deep copy from para
	void CopyFrom(const CamSetting& setting);
	/// Deep copy from para
	virtual void CopyFrom(const MEGASTR& setting);
};
#endif
///////////////////////////////////////////////////

//////////////////////////////////////////////////
/** \class CamData
*  \brief Base class to store template camera IO data
*  \note
*   Inlined methods for better performents;\n
*   Implemente copy constructor and = operator;\n
*   Recode create time in, use GetLife() to avoid dead data;\n
*   Use dataTimeStr in compare of operator ==, to avoid duplicate data.\n
*  \author Leon Contact: towanglei@163.com
*  \version 1.0
*  \date 2016/05/02 11:38
*/
class CamData :public DeData
{
public:
	//int* data = 0;///< Data body, if overwrite in derived struct

	/// Default constructor
	CamData();
	/** \fn  ~Device
	*  \brief virtual destruct function, avoid delete wrong object
	*/
	virtual ~CamData();

	/// Deep copy constructor
	CamData(const CamData& dataIn);
	/// Deep copy operator=
	CamData& operator=(const CamData& dataIn);
	/// Deep copy to para
	virtual void CopyTo(CamData& dataIn);
	/// Deep copy from para
	virtual void CopyFrom(const CamData& dataIn);

	/** \fn  operator==
	*  \brief Compare operator==, to avoid duplicate data, must override
	*  \param[in] DeData& dataIn = (DeData&)CamDataObject
	*  \return bool
	*/
	virtual bool operator== (const DeData& dataIn) override;
};
///////////////////////////////////////////////////

///////////////////////////////////////////////////
/** \class Camera : 
 *  \brief Base class of all Camera.
 *  \author Leon Contact: towanglei@163.com
 *  \version 1.0
 *  \date 2016/05/11 5:43
 */
class Camera : public Device
{
public:
	/// Default constructor
	Camera();
	/** \fn  ~Camera
	*  \brief virtual destruct function, avoid delete wrong object
	*/
	virtual ~Camera();

	/** \fn  Initial
	*  \brief Initial camera
	*  \param[in] DeSetting* pSetting = new CamSetting(MEGASTR& setting)
	*  \return bool
	*/
	virtual bool Initial(DeSetting* pSetting) override;
	/** \fn  Unitial
	*  \brief Unitial camera
	*/
	virtual void Unitial() override;
	/** \fn  Set
	*  \brief Set camera para without stop
	*  \param[in] DeSetting* pSetting = new CamSetting(MEGASTR& setting)
	*  \return bool
	*/
	virtual bool Set(DeSetting* pSetting) override;
	/** \fn  ReSet
	*  \brief ReSet camera, stop and restart
	*  \param[in] DeSetting* pSetting = new CamSetting(MEGASTR& setting)
	*  \return bool
	*/
	virtual bool ReSet(DeSetting* pSetting) override;

	/** \fn  Sample
	*  \brief Sample device, overwrite this function in delivered class
	*  \return Self newed DeData* = new CamData() or 0 if failed.
	*/
	virtual DeData* Sample() override;
};
///////////////////////////////////////////////////