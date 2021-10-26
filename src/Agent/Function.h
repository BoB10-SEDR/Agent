#pragma once
#include "stdafx.h"

namespace func
{
	void GetProcessList();
	void GetFileDescriptorList();
	void StartMonitoring(std::string data);
	void StopMonitoring(std::string data);
	void GetMonitoringInfo();
	void GetDeviceInfo();
	void GetModuleInfo();
	void ActivatePolicy(std::string data);
	void InactivatePolicy(std::string data);
	void ActivateCheck(std::string data);
	void GetCheckStatus();
}
