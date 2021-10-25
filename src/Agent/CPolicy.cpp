#include "CPolicy.h"

typedef std::vector <std::string> PnameList;
PnameList PnL;

typedef std::vector <std::string> PpathList;
PpathList PpL;

std::vector<policyInfo>::size_type st;

/*-------------------------------------------------------------------------------------------------------*/
std::string CPolicy::getPname()
{
	std::string PolicyDirPath = SendToTerminal("find / -name SEDR_Policy");
	std::string sumPath = "/";
	PolicyDirPath += sumPath;
	DIR* dirp = opendir(PolicyDirPath.c_str());
	struct dirent* dp;

	/*	if policy directory doesn't exist, make directory and call download().
	download();
	*/

	std::string PolicyName;
	while ((dp = readdir(dirp)) != NULL) {
		PnL.push_back(dp->d_name);
		PolicyName = dp->d_name;
	}
	closedir(dirp);

	for (int i = 0; i < PnL.size(); i++)
	{
		if (PnL[i] == ".") { PnL.erase(PnL.begin() + i); }
	}

	for (int i = 0; i < PnL.size(); i++)
	{
		if (PnL[i] == "..") { PnL.erase(PnL.begin() + i); }
		//std::cout << "res : " << PnL[i] << std::endl;
	}
	return PolicyName;
}


/*-------------------------------------------------------------------------------------------------------*/
std::string CPolicy::getPpath(std::string PFilename)
{
	std::string Ppath;

	std::string PpathCommTemp1 = "find / -name ";
	std::string PpathComm = PpathCommTemp1 + PFilename;
	
	Ppath = SendToTerminal(PpathComm.c_str());
	PpL.push_back(Ppath);


	return Ppath;
}


/*-------------------------------------------------------------------------------------------------------*/
std::string CPolicy::getPCreateTime(std::string PFilePath)
{
	std::string CreateTimeRes;

	std::string CommTemp1 = "stat ";
	std::string CommTemp2 = " | grep Access | grep + | awk '{print $2,$3}'";
	std::string CreateTimeComm = CommTemp1 + PFilePath + CommTemp2;

	CreateTimeRes = SendToTerminal(CreateTimeComm.c_str());

	return CreateTimeRes;
}


/*-------------------------------------------------------------------------------------------------------*/
std::string CPolicy::getPModifyTime(std::string PFilePath)
{
	std::string ModifyTimeRes;

	std::string CommTemp1 = "stat ";
	std::string CommTemp2 = " | grep Modify | grep + | awk '{print $2,$3}'";
	std::string ModifyTimeComm = CommTemp1 + PFilePath + CommTemp2;

	ModifyTimeRes = SendToTerminal(ModifyTimeComm.c_str());

	return ModifyTimeRes;
}


/*-------------------------------------------------------------------------------------------------------*/
std::string CPolicy::getPChangeTime(std::string PFilePath)
{
	std::string ChangeTimeRes;

	std::string CommTemp1 = "stat ";
	std::string CommTemp2 = " | grep Change | grep + | awk '{print $2,$3}'";
	std::string ChangeTimeComm = CommTemp1 + PFilePath + CommTemp2;

	ChangeTimeRes = SendToTerminal(ChangeTimeComm.c_str());

	return ChangeTimeRes;
}


/*-------------------------------------------------------------------------------------------------------*/
bool CPolicy::isExist()
{ // find policy file and check exist. return value -> exist : 0, non-exist : 1
	int isExistFile;

	for (int i = 0; i < PnL[i].size(); i++)
	{
		if (access(PnL[i].c_str(), F_OK) != -1)
			isExistFile = 0;
		else { isExistFile = 1; }
	}
	return isExistFile;
}


/*-------------------------------------------------------------------------------------------------------*/
std::string CPolicy::getActivePolicies()
{
	std::string activatedpolicyInfo;


	for (st = 0; st < PolicyList.size(); st++)
	{
		if (PolicyList[st].APCreateTime != PolicyList[st].APChageTime)
		{
			printf("#-------------------------------------------------------#\n");
			std::cout << "[+] policy has been activated : " << PolicyList[st].APolicyName << std::endl;
			printf("#-------------------------------------------------------#\n");
		}
	}
	return activatedpolicyInfo;
}


/*-------------------------------------------------------------------------------------------------------*/
std::string CPolicy::getInactivatePolicies()
{

	std::string InactivatedpolicyInfo;

	for (st = 0; st < PolicyList.size(); st++)
	{
		if (PolicyList[st].APCreateTime == PolicyList[st].APChageTime)
		{
			printf("#-------------------------------------------------------#\n");
			std::cout << "[-] policy has been Inactivated : " << PolicyList[st].APolicyName << std::endl;
			printf("#-------------------------------------------------------#\n");
		}
	}
	return InactivatedpolicyInfo;
}


/*-------------------------------------------------------------------------------------------------------*/
void CPolicy::getPolicyInfo()
{
	m_sgetPname = getPname();
	m_sisExist = isExist();

	for (int i = 0; i < PnL.size(); i++)
	{
		getPpath(PnL[i]);

		m_sgetPCreateTime = getPCreateTime(PpL[i]);
		m_sgetPModifyTime = getPModifyTime(PpL[i]);
		m_sgetPChangeTime = getPChangeTime(PpL[i]);

		PolicyList.push_back(policyInfo(PnL[i], PpL[i], m_sgetPCreateTime, m_sgetPModifyTime, m_sgetPChangeTime));
	}

	m_sgetActivePolicies = getActivePolicies();
	m_sgetInactivatePolicies = getInactivatePolicies();


	std::string t_spid = "1";
	std::string t_spname = "policy1.sh";
	m_sactive = active(t_spid, t_spname);
	m_sactiveFull = activeFull();
}


bool CPolicy::active(std::string sPid, std::string sPName)
{
	int ActiveRes;
	std::string ActiveComm;

	for (int i = 0; i < PnL[i].size(); i++)
	{
		if (PnL[i] == sPName)
		{
			ActiveComm = PpL[i];
			SendToTerminal(ActiveComm.c_str());
			ActiveRes = 0;
		}
		else
		{
			//download(sPid, sPName);
			ActiveRes = 1;
		}
	}
	return ActiveRes;
}
bool CPolicy::activeFull()
{
	int ActiveFullRes;

	for (int x = 0; x < PpL[x].size(); x++)
	{
		SendToTerminal(PpL[x].c_str());

		ActiveFullRes = 0;
	}
	return ActiveFullRes;
}


/*
* 
bool CPolicy::download()
{// download() 실행 조건 -> 서버로부터 api를 통해 특정 정책을 적용하라는 명령이 온다면
// 해당 데이터에 포함된 정책 파일명이 Agent 본인이 관리하고 있는지(파일이 있는지) 확인하고
// 있다면 active() 함수로 값을 넘기고, 없다면 서버에 GET 요청한다.


	SendToTerminal("mkdir /SEDR_Policy");
	// 먼저 정책 스크립트를 생성할 디렉토리를 만든다.
	// 만약 같은 이름의 디렉토리가 있다면 그냥 넘어갈 것.
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










void CPolicy::PolicyInfoPrint()
{
	printf("[!] PolicyList print");

	for (st = 0; st < PolicyList.size(); st++)
	{
		printf("\n");
		std::cout << "[+] policy name : " << PolicyList[st].APolicyName << std::endl;
		std::cout << "[+] policy path : " << PolicyList[st].APolicyPath << std::endl;
		std::cout << "[+] policy create time : " << PolicyList[st].APCreateTime << std::endl;
		std::cout << "[+] policy modify time : " << PolicyList[st].APModifyTime << std::endl;
		std::cout << "[+] policy execute time : " << PolicyList[st].APChageTime << std::endl;
	}
}
