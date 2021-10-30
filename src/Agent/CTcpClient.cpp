#include "CTcpClient.h"
#include "CTcpClientException.h"
#include "CMessage.h"

CTcpClient::CTcpClient()
{
	connectStatus = -1;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(atoi("12345"));
}

CTcpClient::CTcpClient(std::string ip, std::string port)
{
	connectStatus = -1;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
	serverAddress.sin_port = htons(atoi(port.c_str()));
}

CTcpClient::~CTcpClient()
{

}

int CTcpClient::Connect()
{
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (clientSocket < 0)
		throw CTcpClientException("Socket Create Fail");

	connectStatus = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	if (connectStatus == -1)
		LoggerManager()->Warn("Connect Fail");
	else
		LoggerManager()->Info("Connected...........\n");

	return 0;
}

int CTcpClient::Reconnect()
{
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (clientSocket < 0)
		throw CTcpClientException("Socket Create Fail");

	while (!Live()) {
		sleep(5);

		connectStatus = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));


		if (connectStatus == -1) {
			LoggerManager()->Warn("Reconnect Fail");
		}
		else {
			LoggerManager()->Info("Reconnected...........\n");
			break;
		}
	}

	return 0;
}

int CTcpClient::Send(std::string message)
{
	if (!Live()) {
		Reconnect();
	}
	int result = write(clientSocket, message.c_str(), message.length());
	LoggerManager()->Info(StringFormatter("Send Complete [%d]", result));
	return 0;
}

int CTcpClient::Recv()
{
	if (!Live()) {
		Reconnect();
	}

	int messageLength;
	char message[BUFFER_SIZE + 1];

	while (1) {
		messageLength = read(clientSocket, &message, BUFFER_SIZE);
		LoggerManager()->Info(StringFormatter("Read Complete [%d]", messageLength));

		if (messageLength == 0)
		{
			CTcpClient::Disconnet();
			break;
		}
		else if (messageLength < 0)
		{
			LoggerManager()->Error(StringFormatter("Error Code [%d]", errno));
			LoggerManager()->Error(message);
			break;
		}
		else
		{
			message[messageLength] = 0;
			MessageBuffers += message;

			while (1)
			{
				size_t location = MessageBuffers.find("END");
				printf("find End : %d\n", location);

				if (location == -1)
					break;

				LoggerManager()->Info(StringFormatter("\nMessage Length : %d", messageLength));
				ST_PACKET_INFO* stPacketRead = new ST_PACKET_INFO();
				core::ReadJsonFromString(stPacketRead, MessageBuffers.substr(0, location));

				MessageManager()->PushReceiveMessage(stPacketRead);
				MessageBuffers = MessageBuffers.substr(location + 3);
			}
		}
		message[0] = 0;
	}
	return 0;
}

int CTcpClient::Disconnet()
{
	close(clientSocket);
	LoggerManager()->Info("Disconneted...........\n");
	clientSocket = -1;
	connectStatus = -1;

	return 0;
}

bool CTcpClient::Live()
{
	if (connectStatus == -1) {
		return false;
	}
	return true;
}

CTcpClient* CTcpClient::GetInstance()
{
	static CTcpClient instance("14.138.200.178", "55555");
	return &instance;
}