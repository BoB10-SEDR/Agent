#include "Device.h"

/*===========================================================================================================================*/
std::string Device::getDeviceOS()
{
	std::string OsNameBuf;
	std::string KernelVersionBuf;

	const char* OsNameCommand = "cat /etc/issue | awk '{print $1, $2, $3}'";
	const char* KernekVersionCommand = "uname -r";

	OsNameBuf = SendToTerminal(OsNameCommand);
	KernelVersionBuf = SendToTerminal(KernekVersionCommand);

	return OsNameBuf + KernelVersionBuf;
}

std::string Device::getDeviceName()
{
	std::string DeviceNameBuf;
	const char* DeviceNameCommand = "hostname";

	DeviceNameBuf = SendToTerminal(DeviceNameCommand);

	return DeviceNameBuf;
}

std::string Device::getDeviceModelName()
{
	std::string ModelNameBuf;
	const char* ModelNameCommand = "cat / proc / cpuinfo | grep Model | awk '{print $3 $4, $5, $6, $7, $8, $9}'";

	ModelNameBuf = SendToTerminal(ModelNameCommand);

	return ModelNameBuf;
}

std::string Device::getSerialNum()
{
	std::string SerialNumBuf;
	const char* SerialNumCommand = "cat /proc/cpuinfo | grep Serial | awk '{print $3}'";

	SerialNumBuf = SendToTerminal(SerialNumCommand);

	return SerialNumBuf;
}

std::string Device::getDeviceIpAddr()
{
	std::string DeviceIpAddrBuf;
	const char* DeviceIpAddrCommand = "ifconfig | grep 'ether' | awk '{print $2}'";

	DeviceIpAddrBuf = SendToTerminal(DeviceIpAddrCommand);

	return DeviceIpAddrBuf;
}

std::string Device::getDeviceMacAddr()
{
	std::string DeviceMacAddrBuf;
	const char* DeviceMacAddrCommand = "ifconfig | grep 'ether' | awk '{print $2}'";

	DeviceMacAddrBuf = SendToTerminal(DeviceMacAddrCommand);

	return DeviceMacAddrBuf;
}

std::string Device::getDeviceArchitecture()
{
	std::string DeviceArchitectureBuf;
	const char* DeviceArchitectureCommand = "cat /proc/cpuinfo | grep 'model name' | awk '!x[$0]++ {print $4, $5, $6}'";

	DeviceArchitectureBuf = SendToTerminal(DeviceArchitectureCommand);

	return DeviceArchitectureBuf;
}


/*===========================================================================================================================*/


/*
std::string Device::getDeviceNetworkInfo()
{

}
*/

/*
std::string Device::getDeviceServiceFile()
{

}
*/


/*===========================================================================================================================*/


std::string Device::SendToTerminal(const char* ShellCommand)
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
