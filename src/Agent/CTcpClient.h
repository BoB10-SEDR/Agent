#pragma once
#include <stdio.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> 
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

class CTcpClient
{
private:
	int clientSocket;
public:
	CTcpClient();
	~CTcpClient();

	int Connect(std::string ip, std::string port);
	int Send(std::string message);
	int Recv();
	int Disconnet();
};

