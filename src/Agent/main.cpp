#include "stdafx.h"
#include "CDevice.h"
#include "CPolicy.h"
#include "CMessage.h"

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

int main(int argc, char* argv[])
{
	LoggerManager()->Info("Start Agent Program!");
	try
	{
		std::future<void> a = std::async(std::launch::async, &CMessage::Init, MessageManager());
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