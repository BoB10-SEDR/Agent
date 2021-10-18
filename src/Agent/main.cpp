#include <iostream>
#include "Device.h"

int main(int argc, char* argv[])
{
	auto dev = new Device();
	std::string DeviceOS = dev->getDeviceOS();
	std::cout << "[+] OS name : " << DeviceOS << "\n";

	std::string DeviceName = dev->getDeviceName();
	std::cout << "[+] Device name : " << DeviceName << "\n";

	std::string DeviceIpAddr = dev->getDeviceIpAddr();
	std::cout << "[+] Device Ip address : " << DeviceIpAddr << "\n";

	std::string DeviceMacAddr = dev->getDeviceMacAddr();
	std::cout << "[+] Device Mac address : " << DeviceMacAddr << "\n";

	std::string DeviceArchitecture = dev->getDeviceArchitecture();
	std::cout << "[+] Device CPU architecture : " << DeviceArchitecture << "\n";

	/*
	std::string DeviceNetworkInfo = dev->getDeviceNetworkInfo();
	std::cout << "[+] Network range : " << DeviceNetworkInfo << "\n";
	*/

	return 0;
}