#pragma once

#include <cstdio>
#include <cppcore.h>
#include <protocol.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>
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