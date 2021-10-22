#pragma once
#include <stdio.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "stdafx.h"
#include "TcpClientException.h"

#define BUFFER_SIZE 1024

class CTcpClient
{
private:
	int clientSocket;

	CTcpClient();
	~CTcpClient();
public:
	static CTcpClient* GetInstance(void);

	int Connect(std::string ip, std::string port);
	int Live();
	int Send(std::string message);
	int Recv();
	int Disconnet();
};

inline CTcpClient* ClientManager()
{
	return CTcpClient::GetInstance();
}