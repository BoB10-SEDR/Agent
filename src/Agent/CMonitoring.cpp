#include "CMonitoring.h"

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
		wd = inotify_add_watch(fd, directoryPath.c_str(), IN_MODIFY);
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
							
							LoggerManager()->Info(StringFormatter("File Modify [%s] : %s", fullPath.c_str(), message.c_str()));
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

CMonitoring* CMonitoring::GetInstance()
{
	static CMonitoring instance;
	return &instance;
}