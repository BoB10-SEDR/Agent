//
//#include <unistd.h>
//#include <stdlib.h>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <string>
//#include <algorithm>
//#include <pcap.h>
//#include <errno.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <vector>
//using namespace std;
//
//
//
//
//
//string DeviceName;
//string DeviceModelName;
//string DeviceSerialName;
//string DeviceIpAddr;
//string DeviceMacAddr;
//string DeviceArchitecture;
//string DeviceOsName;
//string DeviceLinuxName;
//string DeviceKernelVer;
///* 보류
//    string ModleNum;
//*/
//
//const char* DeviceNameCommand = "hostname";
//const char* DeviceModelNameCommand = "cat /proc/cpuinfo | grep Model | awk '{print $3 $4, $5, $6, $7, $8, $9}'";
//const char* DeviceSerialNameCommand = "cat /proc/cpuinfo | grep Serial | awk '{print $3}'";
//const char* DeviceIpAddrCommand = "ip addr | grep 'dynamic noprefix' | awk '{print $2}'";
//const char* DeviceMacAddrCommand = "ifconfig | grep 'ether' | awk '{print $2}'";
//const char* DeviceArchitectureCommand = "cat /proc/cpuinfo | grep 'model name' | awk '!x[$0]++ {print $4, $5, $6}'";
//const char* DeviceOsNameCommand = "cat /etc/issue | awk '{print $1, $2, $3}'";
//const char* KernelVerCommand = "uname -r";
///* 보류
//    const char *DeviceIpAddrCommand = "hostname -I";
//*/
//
//
//string LiveServices;
//const char* LiveServiceCommand = "service --status-all | grep + | grep -v ?";
//
//
//string LiveServices2;
//const char* LiveServiceCommand2 = "systemctl | awk '{print $1}'";
///* 네트워크 정보 구할 때 ifconfig 파싱 방법 사용 시
//    string NetworkAdapterName;
//    const char *NetworkAdapterCommand = "ifconfig | grep -E'eth0|eth1|eth3|eth4|eth5|enp0s3|enp0s8|wlan0|wlan1|wlan2|wlan3|wlan4|wlan5|wlp1s0' | awk '{print $1}'";
//*/
//
//
//
//
//
///*---------------------------- Global Functions start ---------------------------------*/
//string SendToTerminal(const char* ShellCommand)
//{
//    std::ostringstream	ShellCommandTemp;
//    FILE* SendShellCommand = popen(ShellCommand, "r");
//
//    while (!feof(SendShellCommand) && !ferror(SendShellCommand))
//    {
//        char ShellCommandBuf[128];
//        int ShellBytesRead = fread(ShellCommandBuf, 1, 128, SendShellCommand);
//        ShellCommandTemp.write(ShellCommandBuf, ShellBytesRead);
//    }
//
//    std::string RecievedData = ShellCommandTemp.str();
//    pclose(SendShellCommand);
//
//    return RecievedData;
//}
//
//string EraseAllSpace(string RawData)
//{
//    std::string::size_type pos = 0;
//    std::string::size_type offset = 0;
//    const std::string SpacePattern = "\n";
//    const std::string Replace = "";
//
//    while ((pos = RawData.find(SpacePattern, offset)) != std::string::npos)
//    {
//        RawData.replace(RawData.begin() + pos, RawData.begin() + pos + SpacePattern.size(), Replace);
//        offset = pos + Replace.size();
//    }
//
//    return RawData;
//}
//
//
///*---------------------------- CCollector Functions start ------------------------------*/
//void getDeviceInfo()
//{
//    DeviceName = SendToTerminal(DeviceNameCommand);
//    DeviceModelName = SendToTerminal(DeviceModelNameCommand);
//    DeviceSerialName = SendToTerminal(DeviceSerialNameCommand);
//    DeviceIpAddr = SendToTerminal(DeviceIpAddrCommand);
//    DeviceMacAddr = SendToTerminal(DeviceMacAddrCommand);
//    DeviceArchitecture = SendToTerminal(DeviceArchitectureCommand);
//    DeviceLinuxName = SendToTerminal(DeviceOsNameCommand);
//    DeviceKernelVer = SendToTerminal(KernelVerCommand);
//
//    DeviceOsName = DeviceLinuxName + DeviceKernelVer;
//    DeviceOsName = EraseAllSpace(DeviceOsName);
//}
//
//void getNetworkInfo()
//{
//    /* SendToTerminal()를 사용하여 네트워크 어댑터 이름을 가져올 시
//        NetworkAdapterName = SendToTerminal(NetworkAdapterCommand, NetworkAdapterName);
//        // erase ':' in data from NetworkAdapterName.
//        NetworkAdapterName.erase(find(NetworkAdapterName.begin(), NetworkAdapterName.end(), ':'));
//    */
//
//    char* dev;
//    char* net;
//    char* mask;
//    char errbuf[PCAP_ERRBUF_SIZE];
//    int ret = 0, i = 0, inum = 1;
//
//    pcap_if_t* alldevs;
//    pcap_if_t* d;
//    bpf_u_int32 netp;
//    bpf_u_int32 maskp;
//    struct in_addr addr;
//
//
//    ret = pcap_findalldevs(&alldevs, errbuf);
//    if (ret == -1)
//    {
//        printf("pcap_findalldevs: %s\n", errbuf);
//        exit(1);
//    }
//
//
//    for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);
//
//    ret = pcap_lookupnet(d->name, &netp, &maskp, errbuf);
//    printf("       [+] Net Adapter name    : %s\n", d->name);
//
//    if (ret == -1)
//    {
//        printf("       [+] pcap_lookupnet : %s\n", errbuf);
//        exit(1);
//    }
//
//    addr.s_addr = netp;
//    net = inet_ntoa(addr);
//    if (net == NULL)
//    {
//        perror("inet_ntoa");
//        exit(1);
//    }
//    printf("       [+] Network             : %s\n", net);
//
//    addr.s_addr = maskp;
//    mask = inet_ntoa(addr);
//    if (mask == NULL)
//    {
//        perror("inet_ntoa");
//        exit(1);
//    }
//    printf("       [+] Network mask        : %s\n", mask);
//    pcap_freealldevs(alldevs);
//}
//
//
//void getServiceFile() // searching established(executed and still running in it) services
//{
//    // ssh log, conf file
//    vector<string> sshLog = { "/var/log/btmp","/var/log/auth.log" };
//    string sshConf = "/etc/ssh/sshd_config";
//
//    // vsftp/proftp log, conf file
//    vector<string> vsftpLog = { "/var/log/vsftpd.log", "/var/log/xferlog" };
//    string vsftpConf = "/etc/vsftpd.conf";
//
//    vector<string> proftpLog = { "/var/log/proftpd/proftpd.log", "/var/log/proftpd/xferlog" };
//    string proftpConf = "/usr/local/proftpd/etc/proftpd.conf";
//
//
//
//    /* case 1 service 사용시 (service --status-all)
//        LiveServices = SendToTerminal(LiveServiceCommand);
//    */
//    LiveServices2 = SendToTerminal(LiveServiceCommand2); // case 2 systemctl 사용시
//
//
//    if (LiveServices2.find("ssh.service") != string::npos)
//    {
//        printf("-> I found ssh service from this IoT device !\n");
//        printf("\n   [*] Start check ssh log, config file...\n");
//
//        for (int i = 0; i < sshLog.size(); i++)
//        {
//            ifstream sshlogcheck(sshLog[i]);
//            if (sshlogcheck.fail())
//            {
//                printf("        [!] Can not find ssh log file %s\n", sshLog[i]);
//            }
//            else
//            {
//                cout << "          ssh log file founded !  :   " << sshLog[i];
//                cout << "\n" << endl;
//            }
//        }
//        ifstream sshconfcheck(sshConf);
//        if (sshconfcheck.fail())
//        {
//            printf("      [!] Can not find ssh config file %s\n", sshConf);
//        }
//        else
//        {
//            cout << "          ssh config file founded !  :   " << sshConf;
//            cout << "\n" << endl;
//        }
//    }
//
//
//    if (LiveServices2.find("vsftpd.service") != string::npos)
//    {
//        printf("-> I found vsftp service from this IoT device !\n");
//        printf("\n   [*] Start check vsftplog, config file...\n");
//
//        for (int i = 0; i < vsftpLog.size(); i++)
//        {
//            ifstream vsftplogcheck(vsftpLog[i]);
//            if (vsftplogcheck.fail())
//            {
//                printf("        [!] Can not find vsftp log file %s\n", vsftpLog[i]);
//            }
//            else
//            {
//                cout << "          vsftp vsftp file founded !  :   " << vsftpLog[i];
//                cout << "\n" << endl;
//            }
//        }
//        ifstream vsftpconfcheck(vsftpConf);
//        if (vsftpconfcheck.fail())
//        {
//            printf("      [!] Can not find vsftp config file %s\n", vsftpConf);
//        }
//        else
//        {
//            cout << "          vsftp config file founded !  :   " << vsftpConf;
//            cout << "\n" << endl;
//        }
//    }
//
//
//    if (LiveServices2.find("proftpd.service") != string::npos)
//    {
//
//        printf("-> I found proftp service from this IoT device !\n");
//        printf("\n[*] Start check proftplog, config file...\n");
//
//        for (int i = 0; i < proftpLog.size(); i++)
//        {
//            ifstream proftplogcheck(proftpLog[i]);
//            if (proftplogcheck.fail())
//            {
//                printf("        [!] Can not find proftp log file %s\n", proftpLog[i]);
//            }
//            else
//            {
//                cout << "          proftp file founded !  :   " << proftpLog[i];
//                cout << "\n" << endl;
//            }
//        }
//        ifstream proftpconfcheck(proftpConf);
//        if (proftpconfcheck.fail())
//        {
//            printf("      [!] Can not find proftp config file %s\n", proftpConf);
//        }
//        else
//        {
//            cout << "          proftp config file founded !  :   " << proftpConf;
//            cout << "\n" << endl;
//        }
//    }
//}
//
//
//
//int main()
//{
//    getDeviceInfo();
//
//
//    /*-- bellow codes are just for a testing--*/
//    printf("#--------------------- Device Info ----------------------#\n\n");
//    printf("getDeviceInfo()\n");
//
//    cout << "       [+] Device name        :   " << DeviceName;
//    cout << "       [+] Model name         :   " << DeviceModelName;
//    cout << "       [+] Serial Number      :   " << DeviceSerialName;
//    cout << "       [+] Device Ip address  :   " << DeviceIpAddr;
//    cout << "       [+] Device Mac address :   " << DeviceMacAddr;
//    cout << "       [+] CPU architecture   :   " << DeviceArchitecture;
//    cout << "       [+] OS name            :   " << DeviceOsName << "\n";
//
//    printf("#--------------------- Network Info ----------------------#\n\n");
//    printf("getNetworkInfo()\n");
//    getNetworkInfo();
//    /* ifconfig 파싱하여 네트워크 어댑터 이름 가져올 시
//        cout << "       [+] Network Adapter    :   " << NetworkAdapterName;
//    */
//
//    printf("#--------------------- Service Info ----------------------#\n\n");
//    printf("Search established services\n");
//
//    LiveServices = SendToTerminal(LiveServiceCommand); // 결과 이쁘게
//    cout << "\n" << LiveServices;
//    sleep(2);
//    printf("#--------------- Service Log/Config file ----------------#\n\n");
//    getServiceFile();
//
//    /* systemctl 사용시 systemctl | awk '{print $1}' 명령어의 결과가 저장되는 변수
//        cout << "\n" << LiveServices2;
//    */
//
//    printf("#---------------------------------------------------------#\n");
//    return 0;
//}