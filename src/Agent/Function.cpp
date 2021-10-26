#include "Function.h"
#include "CMessage.h"
#include "CDevice.h"

void func::GetProcessList()
{
	LoggerManager()->Info("Response GetProcessList");
	ST_PROCESS_LIST* processList = new ST_PROCESS_LIST();

	processList->processLists.push_back(ST_PROCESS_INFO(1, "/test1"));
	processList->processLists.push_back(ST_PROCESS_INFO(2, "/test2"));
	processList->processLists.push_back(ST_PROCESS_INFO(3, "/test3"));

	std::tstring jsProcessList;
	core::WriteJsonToString(processList, jsProcessList);

	MessageManager()->PushSendMessage(RESPONSE, PROCESS_LIST, jsProcessList);
}
void func::GetFileDescriptorList()
{
	LoggerManager()->Info("Response GetFileDescriptorList");
	ST_FD_LIST* fdList = new ST_FD_LIST();

	fdList->fdLists.push_back(ST_FD_INFO(1, "/test1", "/path/test1"));
	fdList->fdLists.push_back(ST_FD_INFO(2, "/test2", "/path/test2"));
	fdList->fdLists.push_back(ST_FD_INFO(3, "/test3", "/path/test3"));

	std::tstring jsFdList;
	core::WriteJsonToString(fdList, jsFdList);

	MessageManager()->PushSendMessage(RESPONSE, FD_LIST, jsFdList);
}
void func::StartMonitoring(std::string data)
{
	LoggerManager()->Info("Response StartMonitoring");
	ST_MESSAGE* message = new ST_MESSAGE();

	message->opcode = MONITOR_ACTIVATE;
	message->status = true;
	message->data = data;

	std::tstring jsMessage;
	core::WriteJsonToString(message, jsMessage);

	MessageManager()->PushSendMessage(RESPONSE, MESSAGE, jsMessage);
}
void func::StopMonitoring(std::string data)
{
	LoggerManager()->Info("Response StopMonitoring");
	ST_MESSAGE* message = new ST_MESSAGE();

	message->opcode = MONITOR_ACTIVATE;
	message->status = true;
	message->data = data;

	std::tstring jsMessage;
	core::WriteJsonToString(message, jsMessage);

	MessageManager()->PushSendMessage(RESPONSE, MESSAGE, jsMessage);
}
void func::GetMonitoringInfo()
{
	LoggerManager()->Info("Response GetMonitoringInfo");
	ST_MONITOR_INFO* monitorInfo = new ST_MONITOR_INFO();

	monitorInfo->path = "monitoring log process";
	monitorInfo->data = "monitoring log data";

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
