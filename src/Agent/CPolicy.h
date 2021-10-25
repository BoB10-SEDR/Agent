#pragma once
#include "stdafx.h"



struct policyInfo {
	/* �����κ��� ���� ��å ���� */
	std::string SPolicyId; // ������ DB�� ����� �� ���� ��å ������ �ε��� ��
	std::string SPolicyName; // �����κ��� ���� ������ �̸�

	/* Agent�� ���� ��å ������ ����*/
	std::string APolicyName;
	std::string APolicyPath;
	std::string APCreateTime; // first creation time
	std::string APModifyTime; // lastest modified time
	std::string APChageTime; // lastetst execute time

	policyInfo(void){}
	policyInfo(std::string _apolicyname, std::string _appath, std::string _apcreatetime, std::string _apmodifytime, std::string _apchangetime)
		: APolicyName(_apolicyname), APolicyPath(_appath), APCreateTime(_apcreatetime), APModifyTime(_apmodifytime), APChageTime(_apchangetime)
	{}
};


class CPolicy
{
private:
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

	std::vector<struct policyInfo> PolicyList;

public:
	std::string getPname();
	std::string getPpath(std::string PFilename);
	std::string getPCreateTime(std::string PFilePath);
	std::string getPModifyTime(std::string PFilePath);
	std::string getPChangeTime(std::string PFilePath);

	std::string getActivePolicies();
	std::string getInactivatePolicies();
	bool download(std::string sPid, std::string sPName);
	bool active(std::string sPid, std::string sPName);
	bool activeFull();
	bool Inactivate();
	bool InactivateFull();
	bool isActive();
	bool isExist();

	void getPolicyInfo();
	void PolicyInfoPrint();
};