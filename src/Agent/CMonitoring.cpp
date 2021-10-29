#include "CMonitoring.h"
#include "Function.h"

CMonitoring::CMonitoring()
{
	LoggerManager()->Info("Monintoring Init");
	fd = inotify_init();
	terminate = false;
}
CMonitoring::~CMonitoring()
{
	LoggerManager()->Info("Monintoring End");
	(void)close(fd);
}

std::string CMonitoring::GetDirectoryPath(std::string logPath)
{
	std::filesystem::path path(logPath);

	if (std::filesystem::exists(path.parent_path()))
		return path.parent_path();
	else
		return "";
}

std::string CMonitoring::GetFilename(std::string logPath)
{
	std::filesystem::path path(logPath);

	if (std::filesystem::exists(path))
		return path.filename();
	else
		return "";
}

std::string CMonitoring::ColumnSplit(std::string s, std::string divid)
{
	char* c = strtok((char*)s.c_str(), divid.c_str());
	c = strtok(NULL, divid.c_str());
	return c;
}

std::string CMonitoring::Trim(const std::string& s)
{
	if (s.length() == 0)
		return s;

	std::size_t beg = s.find_first_not_of(" \a\b\f\n\r\t\v");
	std::size_t end = s.find_last_not_of(" \a\b\f\n\r\t\v");
	if (beg == std::string::npos) // No non-spaces
		return "";

	return std::string(s, beg, end - beg + 1);
}

int CMonitoring::FileEndPosition(std::ifstream& fileFd) {
	if (fileFd.is_open()) {
		fileFd.seekg(0, std::ios::end);
		int size = fileFd.tellg();
		return size;
	}
	else {
		LoggerManager()->Warn("File Not Open");
		return -1;
	}
}

int CMonitoring::AddMonitoringTarget(std::string logPath)
{
	LoggerManager()->Info("AddMonitoringTarget");
	sleep(0);
	std::lock_guard<std::mutex> lock_guard(monitoringMutex);

	std::string originalPath = std::filesystem::canonical(logPath);

	std::string directoryPath = GetDirectoryPath(originalPath);
	std::string fileName = GetFilename(originalPath);
	
	if (directoryPath == "" || fileName == "") 
	{
		LoggerManager()->Warn(StringFormatter("Path is not exists : %s", originalPath.c_str()));
		return -1;
	}

	int wd = wdCountLists.count(directoryPath) ? wdCountLists[directoryPath] : -1;
	ST_MONITORING_EVENT* monitoringEvent = monitoringLists.count(originalPath) ? monitoringLists[originalPath] : NULL;

	if (wd != -1 && monitoringEvent == NULL)
	{
		wdCountLists[directoryPath]++;
	}

	if (wd == -1)
	{
		wd = inotify_add_watch(fd, directoryPath.c_str(), IN_MODIFY | IN_MOVE);
		if (wd == -1)
		{
			LoggerManager()->Error("Watcher Descriptor Error");
			return -1;
		}
		wdCountLists.insert(std::pair<std::string, int>(directoryPath, 1));
		wdKeyPathLists.insert(std::pair<int, std::string>(wd, directoryPath));
	}

	if (monitoringEvent == NULL)
	{
		ST_MONITORING_EVENT* monitoringEvent = new ST_MONITORING_EVENT();
		monitoringEvent->orignalPath = originalPath;
		monitoringEvent->directoryPath = directoryPath;
		monitoringEvent->fileName = fileName;
		monitoringEvent->fd = std::ifstream(originalPath);
		monitoringEvent->wd = wd;
		monitoringEvent->size = FileEndPosition(monitoringEvent->fd);

		monitoringLists.insert(std::pair<std::string, struct ST_MONITORING_EVENT*>(originalPath, monitoringEvent));
	}

	return 0;
}
int CMonitoring::RemoveMonitoringTarget(std::string logPath)
{
	LoggerManager()->Info("RemoveMonitoringTarget");
	sleep(0);
	std::lock_guard<std::mutex> lock_guard(monitoringMutex);

	std::string originalPath = std::filesystem::canonical(logPath);

	std::string directoryPath = GetDirectoryPath(originalPath);
	std::string fileName = GetFilename(originalPath);

	if (directoryPath == "" || fileName == "")
	{
		LoggerManager()->Warn(StringFormatter("Path is not exists : %s", originalPath.c_str()));
		return -1;
	}

	ST_MONITORING_EVENT* monitoringEvent = monitoringLists.count(originalPath) ? monitoringLists[originalPath] : NULL;

	if (monitoringEvent != NULL)
	{
		monitoringEvent->fd.close();

		if (wdCountLists[monitoringEvent->directoryPath] > 1)
			wdCountLists[monitoringEvent->directoryPath]--;
		else if (wdCountLists[monitoringEvent->directoryPath] == 1)
		{
			(void)inotify_rm_watch(fd, monitoringEvent->wd);
			wdCountLists.erase(monitoringEvent->directoryPath);
			wdKeyPathLists.erase(monitoringEvent->wd);
		}

		free(monitoringEvent);
		monitoringLists.erase(originalPath);
	}

	return 0;
}

void CMonitoring::StartMonitoring()
{
	LoggerManager()->Info("Monintoring Start");

	char buffer[BUF_LEN];
	while (!terminate) {
		sleep(0);

		int length = read(fd, buffer, BUF_LEN);
		int i = 0;

		if (length < 0) {
			LoggerManager()->Warn("Monitoring Error!");
		}

		while (i < length) {
			struct inotify_event* event = (struct inotify_event*)&buffer[i];

			if (event->len) 
			{
				if (event->mask & IN_MOVE)
				{
					if (!(event->mask & IN_ISDIR))
					{
						// vi, nano editor가 저장과 동시 종료시에는 수정된 내용이 반영되지만
						// 파일을 열어놓은 상태에서 저장하고, 나중에 종료하면 연결이 끊어진다. (조치 필요)

						std::string directoryPath = wdKeyPathLists[event->wd];
						std::string fullPath = directoryPath + "/" + event->name;

						ST_MONITORING_EVENT* monitoringEvent = monitoringLists.count(fullPath) ? monitoringLists[fullPath] : NULL;

						if (monitoringEvent != NULL) {
							monitoringEvent->fd.close();
							monitoringEvent->fd = std::ifstream(monitoringEvent->orignalPath);
						}
					}
				}

				if (event->mask & IN_MODIFY) 
				{
					if (!(event->mask & IN_ISDIR)) 
					{
						std::string directoryPath = wdKeyPathLists[event->wd];
						std::string fullPath = directoryPath + "/" + event->name;
						std::string message;

						ST_MONITORING_EVENT* monitoringEvent = monitoringLists.count(fullPath) ? monitoringLists[fullPath] : NULL;

						if (monitoringEvent != NULL) {
							int re_size = monitoringEvent->size;
							monitoringEvent->fd.seekg(0, std::ios::end);

							int size = monitoringEvent->fd.tellg();
							message.resize(size - re_size);

							monitoringEvent->fd.seekg(re_size);
							monitoringEvent->size = size;
							monitoringEvent->fd.read(&message[0], size - re_size);
							LoggerManager()->Info(StringFormatter("File size [%d] : [%d]", re_size, size));
							LoggerManager()->Info(StringFormatter("File Modify [%s] : %s", fullPath.c_str(), message.c_str()));

							func::SendMonitoringInfo(fullPath, message);
						}
					}
				}
			}
			
			i += EVENT_SIZE + event->len;
		}
	}
}

void CMonitoring::EndMonitoring()
{
	LoggerManager()->Info("Monintoring Stop");
	terminate = true;
}

std::vector<ST_PROCESS_INFO> CMonitoring::GetProcessLists()
{
	LoggerManager()->Info("Start GetProcessLists");
	std::string path = "/proc";
	std::vector<ST_PROCESS_INFO> processLists;

	int i = 0;
	for (auto& p : std::filesystem::directory_iterator(path)) {
		if (strtol(p.path().filename().c_str(), NULL, 10) > 0) {
			ST_PROCESS_INFO pinfo;
			std::ifstream status(p.path().string() + "/status");
			std::string buffer;

			while (buffer.find("Name:") == std::string::npos)
				std::getline(status, buffer);
			pinfo.name = Trim(ColumnSplit(buffer, ":"));

			while (buffer.find("State:") == std::string::npos)
				std::getline(status, buffer);
			pinfo.state = Trim(ColumnSplit(buffer, ":"));

			while (buffer.find("Pid:") == std::string::npos)
				std::getline(status, buffer);
			pinfo.pid = strtol(Trim(ColumnSplit(buffer, ":")).c_str(), NULL, 10);

			while (buffer.find("PPid:") == std::string::npos)
				std::getline(status, buffer);
			pinfo.ppid = strtol(Trim(ColumnSplit(buffer, ":")).c_str(), NULL, 10);

			status.close();

			std::ifstream cmdLine(p.path().string() + "/cmdline");
			std::getline(cmdLine, buffer);
			pinfo.cmdline = Trim(buffer);
			cmdLine.close();

			std::ifstream timeInfo(p.path().string() + "/sched");

			while (buffer.find("se.exec_start") == std::string::npos)
				std::getline(timeInfo, buffer);
			timeInfo.close();

			time_t curr_time = strtol(Trim(ColumnSplit(buffer, ":")).c_str(), NULL, 10);
			pinfo.startTime = std::asctime(std::localtime(&curr_time));

			processLists.push_back(pinfo);
			break;
		}
	}
	LoggerManager()->Info(StringFormatter("count : %d", i));
	return processLists;
}

std::vector<ST_FD_INFO> CMonitoring::GetFdLists(std::string pid)
{
	LoggerManager()->Info("Start GetFdLists");
	std::string path = "/proc/" + pid + "/fd";
	std::vector<ST_FD_INFO> fdLists;

	int i = 0;
	for (auto& p : std::filesystem::directory_iterator(path)) {
		ST_FD_INFO pinfo;

		pinfo.pid = strtol(pid.c_str(), NULL, 10);
		pinfo.name = p.path().string();
		pinfo.path = std::filesystem::read_symlink(p).string();

		fdLists.push_back(pinfo);
	}
	LoggerManager()->Info(StringFormatter("count : %d", i));
	return fdLists;
}

CMonitoring* CMonitoring::GetInstance()
{
	static CMonitoring instance;
	return &instance;
}