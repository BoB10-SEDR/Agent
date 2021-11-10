#pragma once
#include "stdafx.h"
#include <map>

struct time_info 
{
	std::tstring createTime;
	std::tstring modifyTime;
	std::tstring changeTime;
};

struct policy_Info {
	/* 서버로부터 받은 정책 정보 */
	std::string SIdx; // 서버의 DB에 저장된 각 대응 정책 파일의 인덱스 값
	std::string SName; // 서버로부터 받은 파일의 이름

	/* Agent가 가진 정책 파일의 정보*/
	std::string APolicyName;
	std::string APolicyPath;
	std::string APCreateTime; // first creation time
	std::string APModifyTime; // lastest modified time
	std::string APChageTime; // lastetst execute time

	policy_Info(void){}
	policy_Info(std::string _apolicyname, std::string _appath, std::string _apcreatetime, std::string _apmodifytime, std::string _apchangetime)
		: APolicyName(_apolicyname), APolicyPath(_appath), APCreateTime(_apcreatetime), APModifyTime(_apmodifytime), APChageTime(_apchangetime)
	{}
};


class CPolicy
{
private:
	std::string directoryPath;
	std::string m_sgetPname;
	std::string m_sgetPpath;
	std::string m_sgetPCreateTime;
	std::string m_sgetPModifyTime;
	std::string m_sgetPChangeTime;

	std::string m_sgetActivePolicies;
	std::string m_sgetInactivatePolicies;
	bool m_sdownload;
	bool m_sactive;
	bool m_sactiveFull;
	bool m_sinactivate;
	bool m_sinactivateFull;
	bool m_sisExist;
	bool m_sisActive;

	std::map<std::string, struct policy_Info*> PolicyList;

	CPolicy();
	~CPolicy();
	struct policy_Info* SearchPolicy(std::string key);
	std::string GeneratePolicyKey(ST_POLICY_INFO* policyServerInfo);

public:
	static CPolicy* GetInstance(void);

	std::string getPname();
	std::string getPpath(std::string PFilename);
	std::string getPCreateTime(std::string PFilePath);
	std::string getPModifyTime(std::string PFilePath);
	std::string getPChangeTime(std::string PFilePath);
	
	std::string getActivePolicies();
	std::string getInactivatePolicies();
	bool download(ST_POLICY_INFO* policyServerInfo);
	bool active(ST_POLICY_INFO* policyServerInfo);
	bool activeFull();
	bool Inactivate(ST_POLICY_INFO* policyServerInfo);
	bool InactivateFull();
	bool isActive(ST_POLICY_INFO* policyServerInfo);
	bool isExist(ST_POLICY_INFO* policyServerInfo);
	bool SuccessPolicy(ST_POLICY_INFO* policyServerInfo);
	void getPolicyInfo();
	void PolicyInfoPrint();
	bool getFileFromHttp(char* pszUrl, char* pszFile);
};

inline CPolicy* PolicyManager()
{
	return CPolicy::GetInstance();
}
