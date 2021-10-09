#include "CTcpClient.h"
#include <iostream>

CTcpClient::CTcpClient()
{

}

CTcpClient::~CTcpClient()
{

}

int CTcpClient::Connect(std::string ip, std::string port)
{
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
	serverAddress.sin_port = htons(atoi(port.c_str()));

	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	//int flags = fcntl(clientSocket, F_GETFL);
	//flags |= O_NONBLOCK;

	//if (fcntl(clientSocket, F_SETFL, flags) < 0)
	//	printf("error\n");

	if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
		printf("connect() error!");
		return -1;
	}
	else {
		puts("Connected...........");
		return 0;
	}
}

int CTcpClient::Send(std::string message)
{
	write(clientSocket, message.c_str(), message.length());
}

int CTcpClient::Recv()
{
	char message[BUFFER_SIZE];
	int messageLength;

	while (1)
	{
		messageLength = read(clientSocket, &message, BUFFER_SIZE - 1);
		printf("len : %d \n", messageLength);

		if (messageLength <= 0)    // close request!
		{
			CTcpClient::Disconnet();
			printf("closed client\n");
			break;
		}
		message[messageLength] = 0;
		printf("Message from server: %s\n", message);
	}
}

int CTcpClient::Disconnet()
{
	close(clientSocket);
}