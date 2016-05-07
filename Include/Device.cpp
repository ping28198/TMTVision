#include "Device.h"
#include "json.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

DeSetting::DeSetting(const MEGASTR setting)
{		
	try
	{
		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(setting, root, false))
		{
			throw DE_ERR;
		}
		strcpy_s(DeviceName, sizeof(DeviceName), root["DeviceName"].asString().data());
		strcpy_s(DevicePath, sizeof(DevicePath), root["DevicePath"].asString().data());
		ServerDataSize = root["ServerDataSize"].asInt();
		ServerTimes = root["ServerTimes"].asInt64();
		ServerWaitTime = root["ServerWaitTime"].asInt64();
		ServerIncludeTaskTime = root["ServerIncludeTaskTime"].asBool();
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

DeSetting & DeSetting::operator=(const MEGASTR setting)
{
	// TODO: 在此处插入 return 语句
	try
	{
		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(setting, root, false))
		{
			throw DE_ERR;
		}
		strcpy_s(DeviceName, sizeof(DeviceName), root["DeviceName"].asString().data());
		strcpy_s(DevicePath, sizeof(DevicePath), root["DevicePath"].asString().data());
		ServerDataSize = root["ServerDataSize"].asInt();
		ServerTimes = root["ServerTimes"].asInt64();
		ServerWaitTime = root["ServerWaitTime"].asInt64();
		ServerIncludeTaskTime = root["ServerIncludeTaskTime"].asBool();
		return *this;
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

void DeSetting::CopyTo(MEGASTR setting)
{
	try
	{
		setting[0] = 0;
		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(setting, root, false))
		{
			throw DE_ERR;
		}
		strcpy_s(DeviceName, sizeof(DeviceName), root["DeviceName"].asString().data());
		strcpy_s(DevicePath, sizeof(DevicePath), root["DevicePath"].asString().data());
		ServerDataSize = root["ServerDataSize"].asInt();
		ServerTimes = root["ServerTimes"].asInt64();
		ServerWaitTime = root["ServerWaitTime"].asInt64();
		ServerIncludeTaskTime = root["ServerIncludeTaskTime"].asBool();
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

void DeSetting::CopyFrom(const MEGASTR setting)
{
	try
	{
		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(setting, root, false))
		{
			throw DE_ERR;
		}
		strcpy_s(DeviceName, sizeof(DeviceName), root["DeviceName"].asString().data());
		strcpy_s(DevicePath, sizeof(DevicePath), root["DevicePath"].asString().data());
		ServerDataSize = root["ServerDataSize"].asInt();
		ServerTimes = root["ServerTimes"].asInt64();
		ServerWaitTime = root["ServerWaitTime"].asInt64();
		ServerIncludeTaskTime = root["ServerIncludeTaskTime"].asBool();
	}
	catch (DWORD& e)
	{
	}
	catch (exception& e)
	{
	}
}

Device::Device()
{
	m_hDevice = INVALID_HANDLE_VALUE;
	m_DeStatus = DE_INVALID;
}
