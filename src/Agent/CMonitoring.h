#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
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
	std::tstring processName;
	std::tstring orignalPath;
	std::tstring directoryPath;
	std::tstring fileName;
	std::ifstream fd;
	int wd;
	int size;
};

class CMonitoring
{
private:
	int fd;
	std::map<std::tstring, struct ST_MONITORING_EVENT*> monitoringLists;
	std::map<std::tstring, int> wdCountLists;
	std::map<int, std::tstring> wdKeyPathLists;
	std::mutex monitoringMutex;
	bool terminate;

	CMonitoring();
	~CMonitoring();
	std::tstring GetDirectoryPath(std::tstring logPath);
	std::tstring GetFilename(std::tstring logPath);
	int FindFileEndPosition(std::ifstream& fileFd);
	std::tstring ColumnSplit(std::tstring s, std::tstring divid);
	std::tstring Trim(const std::tstring& s);
public:
	static CMonitoring* GetInstance(void);
	int AddMonitoringTarget(ST_MONITOR_TARGET target);
	int RemoveMonitoringTarget(ST_MONITOR_TARGET target);
	void StartMonitoring();
	void EndMonitoring();
	std::vector<ST_PROCESS_INFO> GetProcessLists();
	std::vector<ST_FD_INFO> GetFdLists(std::tstring pid);
};

inline CMonitoring* MonitoringManager()
{
	return CMonitoring::GetInstance();
}
