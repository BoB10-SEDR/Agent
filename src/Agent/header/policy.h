#pragma once
#include "stdafx.h"


struct policyList {
	std::string Pname;
	std::string Ppath;
	std::string PCreateTime; // 대응 정책 파일 최초 생성 날짜:시간
	std::string PModifyTime; // 대응 정책 파일 최근 수정 날짜:시간
	std::string PChageTime; // 대응 정책 파일 최근 실행 날짜:시간

	policyList(void){}
	policyList(std::string _pname, std::string _ppath, std::string _pcreatetime, std::string _pmodifytime, std::string _pchangetime)
		: Pname(_pname), Ppath(_ppath), PCreateTime(_pcreatetime), PModifyTime(_pmodifytime), PChageTime(_pchangetime)
	{}
};


class CPolicy
{
private:
	bool m_download;
	bool m_active;
	bool m_activeFull;
	bool m_inactivate;
	bool m_inactivateFull;
	bool m_isExist;
	bool m_isActive;
	
	std::string m_sgetPname;
	std::string m_sgetPpath;
	std::string m_sgetPCreateTime;
	std::string m_sgetPModifyTime;
	std::string m_sgetPChangeTime;

	std::vector<struct policyList> ST_POLICY_INFO;

public:
	void CPolicyInit();

	std::string getActivePolicies();
	std::string getInactivatePolicies();
	bool download();
	bool active();
	bool activeFull();
	bool Inactivate();
	bool InactivateFull();
	bool isExist();
	bool isActive();

	void getPname();
	void getPpath(std::string PFilename);
	std::string getPCreateTime(std::string PFilePath);
	std::string getPModifyTime(std::string PFilePath);
	std::string getPChangeTime(std::string PFilePath);
	void getPolicyInfo();

};