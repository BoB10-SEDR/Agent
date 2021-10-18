#pragma once
#include <string>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <pcap.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


class Device
{
private:
	std::string SendToTerminal(const char* ShellCommand);

	std::string m_sDeviceName;
	std::string m_sDeviceModelName;
	std::string m_sDeviceSerialName;
	std::string m_sDeviceIpAddr;
	std::string m_sDeviceMacAddr;
	std::string m_sDeviceArchitecture;
	std::string m_sDeviceLinuxOsName;

	//std::string m_DeviceNetworkInfo;
	//std::string m_sDeviceServiceFile;

public:
	std::string getDeviceName();
	std::string getDeviceModelName();
	std::string getSerialNum();
	std::string getDeviceIpAddr();
	std::string getDeviceMacAddr();
	std::string getDeviceArchitecture();
	std::string getDeviceOS();

	std::string getNetAdapterName();

	//std::string getDeviceNetworkInfo();
	//std::string getDeviceServiceFile();
};



#if 0
typedef struct pcap_if pcap_if_t;
struct pcap_if {
	struct pcap_if* next;
	char* name;
	char* description;
	struct pcap_addr* addresses;
	u_int flags;
};
typedef struct pcap_addr pcap_addr_t;
struct pcap_addr {
	struct pcap_addr* next;


	struct sockaddr* addr;
	struct sockaddr* netmask;
	struct sockaddr* broadaddr;
	struct sockaddr* dstaddr;
};
#endif