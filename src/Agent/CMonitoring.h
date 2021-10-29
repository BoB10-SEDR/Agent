#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <map>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024*(EVENT_SIZE+16))

struct ST_MONITORING_EVENT
{
	std::string orignalPath;
	std::string directoryPath;
	std::string fileName;
	std::ifstream fd;
	int wd;
	int size;
};

class CMonitoring
{
private:
	int fd;
	std::map<std::string, struct ST_MONITORING_EVENT*> monitoringLists;
	std::map<std::string, int> wdCountLists;
	std::map<int, std::string> wdKeyPathLists;
	std::mutex monitoringMutex;
	bool terminate;

	CMonitoring();
	~CMonitoring();
	std::string GetDirectoryPath(std::string logPath);
	std::string GetFilename(std::string logPath);
	int FileEndPosition(std::ifstream& fileFd);
	std::string ColumnSplit(std::string s, std::string divid);
	std::string Trim(const std::string& s);
public:
	static CMonitoring* GetInstance(void);
	int AddMonitoringTarget(std::string logPath);
	int RemoveMonitoringTarget(std::string logPath);
	void StartMonitoring();
	void EndMonitoring();
	std::vector<ST_PROCESS_INFO> GetProcessLists();
	std::vector<ST_FD_INFO> GetFdLists(std::string pid);
};

inline CMonitoring* MonitoringManager()
{
	return CMonitoring::GetInstance();
}
