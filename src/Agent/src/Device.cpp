#include "../header/stdafx.h"
#include "../header/Device.h"


std::string Device::getDeviceName()
{
	std::string DeviceNameBuf = SendToTerminal("hostname");
	return DeviceNameBuf;
}

std::string Device::getDeviceModelName()
{
	std::string ModelNameBuf = SendToTerminal("cat /proc/cpuinfo | grep Model | awk '{print $3 $4, $5, $6, $7, $8, $9}'");
	return ModelNameBuf;
}

std::string Device::getDeviceSerialNum()
{
	std::string SerialNumBuf = SendToTerminal("cat /proc/cpuinfo | grep Serial | awk '{print $3}'");
	return SerialNumBuf;
}

std::string Device::getDeviceIpAddr()
{
	std::string DeviceIpAddrBuf = SendToTerminal("ip addr | grep 'dynamic noprefix' | awk '{print $2}'");
	return DeviceIpAddrBuf;
}

std::string Device::getDeviceMacAddr()
{
	std::string DeviceMacAddrBuf = SendToTerminal("ifconfig | grep 'ether' | awk '{print $2}'");
	return DeviceMacAddrBuf;
}

std::string Device::getDeviceArchitecture()
{
	std::string DeviceArchitectureBuf = SendToTerminal("cat /proc/cpuinfo | grep 'model name' | awk '!x[$0]++ {print $4, $5, $6}'");
	return DeviceArchitectureBuf;
}

std::string Device::getDeviceOS()
{
	std::string OsNameBuf = SendToTerminal("cat /etc/issue | awk '{print $1, $2, $3}'");
	std::string KernelVersionBuf = SendToTerminal("uname -r");
	
	return OsNameBuf + KernelVersionBuf;
}

void Device::getDeviceNetworkInfo()
{
	char errbuf[PCAP_ERRBUF_SIZE];
	int i = 0, inum = 1;

	pcap_if_t* alldevs;
	pcap_if_t* device;
	bpf_u_int32 netp;
	bpf_u_int32 maskp;
	struct in_addr addr;

	int ret = pcap_findalldevs(&alldevs, errbuf);
	if (ret == -1)
	{
		printf("pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}

	for (device = alldevs, i = 0; i < inum - 1; device = device->next, i++);

	ret = pcap_lookupnet(device->name, &netp, &maskp, errbuf);
	m_sDeviceNetAdapterName = device->name;

	if (ret == -1)
	{
		printf("[+] pcap_lookupnet : % s\n", errbuf);
		return;
	}

	addr.s_addr = netp;
	m_sDeviceNetwork = inet_ntoa(addr);

	if (m_sDeviceNetwork.c_str() == NULL)
	{
		perror("inet_ntoa");
		return;
	}

	addr.s_addr = maskp;
	m_sDeviceNetworkMask = inet_ntoa(addr);

	if (m_sDeviceNetworkMask.c_str() == NULL)
	{
		perror("inet_ntoa");
		return;
	}

	pcap_freealldevs(alldevs);
}

void Device::getDeviceServiceFile(std::vector<struct service> serviceLists)
{
	std::string LiveServices2 = SendToTerminal("systemctl | awk '{print $1}'"); // case 2 systemctl »ç¿ë½Ã

	for (int i = 0; i < serviceLists.size(); i++)
	{
		if (LiveServices2.find(serviceLists[i].name) != std::string::npos)
		{
			printf("[+] find %s \n", serviceLists[i].name.c_str());
			for (int j = 0; j < serviceLists[i].log.size(); j++)
			{
				std::ifstream logcheck(serviceLists[i].log[j]);
				if (logcheck.fail())
					printf("[!] Can not find %s log file %s\n", serviceLists[i].name.c_str(), serviceLists[i].log[j].c_str());
				else
					printf("[+] %s log file founded ! : %s \n", serviceLists[i].name.c_str(), serviceLists[i].log[j].c_str());
			}
			std::ifstream confcheck(serviceLists[i].conf);
			if (confcheck.fail())
				printf("[!] Can not find %s config file %s\n", serviceLists[i].name.c_str(), serviceLists[i].conf.c_str());
			else
				printf("[+] %s conf file founded ! : %s \n", serviceLists[i].name.c_str(), serviceLists[i].conf.c_str());
		}
		else
			printf("[!] Can not find %s \n", serviceLists[i].name.c_str());
	}
}

/*===========================================================================================================================*/


void Device::getDeviceInfo()
{
	m_sDeviceName = getDeviceName();
	m_sDeviceModelName = getDeviceModelName();
	m_sDeviceSerialNum = getDeviceSerialNum();
	m_sDeviceIpAddr = getDeviceIpAddr();
	m_sDeviceMacAddr = getDeviceMacAddr();
	m_sDeviceArchitecture = getDeviceArchitecture();
	m_sDeviceLinuxOsName = getDeviceOS();

	ST_DEVICE_INFO.push_back(DeviceList(m_sDeviceName, m_sDeviceSerialNum, m_sDeviceModelName, m_sDeviceLinuxOsName, m_sDeviceIpAddr, m_sDeviceMacAddr, m_sDeviceArchitecture));
}

void Device::DeviceInit()
{
	// service List init
	serviceLists.push_back(service("ssh.service", "/etc/ssh/sshd_config", {"/var/log/btmp","/var/log/auth.log"}));
	serviceLists.push_back(service("vsftpd.service", "/etc/vsftpd.conf", {"/var/log/vsftpd.log", "/var/log/xferlog"}));
	serviceLists.push_back(service("proftpd.service", "/usr/local/proftpd/etc/proftpd.conf", {"/var/log/proftpd/proftpd.log", "/var/log/proftpd/xferlog"}));

	getDeviceNetworkInfo();
}

void Device::DeviceInfoPrint()
{
	printf("[+] Device name : %s\n", m_sDeviceName.c_str());
	printf("[+] Device modelName : %s\n", m_sDeviceModelName.c_str());
	printf("[+] Device SerialNum : %s\n", m_sDeviceSerialNum.c_str());
	printf("[+] Device IP Address : %s\n", m_sDeviceIpAddr.c_str());
	printf("[+] Device Mac Address : %s\n", m_sDeviceMacAddr.c_str());
	printf("[+] Device CPU architecture : %s\n", m_sDeviceArchitecture.c_str());
	printf("[+] Device OS : %s\n\n", m_sDeviceLinuxOsName.c_str());

	printf("[+] Device Network Adapter Name : %s\n", m_sDeviceNetAdapterName.c_str());
	printf("[+] Device Network Address : %s\n", m_sDeviceNetwork.c_str());
	printf("[+] Device Network Mask : %s\n\n", m_sDeviceNetworkMask.c_str());

	getDeviceServiceFile(serviceLists);
}