#include "CTcpClient.h"

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

	if (clientSocket < 0)
		throw CTcpClientException("Socket Create Fail");

	if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
		throw CTcpClientException("connect Fail");

	else {
		printf("Connected...........\n");
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

		struct ST_PACKET_INFO stPacketRead;
		core::ReadJsonFromString(&stPacketRead, message);
		if (stPacketRead.destination == AGENT && stPacketRead.type == REQUEST)
		{
			tprintf(TEXT("RESQUEST OPCODE : %d\n"), stPacketRead.opcode);
			tprintf(TEXT("RESQUEST DATA : %s\n"), stPacketRead.data.c_str());
		}
		else if (stPacketRead.destination == AGENT && stPacketRead.type == RESPONSE)
		{
			tprintf(TEXT("RESPONSE OPCODE : %d\n"), stPacketRead.opcode);
			tprintf(TEXT("RESPONSE DATA : %s\n"), stPacketRead.data.c_str());
		}
		else
		{
			tprintf(TEXT("The message format is incorrect.The message format is incorrect. : %s\n"), message);
		}
	}
}

int CTcpClient::Disconnet()
{
	close(clientSocket);
}