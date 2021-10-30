#include "stdafx.h"
#include "CDevice.h"
#include "CPolicy.h"
#include "CMessage.h"
#include "CMonitoring.h"
#include "Function.h"

#define BUFFER_SIZE 1024

std::string SendToTerminal(const char* ShellCommand)
{
	std::ostringstream	ShellCommandTemp;
	FILE* SendShellCommand = popen(ShellCommand, "r");

	while (!feof(SendShellCommand) && !ferror(SendShellCommand))
	{
		char ShellCommandBuf[128];
		int ShellBytesRead = fread(ShellCommandBuf, 1, 128, SendShellCommand);
		ShellCommandTemp.write(ShellCommandBuf, ShellBytesRead);
	}

	std::string RecievedData = ShellCommandTemp.str();
	pclose(SendShellCommand);


	std::string::size_type pos = 0;
	std::string::size_type offset = 0;
	const std::string SpacePattern = "\n";
	const std::string Replace = "";

	while ((pos = RecievedData.find(SpacePattern, offset)) != std::string::npos)
	{
		RecievedData.replace(RecievedData.begin() + pos, RecievedData.begin() + pos + SpacePattern.size(), Replace);
		offset = pos + Replace.size();
	}

	return RecievedData;
}
void AddLogPath()
{
	while (true)
	{
		sleep(0);
		char agentInfo[BUFFER_SIZE];
		int select;
		printf("Path : ");
		fgets(agentInfo, BUFFER_SIZE, stdin);
		*(agentInfo + (strlen(agentInfo) - 1)) = 0;

		printf("Opcode : ");
		scanf("%d", &select);
		printf("opcode : %d\n", select);

		while (getchar() != '\n');

		if (select == 1)
		{
			std::cout << MonitoringManager()->AddMonitoringTarget(agentInfo) << std::endl;
		}
		else
		{
			std::cout << MonitoringManager()->RemoveMonitoringTarget(agentInfo) << std::endl;
		}
	}
}

int main(int argc, char* argv[])
{
	//func::GetProcessList();
	//func::GetFileDescriptorList("27856");

	LoggerManager()->Info("Start Agent Program!");
	try
	{
		std::future<void> a = std::async(std::launch::async, &CMessage::Init, MessageManager());
		std::future<void> b = std::async(std::launch::async, &CMonitoring::StartMonitoring, MonitoringManager());
	}
	catch (std::exception& e)
	{
		LoggerManager()->Error(e.what());
	}
	LoggerManager()->Info("Terminate Agent Program!");

// 	CDevice dev = CDevice();
// 	dev.DeviceInit();
// 	dev.DeviceInfoPrint();

// 	CPolicy pol = CPolicy();
// 	pol.getPolicyInfo();
// 	pol.PolicyInfoPrint();
	return 0;
}
//struct Test : public core::IFormatterObject {
//	std::vector <int*> numberList;
//	int* num;
//
//    Test(void)
//	{
//		num = new int();
//	}
//    Test(std::vector <int*> _numberList, int *_num)
//        : numberList(_numberList), num(_num)
//    {}
//
//    void OnSync(core::IFormatter& formatter)
//    {
//		formatter
//			+ core::sPair(TEXT("NumberList"), numberList)
//			+ core::sPair(TEXT("Num"), *num);
//            ;
//    }
//};
//
//int main() {
//	//core::InitLog();
//	core::Log_Info(("123"));
//
//	Test* t = new Test();
//	std::vector <int*> numberList;
//
//	for (int i = 0; i < 10; i++) {
//		numberList.push_back(new int(i));
//	}
//
//	for (auto i : numberList) {
//		std::cout << *i << std::endl;
//	}
//
//	t->numberList = numberList;
//	t->num = new int(10);
//	std::tstring testString;
//	core::WriteJsonToString(t, testString);
//
//	std::cout << testString << std::endl;
//
//	Test* t_new = new Test();
//	core::ReadJsonFromString(t_new, testString);
//	//for (auto i : *t_new->numberList) {
//	//	std::cout << i << std::endl;
//	//}
//	std::cout << *t_new->num << std::endl;
//
//	for (auto i : t_new->numberList) {
//		std::cout << *i << std::endl;
//	}
//}