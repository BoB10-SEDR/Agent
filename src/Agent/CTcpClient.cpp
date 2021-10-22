#include "CTcpClient.h"
#include "CMessage.h"

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
	while (1)
	{
		sleep(0);
		char message[BUFFER_SIZE];
		int messageLength;

		messageLength = read(clientSocket, &message, BUFFER_SIZE - 1);
		printf("len : %d \n", messageLength);

		if (messageLength <= 0)    // close request!
		{
			CTcpClient::Disconnet();
			printf("closed client\n");
			break;
		}
		message[messageLength] = 0;

		ST_PACKET_INFO* stPacketRead = new ST_PACKET_INFO();
		core::ReadJsonFromString(stPacketRead, message); //제대로 변환이 되지 않는 메세지들에 대한 에러 처리 필요
		MessageManager()->PushReceiveMessage(stPacketRead);
	}
}

int CTcpClient::Disconnet()
{
	close(clientSocket);
	clientSocket = -1;
}

int CTcpClient::Live()
{
	return clientSocket;
}

CTcpClient* CTcpClient::GetInstance()
{
	static CTcpClient instance;
	return &instance;
}