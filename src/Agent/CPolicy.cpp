#include "stdafx.h"
#include "CPolicy.h"


typedef std::vector <std::string> PnameList;
PnameList PnL;

typedef std::vector <std::string> PpathList;
PpathList PpL;

std::vector<policy_Info>::size_type st;



CPolicy::CPolicy()
{

}

CPolicy::~CPolicy()
{

}

/*-------------------------------------------------------------------------------------------------------*/
std::string CPolicy::getPname()
{
	std::string PolicyDirPath = SendToTerminal("find / -name SEDR_Policy");
	std::string sumPath = "/";

	PolicyDirPath += sumPath;
	DIR* dirp = opendir(PolicyDirPath.c_str());
	struct dirent* dp;

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

	for (st = 0; st < CPolicy::PolicyList.size(); st++)
	{
		if (PpL[st] == Ppath)
		{ // PpL 리스트에 이미 값이 있다면 구한 경로 반환
			return Ppath;
		}
		else
		{ // 없다면 PpL 리스트에 넣기
			PpL.push_back(Ppath);
		}
	}
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


struct policy_Info* CPolicy::SearchPolicy(std::string key)
{
	struct policy_Info* result = PolicyList[key];
	return result;

}
std::string CPolicy::GeneratePolicyKey(ST_POLICY_INFO* policyServerInfo)
{
	return StringFormatter("%d_%s_%s", policyServerInfo->idx, policyServerInfo->name.c_str(), policyServerInfo->version.c_str());
}

/*-------------------------------------------------------------------------------------------------------*/
bool CPolicy::isExist(ST_POLICY_INFO* policyServerInfo)
{ 
	LoggerManager()->Info("isExist Execute");

	// 1. map 검색을 수행 우리가 원하는 정책이 있는지 검색 수행
	std::string policyKey = GeneratePolicyKey(policyServerInfo);
	struct policy_Info* pinfo = SearchPolicy(policyKey);

	if (pinfo == NULL) {
		//정책이 존재하지 않는다.
		LoggerManager()->Info("Policy Not Exist");
		return false;
	}
	else {
		LoggerManager()->Info("Policy Exist");
	}

	// 2. 실제로 파일이 있는지 확인
	if (access(pinfo->APolicyName.c_str(), F_OK) != -1) {	//실제 장비에 풀 확장자파일이름이 존재하는지
		return true;
	}

	return false;
}


/*-------------------------------------------------------------------------------------------------------*/
std::string CPolicy::getActivePolicies()
{
	// 1. Agent가 가지고 있는 정책 중에 최초 파일 생성 날짜 이후에 실행한 적이 있는지 확인한다.


	for (auto iter = PolicyList.begin(); iter != PolicyList.end(); ++iter)
	{
		struct policy_Info* info = iter->second;
		if (info == NULL) {
			LoggerManager()->Warn("PolicyList NULL");
		}
		else {
			LoggerManager()->Info(StringFormatter("Idx : %s, SName : %s, APolicyName : %s, APolicyPath : %s", info->SId, info->SName, info->APolicyName, info->APolicyPath));

		}
	}
	

	return "";
/*
	std::string activatedpolicyInfo;

	for (st = 0; st < PolicyList.size(); st++)
	{
		if (PolicyList[st]APCreateTime != PolicyList[st].APChageTime)
		{
			printf("#-------------------------------------------------------#\n");
			std::cout << "[+] policy has been activated : " << PolicyList[st].APolicyName << std::endl;
			printf("#-------------------------------------------------------#\n");
		}
	}
	return activatedpolicyInfo;
*/
}


/*-------------------------------------------------------------------------------------------------------*/
std::string CPolicy::getInactivatePolicies()
{

	std::string InactivatedpolicyInfo;

	//for (st = 0; st < PolicyList.size(); st++)
	//{
	//	if (PolicyList[st].APCreateTime == PolicyList[st].APChageTime)
	//	{
	//		printf("#-------------------------------------------------------#\n");
	//		std::cout << "[-] policy has been Inactivated : " << PolicyList[st].APolicyName << std::endl;
	//		printf("#-------------------------------------------------------#\n");
	//	}
	//}
	return InactivatedpolicyInfo;
}


/*-------------------------------------------------------------------------------------------------------*/
void CPolicy::getPolicyInfo()
{
	//m_sgetPname = getPname();
	//m_sisExist = isExist();

	//for (int i = 0; i < PnL.size(); i++)
	//{
	//	getPpath(PnL[i]);

	//	m_sgetPCreateTime = getPCreateTime(PpL[i]);
	//	m_sgetPModifyTime = getPModifyTime(PpL[i]);
	//	m_sgetPChangeTime = getPChangeTime(PpL[i]);


	//	//PolicyList.push_back(policyInfo(PnL[i], PpL[i], m_sgetPCreateTime, m_sgetPModifyTime, m_sgetPChangeTime));
	//}

	//m_sgetActivePolicies = getActivePolicies();
	//m_sgetInactivatePolicies = getInactivatePolicies();


	//std::string t_spid = "3";
	//std::string t_spname = "policy3.sh";
	//std::string t_sversion = "0.0.1";
	//m_sactive = active(t_spid, t_spname, t_sversion);
	//m_sactiveFull = activeFull();
}

/*-------------------------------------------------------------------------------------------------------*/
bool CPolicy::active(ST_POLICY_INFO* policyServerInfo)
{
	LoggerManager()->Info("Active policy");

	// 1. map 검색을 수행 우리가 원하는 정책이 있는지 검색 수행
	std::string policyKey = GeneratePolicyKey(policyServerInfo);
	struct policy_Info* pinfo = SearchPolicy(policyKey);

	int ActiveRes = false;

	if (pinfo != NULL)
	{
		SendToTerminal(StringFormatter("sh %s", pinfo->APolicyPath).c_str());
		ActiveRes = true;
	}

	return ActiveRes;
}


bool CPolicy::activeFull()
{
	LoggerManager()->Info("Active All policy");


	int ActiveFullRes = false;

	for (int x = 0; x < PnL.size(); x++)
	{

		SendToTerminal(StringFormatter("sudo %s", PpL[x]).c_str());
		//std::string isActive = SendToTerminal("echo $?");
		//SendToTerminal(PpL[x].c_str());

		ActiveFullRes = true;
	}
	return ActiveFullRes;
}


/*-------------------------------------------------------------------------------------------------------*/
bool CPolicy::Inactivate(ST_POLICY_INFO* policyServerInfo)
{
	LoggerManager()->Info("InActive policy");

	std::string policyKey = GeneratePolicyKey(policyServerInfo);
	struct policy_Info* pinfo = SearchPolicy(policyKey);
	
	int InActiveRes = false;

	if (pinfo != NULL)
	{
		SendToTerminal(StringFormatter("sh %s", pinfo->APolicyPath).c_str());

		InActiveRes = true;
	}

	return InActiveRes;
}


bool CPolicy::InactivateFull()
{
	LoggerManager()->Info("Inactive All policy");

	int InActiveFullRes = false;

	for (int x = 0; x < PnL.size(); x++)
	{

		SendToTerminal(StringFormatter("sudo %s", PpL[x]).c_str());
		//std::string isActive = SendToTerminal("echo $?");

		InActiveFullRes = true;
	}
	return InActiveFullRes;
}


/*-------------------------------------------------------------------------------------------------------*/
bool CPolicy::isActive(ST_POLICY_INFO* policyServerInfo)
{
	// 1. map 검색을 수행 우리가 원하는 정책이 있는지 검색 수행
	LoggerManager()->Info("SuccessPolicy Execute");

	std::string policyKey = GeneratePolicyKey(policyServerInfo);
	struct policy_Info* pinfo = SearchPolicy(policyKey);

	getActivePolicies();	//map에 값이 제대로 들어갔는지

	int result = true;

	// 2. pinfo 가 NULL값이 아니라면, 정책 파일이 생성한 로그 파일을 찾는다.
	// 파일 기반의 정책 파일들은 실행하자마자 자신의 실행 로그를 남긴다. (ex 형식이 '정책파일명.log'라고 한다면)
	if (pinfo != NULL) {
		std::string logPath = StringFormatter("%s.log", pinfo->APolicyPath.c_str());
		LoggerManager()->Info(StringFormatter("Log PATH : %s.log", logPath));

		int result = false;

		printf("[*] Logfile Check start\n");
		if (access(pinfo->APolicyPath.c_str(), F_OK) != -1)
		{
			printf("[+] Logfile Founded!\n");
			// 여기에 로그 파일 내용중에 [success]라는 단어가 있는지 찾는 로직을 넣어야함.
			return true;
		}
		else
		{
			printf("[!] Logfile Not founded! checking 'echo $?'command\n");
			// 여기에 echo $?해서 정책 파일의 성공 여부를 확인하고 맞다면 result = true 해줘야함.
			result = false;
		}
		printf("[*] Logfile Check end\n");


		return result;
	}


	LoggerManager()->Info("SuccessPolicy End");

	if (result)
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*-------------------------------------------------------------------------------------------------------*/
bool CPolicy::download(ST_POLICY_INFO* policyServerInfo)
{// download() 실행 조건 -> 서버로부터 api를 통해 특정 정책을 적용하라는 명령이 온다면
// 해당 데이터에 포함된 정책 파일명이 Agent 본인이 관리하고 있는지(파일이 있는지) 확인하고
// 있다면 active() 함수로 값을 넘기고, 없다면 서버에 GET 요청한다.
	LoggerManager()->Info("Policy Download Execute");

	std::string policyKey = GeneratePolicyKey(policyServerInfo);
	// 1. 파일 다운로드 후 저장



	// 2-1. 기존에 PolicyList에 존재하면 이름 및 다운로드 경로 업데이트
	// 2-2. PolicyList에 추가

	struct policy_Info* pinfo = SearchPolicy(policyKey);

	if (pinfo == NULL)
	{
		LoggerManager()->Info("New Policy");

		struct policy_Info* tmp = new struct policy_Info();
		tmp->SId = policyServerInfo->idx;
		tmp->SName = policyServerInfo->name;

		tmp->APolicyName = policyKey;
		std::string tmpPpathKey = getPpath(policyKey);
		tmp->APolicyPath = tmpPpathKey;

		LoggerManager()->Info(tmpPpathKey);

		tmp->APCreateTime = getPCreateTime(tmpPpathKey);
		tmp->APModifyTime = getPModifyTime(tmpPpathKey);
		tmp->APChageTime = getPChangeTime(tmpPpathKey);

		PolicyList.insert(std::pair<std::string, struct policy_Info*>(policyKey, tmp));
		getActivePolicies();	//map에 값이 제대로 들어갔는지
	}
	else {
		LoggerManager()->Info("Policy Update");

		pinfo->APolicyName = policyKey;
		
		pinfo->APolicyName = policyKey;
		std::string PpathKey = getPpath(policyKey);
		pinfo->APolicyPath = PpathKey;

		pinfo->APCreateTime = getPCreateTime(PpathKey);
		pinfo->APModifyTime = getPModifyTime(PpathKey);
		pinfo->APChageTime = getPChangeTime(PpathKey);
	}

	return true;

}


bool CPolicy::getFileFromHttp(char* pszUrl, char* pszFile)
{

}

/*-------------------------------------------------------------------------------------------------------*/
/*
void CPolicy::PolicyInfoPrint()
{
	printf("[!] PolicyList print");
	for (st = 0; st < CPolicy::PolicyList.size(); st++)
	{
		printf("\n");
		std::cout << "[+] policy name : " << PolicyList[st].APolicyName << std::endl;
		std::cout << "[+] policy path : " << PolicyList[st].APolicyPath << std::endl;
		std::cout << "[+] policy create time : " << PolicyList[st].APCreateTime << std::endl;
		std::cout << "[+] policy modify time : " << PolicyList[st].APModifyTime << std::endl;
		std::cout << "[+] policy execute time : " << PolicyList[st].APChageTime << std::endl;
	}
}
*/

/*-------------------------------------------------------------------------------------------------------*/
CPolicy* CPolicy::GetInstance()
{
	static CPolicy instance;
	return &instance;
}