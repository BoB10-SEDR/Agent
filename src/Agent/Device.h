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

#include <algorithm>
#include <vector>

struct service {
	std::string name;
	std::string conf;
	std::vector<std::string> log;

	service(void)
	{}
	service(std::string _name, std::string _conf, std::vector<std::string> _log)
		: name(_name), conf(_conf), log(_log)
	{}
};

class Device
{
private:
	std::string m_sDeviceName;
	std::string m_sDeviceModelName;
	std::string m_sDeviceSerialNum;
	std::string m_sDeviceIpAddr;
	std::string m_sDeviceMacAddr;
	std::string m_sDeviceArchitecture;
	std::string m_sDeviceLinuxOsName;

	std::string m_sDeviceNetAdapterName;
	std::string m_sDeviceNetwork;
	std::string m_sDeviceNetworkMask;

	std::vector<struct service> serviceLists;

	std::string SendToTerminal(const char* ShellCommand);

public:
	void DeviceInit();
	void DeviceInfoPrint();

	std::string getDeviceName();
	std::string getDeviceModelName();
	std::string getDeviceSerialNum();
	std::string getDeviceIpAddr();
	std::string getDeviceMacAddr();
	std::string getDeviceArchitecture();
	std::string getDeviceOS();

	void getDeviceNetworkInfo();
	void getDeviceServiceFile(std::vector<struct service> serviceLists);
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