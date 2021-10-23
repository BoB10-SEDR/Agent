#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define BUFFER_SIZE 1024

class CTcpClient
{
private:
	int clientSocket;
	int connectStatus;
	struct sockaddr_in serverAddress;

	CTcpClient();
	CTcpClient(std::string ip, std::string port);
	~CTcpClient();
public:
	static CTcpClient* GetInstance(void);

	int Connect();
	int Reconnect();
	bool Live();
	int Send(std::string message);
	int Recv();
	int Disconnet();
};

inline CTcpClient* ClientManager()
{
	return CTcpClient::GetInstance();
}