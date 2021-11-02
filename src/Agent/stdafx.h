#pragma once
#include <cstdio>
#include <cppcore.h>
#include <protocol.h>
#include <memory>
#include <string>
#include <string.h>

#include "CLogger.h"

template<typename ... Args>
std::string StringFormatter(const std::string& format, Args ... args) {
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;

	if (size <= 0) {
		throw std::runtime_error("Error during formatting.");
	}

	std::unique_ptr<char[]> buf(new char[size]);

	snprintf(buf.get(), size, format.c_str(), args ...);

	return std::string(buf.get(), buf.get() + size - 1);
}

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include <pcap.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <dirent.h>


std::string SendToTerminal(const char* ShellCommand);

