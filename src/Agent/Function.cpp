#include "Function.h"
#include "CMessage.h"
#include "CDevice.h"
#include "CMonitoring.h"

void func::GetProcessList()
{
	sleep(0);
	LoggerManager()->Info("Response GetProcessList");
	ST_PROCESS_LIST* processList = new ST_PROCESS_LIST();

	processList->processLists = MonitoringManager()->GetProcessLists();

	for (auto pInfo : processList->processLists) {
		LoggerManager()->Info("========================================");
		LoggerManager()->Info(StringFormatter("pid : %d", pInfo.pid));
		LoggerManager()->Info(StringFormatter("ppid : %d", pInfo.ppid));
		LoggerManager()->Info(StringFormatter("name : %s", pInfo.name.c_str()));
		LoggerManager()->Info(StringFormatter("state : %s", pInfo.state.c_str()));
		LoggerManager()->Info(StringFormatter("cmdline : %s", pInfo.cmdline.c_str()));
		LoggerManager()->Info(StringFormatter("sTime : %s", pInfo.startTime.c_str()));
	}

	std::string jsProcessList;
	core::WriteJsonToString(processList, jsProcessList);

	MessageManager()->PushSendMessage(RESPONSE, PROCESS_LIST, jsProcessList);
	MessageManager()->PushSendMessage(RESPONSE, PROCESS_LIST, "{\r\n\t\"ProcessLists\":[\r\n\t\t{\r\n\t\t\t\"Pid\":1,\r\n\t\t\t\"PPid\":0,\r\n\t\t\t\"Name\":\"init\",\r\n\t\t\t\"State\":\"S (sleeping)\",\r\n\t\t\t\"Cmdline\":\"\\\/init\",\r\n\t\t\t\"StartTime\":\"Fri Mar 29 15:44:35 1974\\n\"\r\n\t\t}\r\n\t]\r\n}");
}

void func::GetFileDescriptorList(std::string pid)
{
	sleep(0);
	LoggerManager()->Info("Response GetFileDescriptorList");
	
	ST_FD_LIST* fdList = new ST_FD_LIST();

	fdList->fdLists = MonitoringManager()->GetFdLists(pid);

	for (auto fInfo : fdList->fdLists) {
		LoggerManager()->Info("========================================");
		LoggerManager()->Info(StringFormatter("pid : %d", fInfo.pid));
		LoggerManager()->Info(StringFormatter("name : %s", fInfo.name.c_str()));
		LoggerManager()->Info(StringFormatter("path : %s", fInfo.path.c_str()));
	}

	std::tstring jsFdList;
	core::WriteJsonToString(fdList, jsFdList);

	MessageManager()->PushSendMessage(RESPONSE, FD_LIST, jsFdList);
}

void func::StartMonitoring(std::string data)
{
	LoggerManager()->Info("Response StartMonitoring");
	//메시지를 수신
	ST_MONITOR_LIST* monitorList = new ST_MONITOR_LIST();
	core::ReadJsonFromString(monitorList, data);

	for (std::string path : monitorList->pathLists) 
	{ 
		int result = MonitoringManager()->AddMonitoringTarget(path);
		LoggerManager()->Info(StringFormatter("[%s] : %d", path.c_str(), result));
	}

	//ST_MESSAGE* message = new ST_MESSAGE();

	//message->opcode = MONITOR_ACTIVATE;
	//message->status = true;
	//message->data = data;

	//std::tstring jsMessage;
	//core::WriteJsonToString(message, jsMessage);

	//MessageManager()->PushSendMessage(RESPONSE, MESSAGE, jsMessage);
}
void func::StopMonitoring(std::string data)
{
	LoggerManager()->Info("Response StopMonitoring");

	ST_MONITOR_LIST* monitorList = new ST_MONITOR_LIST();
	core::ReadJsonFromString(monitorList, data);

	for (std::string path : monitorList->pathLists)
	{
		int result = MonitoringManager()->RemoveMonitoringTarget(path);
		LoggerManager()->Info(StringFormatter("[%s] : %d", path.c_str(), result));
	}
	//ST_MESSAGE* message = new ST_MESSAGE();

	//message->opcode = MONITOR_ACTIVATE;
	//message->status = true;
	//message->data = data;

	//std::tstring jsMessage;
	//core::WriteJsonToString(message, jsMessage);

	//MessageManager()->PushSendMessage(RESPONSE, MESSAGE, jsMessage);
}
void func::SendMonitoringInfo(std::string path, std::string data)
{
	LoggerManager()->Info("Response GetMonitoringInfo");
	ST_MONITOR_INFO* monitorInfo = new ST_MONITOR_INFO();

	monitorInfo->path = path;
	monitorInfo->data = data;

	std::tstring jsMonitorInfo;
	core::WriteJsonToString(monitorInfo, jsMonitorInfo);

	MessageManager()->PushSendMessage(RESPONSE, MONITOR_INFO, jsMonitorInfo);
}
void func::GetDeviceInfo()
{
	LoggerManager()->Info("Response GetDeviceInfo");
	ST_DEVICE_INFO* deviceInfo = new ST_DEVICE_INFO();

	CDevice* device = new CDevice();

	deviceInfo->name = device->getDeviceName();
	deviceInfo->modelNumber = device->getDeviceModelName();
	deviceInfo->serialNumber = device->getDeviceSerialNum();
	deviceInfo->ip = device->getDeviceIpAddr();
	deviceInfo->mac = device->getDeviceMacAddr();
	deviceInfo->architecture = device->getDeviceArchitecture();
	deviceInfo->os = device->getDeviceOS();

	std::tstring jsDeviceInfo;
	core::WriteJsonToString(deviceInfo, jsDeviceInfo);

	MessageManager()->PushSendMessage(RESPONSE, DEVICE_INFO, jsDeviceInfo);
}
void func::GetModuleInfo()
{
	LoggerManager()->Info("Response GetModuleInfo");
	ST_MODULE_INFO* moduleInfo = new ST_MODULE_INFO();

	CDevice* device = new CDevice();

	moduleInfo->deviceSerialNumber = device->getDeviceSerialNum();
	moduleInfo->deviceMac = device->getDeviceMacAddr();
	moduleInfo->name = "test";
	moduleInfo->modelNumber = "MD-1234";
	moduleInfo->serialNumber = "SN-1234-1234";
	moduleInfo->mac = "11:22:33:44:55:66";

	std::tstring jsModuleInfo;
	core::WriteJsonToString(moduleInfo, jsModuleInfo);

	MessageManager()->PushSendMessage(RESPONSE, MODULE_INFO, jsModuleInfo);
}
void func::ActivatePolicy(std::string data)
{
	LoggerManager()->Info("Response ActivatePolicy");
	ST_POLICY_RESULT* policyResult = new ST_POLICY_RESULT();
	ST_POLICY_INFO* policyInfo = new ST_POLICY_INFO();

	core::ReadJsonFromString(policyInfo, data);

	policyResult->idx = policyInfo->idx;
	policyResult->result = true;
	policyResult->time = "2021-10-26";

	std::tstring jsPolicyResult;
	core::WriteJsonToString(policyResult, jsPolicyResult);

	MessageManager()->PushSendMessage(RESPONSE, POLICY_STATE, jsPolicyResult);
}
void func::InactivatePolicy(std::string data)
{
	LoggerManager()->Info("Response InactivatePolicy");
	ST_POLICY_RESULT* policyResult = new ST_POLICY_RESULT();
	ST_POLICY_INFO* policyInfo = new ST_POLICY_INFO();

	core::ReadJsonFromString(policyInfo, data);

	policyResult->idx = policyInfo->idx;
	policyResult->result = false;
	policyResult->time = "2021-10-26";

	std::tstring jsPolicyResult;
	core::WriteJsonToString(policyResult, jsPolicyResult);

	MessageManager()->PushSendMessage(RESPONSE, POLICY_STATE, jsPolicyResult);
}
void func::ActivateCheck(std::string data)
{
	LoggerManager()->Info("Response ActivateCheck");
	ST_CHECK_INFO* checkInfo = new ST_CHECK_INFO();

	core::ReadJsonFromString(checkInfo, data);

	ST_MESSAGE* message = new ST_MESSAGE();

	message->opcode = CHECK_ACTIVATE;
	message->status = true;
	message->data = data;

	std::tstring jsMessage;
	core::WriteJsonToString(message, jsMessage);

	MessageManager()->PushSendMessage(RESPONSE, MESSAGE, jsMessage);
}
void func::GetCheckStatus()
{
	LoggerManager()->Info("Response InactivatePolicy");
	ST_CHECK_RESULT* checkResult = new ST_CHECK_RESULT();

	checkResult->logID = 1;
	checkResult->result = false;
	checkResult->step = 1;
	checkResult->time = "2021-12-12";

	std::tstring jsCheckResult;
	core::WriteJsonToString(checkResult, jsCheckResult);

	MessageManager()->PushSendMessage(RESPONSE, CHECK_STATE, jsCheckResult);
}
