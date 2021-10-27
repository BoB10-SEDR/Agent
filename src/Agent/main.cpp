#include "stdafx.h"
#include "CDevice.h"
#include "CPolicy.h"
#include "CMessage.h"
#include "CMonitoring.h"

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
	LoggerManager()->Info("Start Agent Program!");
	try
	{
		std::future<void> a = std::async(std::launch::async, &CMessage::Init, MessageManager());
		std::future<void> b = std::async(std::launch::async, &CMonitoring::StartMonitoring, MonitoringManager());
		std::future<void> c = std::async(std::launch::async, &AddLogPath);
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