#include "stdafx.h"
#include "CTcpClient.h"
#include <thread>
#include "communicate.h"

void SendTest(CTcpClient* client) {
	while (true)
	{
		char message[1024];

		printf("Message : ");
		scanf("%s", message);

		client->Send(message);
	}
}

int main(int argc, char* argv[])
{
	CTcpClient client;
	std::vector<std::thread> works;

	client.Connect("127.0.0.1", "12345");

	works.push_back(std::thread(&CTcpClient::Recv, &client));
	works.push_back(std::thread(&SendTest, &client));

	works[0].join();
	works[1].join();

	client.Disconnet();
	return 0;
}