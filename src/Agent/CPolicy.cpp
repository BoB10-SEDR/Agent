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
		{ // PpL ����Ʈ�� �̹� ���� �ִٸ� ���� ��� ��ȯ
			return Ppath;
		}
		else
		{ // ���ٸ� PpL ����Ʈ�� �ֱ�
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

	// 1. map �˻��� ���� �츮�� ���ϴ� ��å�� �ִ��� �˻� ����
	std::string policyKey = GeneratePolicyKey(policyServerInfo);
	struct policy_Info* pinfo = SearchPolicy(policyKey);

	if (pinfo == NULL) {
		//��å�� �������� �ʴ´�.
		LoggerManager()->Info("Policy Not Exist");
		return false;
	}
	else {
		LoggerManager()->Info("Policy Exist");
	}

	// 2. ������ ������ �ִ��� Ȯ��
	if (access(pinfo->APolicyName.c_str(), F_OK) != -1) {	//���� ��� Ǯ Ȯ���������̸��� �����ϴ���
		return true;
	}

	return false;
}


/*-------------------------------------------------------------------------------------------------------*/
std::string CPolicy::getActivePolicies()
{
	// 1. Agent�� ������ �ִ� ��å �߿� ���� ���� ���� ��¥ ���Ŀ� ������ ���� �ִ��� Ȯ���Ѵ�.


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

	// 1. map �˻��� ���� �츮�� ���ϴ� ��å�� �ִ��� �˻� ����
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
	// 1. map �˻��� ���� �츮�� ���ϴ� ��å�� �ִ��� �˻� ����
	LoggerManager()->Info("SuccessPolicy Execute");

	std::string policyKey = GeneratePolicyKey(policyServerInfo);
	struct policy_Info* pinfo = SearchPolicy(policyKey);

	getActivePolicies();	//map�� ���� ����� ������

	int result = true;

	// 2. pinfo �� NULL���� �ƴ϶��, ��å ������ ������ �α� ������ ã�´�.
	// ���� ����� ��å ���ϵ��� �������ڸ��� �ڽ��� ���� �α׸� �����. (ex ������ '��å���ϸ�.log'��� �Ѵٸ�)
	if (pinfo != NULL) {
		std::string logPath = StringFormatter("%s.log", pinfo->APolicyPath.c_str());
		LoggerManager()->Info(StringFormatter("Log PATH : %s.log", logPath));

		int result = false;

		printf("[*] Logfile Check start\n");
		if (access(pinfo->APolicyPath.c_str(), F_OK) != -1)
		{
			printf("[+] Logfile Founded!\n");
			// ���⿡ �α� ���� �����߿� [success]��� �ܾ �ִ��� ã�� ������ �־����.
			return true;
		}
		else
		{
			printf("[!] Logfile Not founded! checking 'echo $?'command\n");
			// ���⿡ echo $?�ؼ� ��å ������ ���� ���θ� Ȯ���ϰ� �´ٸ� result = true �������.
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
{// download() ���� ���� -> �����κ��� api�� ���� Ư�� ��å�� �����϶�� ����� �´ٸ�
// �ش� �����Ϳ� ���Ե� ��å ���ϸ��� Agent ������ �����ϰ� �ִ���(������ �ִ���) Ȯ���ϰ�
// �ִٸ� active() �Լ��� ���� �ѱ��, ���ٸ� ������ GET ��û�Ѵ�.
	LoggerManager()->Info("Policy Download Execute");

	std::string policyKey = GeneratePolicyKey(policyServerInfo);
	// 1. ���� �ٿ�ε� �� ����



	// 2-1. ������ PolicyList�� �����ϸ� �̸� �� �ٿ�ε� ��� ������Ʈ
	// 2-2. PolicyList�� �߰�

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
		getActivePolicies();	//map�� ���� ����� ������
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