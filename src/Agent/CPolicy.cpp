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
	// 대응 정책 파일 생성 날짜와 실행 날짜가 다르다면 정책 파일이 실행된 적 있다는 것.
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
	// 대응 정책 파일 생성 날짜와 실행날짜가 완전히 같다면 실행된 적 없는 것임.

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


	std::string t_spid = "3";
	std::string t_spname = "policy3.sh";
	m_sactive = active(t_spid, t_spname);
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



/*
* 
bool CPolicy::download()
{
	SendToTerminal("mkdir /SEDR_Policy");
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

// getActivePolicies()는 장비가 가진 대응 정책 파일중에 적용된 애들만 리스트로 반환함. -> 장비에 적용된 정책 새로고침 용도
// isActive() 특정 정책을 찝어서 그 정책이 적용되어있는지 확인하는 기능









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
