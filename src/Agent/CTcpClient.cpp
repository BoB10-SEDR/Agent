#include "CTcpClient.h"
#include "CMessage.h"
#include "Function.h"

extern ST_ENV env;

CTcpClient::CTcpClient()
{
	connectStatus = -1;
	clientSocket = -1;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(atoi("12345"));
}

CTcpClient::CTcpClient(std::string ip, std::string port)
{
	connectStatus = -1;
	clientSocket = -1;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
	serverAddress.sin_port = htons(atoi(port.c_str()));
}

CTcpClient::~CTcpClient()
{

}

void CTcpClient::Connect()
{
	std::lock_guard<std::mutex> lock_guard(connectionMutex);
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (clientSocket < 0)
		core::Log_Warn(TEXT("CTcpClient.cpp - [%s] : %d"), TEXT("Socket Create Fail"), errno);

	connectStatus = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	if (connectStatus == -1)
		core::Log_Warn(TEXT("CTcpClient.cpp - [%s] : %s"), TEXT("Server Connected Fail"), TEXT(inet_ntoa(serverAddress.sin_addr)));
	else {
		core::Log_Info(TEXT("CTcpClient.cpp - [%s] : %s"), TEXT("Server Connected"), TEXT(inet_ntoa(serverAddress.sin_addr)));
		func::GetDeviceInfo();
	}
}

int CTcpClient::Send(std::string message)
{
	while (!Live()) {
		sleep(5);
		Connect();
	}

	int result = write(clientSocket, message.c_str(), message.length());

	if (result == -1)
		core::Log_Warn(TEXT("CTcpClient.cpp - [%s] : %d"), TEXT("Send Error Code"), errno);
	else
		core::Log_Debug(TEXT("CTcpClient.cpp - [%s] : %d"), TEXT("Send Complete"), result);

	return 0;
}

int CTcpClient::Recv()
{
	while (!Live()) {
		sleep(5);
		Connect();
	}

	int messageLength;
	char message[BUFFER_SIZE + 1];

	while (1) {
		messageLength = read(clientSocket, &message, BUFFER_SIZE);
		core::Log_Debug(TEXT("CTcpClient.cpp - [%s] : %d"), TEXT("Read Complete"), messageLength);
		core::Log_Debug(TEXT("CTcpClient.cpp - [%s] : %s"), TEXT("Recieve Message"), message);

		if (messageLength == 0)
		{
			CTcpClient::Disconnet();
			break;
		}
		else if (messageLength < 0)
		{
			core::Log_Warn(TEXT("CTcpClient.cpp - [%s] : %d"), TEXT("Read Error Code"), errno);
			core::Log_Debug(TEXT("CTcpClient.cpp - [%s] : %s"), TEXT("Remain MessageBuffer"), TEXT(messageBuffers.c_str()));
			break;
		}
		else
		{
			message[messageLength] = 0;
			messageBuffers += message;

			while (1)
			{
				size_t start_location = messageBuffers.find("BOBSTART");
				size_t end_location = messageBuffers.find("BOBEND");
				core::Log_Debug(TEXT("CTcpClient.cpp - [%s] : %d, %d"), TEXT("Find Position(Start, End)"), start_location, end_location);

				if (start_location == -1 || end_location == -1)
					break;

				ST_PACKET_INFO* stPacketRead = new ST_PACKET_INFO();
				core::ReadJsonFromString(stPacketRead, messageBuffers.substr(start_location + 8, end_location - (start_location + 8)));

				MessageManager()->PushReceiveMessage(stPacketRead);
				messageBuffers = messageBuffers.substr(start_location + 6);
			}
		}
		memset(message, 0, sizeof(message));
	}
	return 0;
}

void CTcpClient::Disconnet()
{
	close(clientSocket);
	clientSocket = -1;
	connectStatus = -1;
	core::Log_Warn(TEXT("CTcpClient.cpp - [%s] : %s"), TEXT("Server Disconneted"), TEXT(inet_ntoa(serverAddress.sin_addr)));
}

bool CTcpClient::Live()
{
	if (connectStatus == -1 || clientSocket == -1) 
		return false;
	
	return true;
}

CTcpClient* CTcpClient::GetInstance()
{
	static CTcpClient instance(env.ip, env.port);
	return &instance;
}