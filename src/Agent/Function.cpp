#include "Function.h"
#include "CMessage.h"
#include "CDevice.h"
#include "CPolicy.h"
#include "CMonitoring.h"

void func::GetProcessList()
{
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Get Process List"));
	
	ST_PROCESS_LIST processList;
	processList.processLists = MonitoringManager()->GetProcessLists();;
	std::tstring jsProcessList;
	core::WriteJsonToString(&processList, jsProcessList);
	MessageManager()->PushSendMessage(RESPONSE, PROCESS_LIST, jsProcessList);

	for (auto pInfo : processList.processLists) {
		GetFileDescriptorList(std::to_string(pInfo.pid));
		core::Log_Debug(TEXT("========================================"));
		core::Log_Debug(TEXT("Function.cpp - [pid] : %d"), pInfo.pid);
		core::Log_Debug(TEXT("Function.cpp - [ppid] : %d"), pInfo.ppid);
		core::Log_Debug(TEXT("Function.cpp - [name] : %s"), TEXT(pInfo.name.c_str()));
		core::Log_Debug(TEXT("Function.cpp - [state] : %s"), TEXT(pInfo.state.c_str()));
		core::Log_Debug(TEXT("Function.cpp - [cmdline] : %s"), TEXT(pInfo.cmdline.c_str()));
		core::Log_Debug(TEXT("Function.cpp - [sTime] : %s"), TEXT(pInfo.startTime.c_str()));
		core::Log_Debug(TEXT("========================================"));
	}
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Get Process List Complete"));
}

void func::GetFileDescriptorList(std::tstring pid)
{
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Get Process File Descriptor List"));
	
	ST_FD_LIST fdList;

	fdList.pid = strtol(pid.c_str(), NULL, 10);
	fdList.fdLists = MonitoringManager()->GetFdLists(pid);

	std::tstring jsFdList;
	core::WriteJsonToString(&fdList, jsFdList);
	MessageManager()->PushSendMessage(RESPONSE, FD_LIST, jsFdList);

	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Get Process File Descriptor List Complete"));
#ifdef DEBUG
	for (auto fInfo : fdList.fdLists) {
		core::Log_Debug(TEXT("========================================"));
		core::Log_Debug(TEXT("Function.cpp - [pid] : %d"), fInfo.pid);
		core::Log_Debug(TEXT("Function.cpp - [fdName] : %s"), TEXT(fInfo.fdName.c_str()));
		core::Log_Debug(TEXT("Function.cpp - [realPath] : %s"), TEXT(fInfo.realPath.c_str()));
		core::Log_Debug(TEXT("========================================"));
	}
#endif
}

void func::StartMonitoring(std::tstring data)
{
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Response Monitoring Target Added"));

	ST_MONITOR_LIST monitorList;
	core::ReadJsonFromString(&monitorList, data);

	for (auto target : monitorList.targetLists)
	{ 		
		core::Log_Debug(TEXT("Function.cpp - [%s] : %s"), TEXT("Monitoring Add Target"), TEXT(target.logPath.c_str()));
		int result = MonitoringManager()->AddMonitoringTarget(target);

		ST_MONITOR_RESULT monitorResult;

		monitorResult.processName = target.processName;
		monitorResult.logPath = target.logPath;
		monitorResult.result = result == 0 ? true : false;

		std::tstring jsMessage;
		core::WriteJsonToString(&monitorResult, jsMessage);

		MessageManager()->PushSendMessage(RESPONSE, MONITOR_RESULT, jsMessage);
	}

	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Response Monitoring Target Added Complete"));
}
void func::StopMonitoring(std::tstring data)
{
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Response Monitoring Target Remove"));

	ST_MONITOR_LIST monitorList;
	core::ReadJsonFromString(&monitorList, data);

	for (auto target : monitorList.targetLists)
	{
		core::Log_Debug(TEXT("Function.cpp - [%s] : %s"), TEXT("Monitoring Remove Target"), TEXT(target.logPath.c_str()));
		int result = MonitoringManager()->RemoveMonitoringTarget(target);

		ST_MONITOR_RESULT monitorResult;

		monitorResult.processName = target.processName;
		monitorResult.logPath = target.logPath;
		monitorResult.result = result == 0 ? false : true;

		std::tstring jsMessage;
		core::WriteJsonToString(&monitorResult, jsMessage);

		MessageManager()->PushSendMessage(RESPONSE, MONITOR_RESULT, jsMessage);
	}

	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Response Monitoring Target Remove Complete"));
}

void func::CollectMonitoringLog(std::tstring processName, std::tstring path, std::tstring data)
{
	core::Log_Info(TEXT("Function.cpp - [%s] : %s"), TEXT("Request Monitoring Info"), TEXT(path.c_str()));

	ST_MONITOR_INFO monitorInfo;

	monitorInfo.processName = processName;
	monitorInfo.logPath = path;
	monitorInfo.changeData = data;

	std::tstring jsMonitorInfo;
	core::WriteJsonToString(&monitorInfo, jsMonitorInfo);

	MessageManager()->PushSendMessage(RESPONSE, MONITOR_INFO, jsMonitorInfo);
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Request Monitoring Info Complete"));
}
void func::GetDeviceInfo()
{
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Response Device Info"));

	ST_DEVICE_INFO deviceInfo;

	CDevice* device = new CDevice();

	deviceInfo.name = device->getDeviceName();
	deviceInfo.modelNumber = device->getDeviceModelName();
	deviceInfo.serialNumber = device->getDeviceSerialNum();
	deviceInfo.ip = device->getDeviceIpAddr();
	deviceInfo.mac = device->getDeviceMacAddr();
	deviceInfo.architecture = device->getDeviceArchitecture();
	deviceInfo.os = device->getDeviceOS();

	free(device);
	std::tstring jsDeviceInfo;
	core::WriteJsonToString(&deviceInfo, jsDeviceInfo);
	
	MessageManager()->PushSendMessage(RESPONSE, DEVICE_INFO, jsDeviceInfo);
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Response Device Info Complete"));
}
void func::GetModuleInfo()
{
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Response Module Info"));
	ST_MODULE_INFO moduleInfo;

	CDevice* device = new CDevice();
	moduleInfo.deviceSerialNumber = device->getDeviceSerialNum();
	moduleInfo.deviceMac = device->getDeviceMacAddr();
	moduleInfo.name = "test";
	moduleInfo.modelNumber = "MD-1234";
	moduleInfo.serialNumber = "SN-1234-1234";
	moduleInfo.mac = "11:22:33:44:55:66";

	free(device);
	std::tstring jsModuleInfo;
	core::WriteJsonToString(&moduleInfo, jsModuleInfo);

	MessageManager()->PushSendMessage(RESPONSE, MODULE_INFO, jsModuleInfo);
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Response Module Info Complete"));
}
void func::ActivatePolicy(std::string data)
{
	LoggerManager()->Info("Response ActivatePolicy");
	ST_POLICY_INFO* policyServerInfo = new ST_POLICY_INFO();
	core::ReadJsonFromString(policyServerInfo, data);

	//�å�� �ִ��� ����
	bool result = PolicyManager()->isExist(policyServerInfo);

	//�å�� ����� �ٿ�ε�
	if (!result)
		PolicyManager()->download(policyServerInfo);

	//�å ����
	PolicyManager()->active(policyServerInfo);

	//���� ��� 
	result = PolicyManager()->SuccessPolicy(policyServerInfo);

	ST_POLICY_RESULT* policyResult = new ST_POLICY_RESULT();

	if (result) // �� ���� ���������
	{
		LoggerManager()->Info("SuccessPolicy True");

		policyResult->idx = policyServerInfo->idx;
		policyResult->result = true;
		policyResult->time = "2021-10-26";
	}
	else //���и� �����
	{
		LoggerManager()->Info("SuccessPolicy Fail");
		policyResult->idx = policyServerInfo->idx;
		policyResult->result = false;
		policyResult->time = "2021-10-26";
	}
	//�å ���� ��� ���
	std::tstring jsPolicyResult;
	core::WriteJsonToString(policyResult, jsPolicyResult);
	LoggerManager()->Info("Send Message");

	MessageManager()->PushSendMessage(RESPONSE, POLICY_STATE, jsPolicyResult);
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Response Policy Activate Complete"));
}

void func::InactivatePolicy(std::string data)
{
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Response Policy InActivate"));
	ST_POLICY_RESULT policyResult;
	ST_POLICY_INFO policyInfo;

	core::ReadJsonFromString(&policyInfo, data);

	policyResult.idx = policyInfo.idx;
	policyResult.result = false;
	policyResult.time = "2021-10-26";

	std::tstring jsPolicyResult;
	core::WriteJsonToString(&policyResult, jsPolicyResult);

	MessageManager()->PushSendMessage(RESPONSE, POLICY_STATE, jsPolicyResult);
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Response Policy InActivate Complete"));
}
void func::ActivateCheck(std::string data)
{
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Request Policy Check"));
	ST_CHECK_INFO checkInfo;

	core::ReadJsonFromString(&checkInfo, data);

	ST_MESSAGE message;

	message.opcode = CHECK_ACTIVATE;
	message.status = true;
	message.data = data;

	std::tstring jsMessage;
	core::WriteJsonToString(&message, jsMessage);

	MessageManager()->PushSendMessage(RESPONSE, MESSAGE, jsMessage);
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Request Policy Check Complete"));
}
void func::GetCheckStatus()
{
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Request Checklist Status"));

	ST_CHECK_RESULT checkResult;

	checkResult.logID = 1;
	checkResult.result = false;
	checkResult.step = 1;
	checkResult.time = "2021-12-12";

	std::tstring jsCheckResult;
	core::WriteJsonToString(&checkResult, jsCheckResult);

	MessageManager()->PushSendMessage(RESPONSE, CHECK_STATE, jsCheckResult);
	core::Log_Info(TEXT("Function.cpp - [%s]"), TEXT("Request Checklist Status Complete"));

}
