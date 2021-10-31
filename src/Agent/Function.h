#pragma once
#include "stdafx.h"

namespace func
{
	void GetProcessList();
	void GetFileDescriptorList(std::tstring pid);
	void StartMonitoring(std::tstring data);
	void StopMonitoring(std::tstring data);
	void CollectMonitoringLog(std::tstring processName, std::tstring path, std::tstring data);
	void GetDeviceInfo();
	void GetModuleInfo();
	void ActivatePolicy(std::string data);
	void InactivatePolicy(std::string data);
	void ActivateCheck(std::string data);
	void GetCheckStatus();
}
