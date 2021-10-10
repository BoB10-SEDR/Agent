#include "stdafx.h"
#include "CTcpClient.h"
#include <thread>

void SendTest(CTcpClient* client) {
	while (true)
	{
		char message[BUFFER_SIZE];

		printf("Message : ");
		fgets(message, BUFFER_SIZE, stdin);

		ST_PACKET_INFO stPacketSend(AGENT, SERVER, REQUEST, OPCODE1, message);

		std::tstring jsPacketSend;
		core::WriteJsonToString(&stPacketSend, jsPacketSend);

		client->Send(jsPacketSend);
	}
}

int main(int argc, char* argv[])
{
	CTcpClient client;
	std::vector<std::thread> works;

	try
	{
		client.Connect("127.0.0.1", "12345");

		works.push_back(std::thread(&CTcpClient::Recv, &client));
		works.push_back(std::thread(&SendTest, &client));

		works[0].join();
		works[1].join();

		client.Disconnet();
	}
	catch (std::exception& e)
	{
		printf("%s\n", e.what());
	}
	return 0;
}