#include "CPolicy.h"


typedef std::vector <std::string> PnameList;
PnameList PnL;

typedef std::vector <std::string> PpathList;
PpathList PpL;

/*
std::string CPolicy::getActivePolicies()
{
}

std::string CPolicy::getInactivatePolicies()
{
}

bool CPolicy::download()
{
}

bool CPolicy::active()
{
}

bool CPolicy::activeFull()
{
}

bool CPolicy::Inactivate()
{
}

bool CPolicy::InactivateFull()
{
}

bool CPolicy::isActive()
{
	
}
*/


void CPolicy::getPname() 
{
	std::string PolicyDirPath = SendToTerminal("find / -name SEDR_Policy");

	std::string sumPath = "/";
	PolicyDirPath += sumPath;

	DIR* dirp = opendir(PolicyDirPath.c_str());
	struct dirent* dp;


	while ((dp = readdir(dirp)) != NULL) {
		PnL.push_back(dp->d_name);
	}
	closedir(dirp);


	for (int i = 0; i < PnL.size(); i++)
	{
		if (PnL[i] == ".") { PnL.erase(PnL.begin() + i); }
	}

	for (int i = 0; i < PnL.size(); i++)
	{
		if (PnL[i] == "..") { PnL.erase(PnL.begin() + i); }
		std::cout << "res : " << PnL[i] << std::endl;
	}
}

void CPolicy::getPpath(std::string PFilename)
{
	std::string Ppath;

	std::string PpathCommTemp1 = "find / -name ";
	std::string PpathComm = PpathCommTemp1 + PFilename;
	
	Ppath = SendToTerminal(PpathComm.c_str());
	PpL.push_back(Ppath);
}
std::string CPolicy::getPCreateTime(std::string PFilePath)
{
	std::string CreateTimeRes;

	std::string CommTemp1 = "stat ";
	std::string CommTemp2 = " | grep Access | grep + | awk '{print $2$3}'";
	std::string CreateTimeComm = CommTemp1 + PFilePath + CommTemp2;

	CreateTimeRes = SendToTerminal(CreateTimeComm.c_str());

	return CreateTimeRes;
}
std::string CPolicy::getPModifyTime(std::string PFilePath)
{
	std::string ModifyTimeRes;

	std::string CommTemp1 = "stat ";
	std::string CommTemp2 = " | grep Modify | grep + | awk '{print $2$3}'";
	std::string ModifyTimeComm = CommTemp1 + PFilePath + CommTemp2;

	ModifyTimeRes = SendToTerminal(ModifyTimeComm.c_str());

	return ModifyTimeRes;
}
std::string CPolicy::getPChangeTime(std::string PFilePath)
{
	std::string ChangeTimeRes;

	std::string CommTemp1 = "stat ";
	std::string CommTemp2 = " | grep Change | grep + | awk '{print $2$3}'";
	std::string ChangeTimeComm = CommTemp1 + PFilePath + CommTemp2;

	ChangeTimeRes = SendToTerminal(ChangeTimeComm.c_str());

	return ChangeTimeRes;
}


void CPolicy::getPolicyInfo()
{
	getPname();

	for (int i = 0; i < PnL.size(); i++)
	{
		getPpath(PnL[i]);

		m_sgetPCreateTime = getPCreateTime(PpL[i]);
		m_sgetPModifyTime = getPModifyTime(PpL[i]);
		m_sgetPChangeTime = getPChangeTime(PpL[i]);


	ST_POLICY_INFO.push_back(policyList(PnL[i], PpL[i], m_sgetPCreateTime, m_sgetPModifyTime, m_sgetPChangeTime));
	
	}
}

bool CPolicy::isExist()
{
	const char* rootsaferPath = "/home/pi/projects/Agent/Build/DebugARM/policy/rootsafer.sh";

	if (access(rootsaferPath, F_OK) != -1)
	{
		printf("\n[+] file is exist!\n");
	}
	else
	{
		printf("\n[-] file is not exist! :(\n");
	}
	return 0;
}



void CPolicy::CPolicyInit()
{
	m_isExist = isExist();
}