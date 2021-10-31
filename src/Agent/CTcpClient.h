#pragma once
#include "stdafx.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 4096

class CTcpClient
{
private:
	int clientSocket;
	int connectStatus;
	std::string messageBuffers;
	struct sockaddr_in serverAddress;
	std::mutex connectionMutex;

	CTcpClient();
	CTcpClient(std::string ip, std::string port);
	~CTcpClient();
public:
	static CTcpClient* GetInstance();

	void Connect();
	bool Live();
	int Send(std::string message);
	int Recv();
	void Disconnet();
};

inline CTcpClient* ClientManager()
{
	return CTcpClient::GetInstance();
}