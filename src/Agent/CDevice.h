#pragma once
#include "stdafx.h"

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

struct DeviceList
{
	std::string DName;
	std::string SerialNumber;
	std::string modelName;
	std::string os;
	std::string IpAddr;
	std::string MacAddr;
	std::string CPUArch;

	DeviceList(void) {}
	DeviceList(std::string _dname, std::string _serialnumber, std::string _modelnumber, std::string _os, std::string _ipaddr, std::string _macaddr, std::string _cpuarch)
		: DName(_dname), SerialNumber(_serialnumber), modelName(_modelnumber), os(_os), IpAddr(_ipaddr), MacAddr(_macaddr), CPUArch(_cpuarch)
	{}
};


class CDevice
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
	std::vector<struct DeviceList> ST_DEVICE_INFO;


public:
	void DeviceInit();
	void DeviceInfoPrint();
	void GetDeviceInfo();

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