#include "stdafx.h"
#include "CMessage.h"

#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
	try
	{
		std::future<void> a = std::async(std::launch::async, &CMessage::Init, MessageManager());
		while (1)
		{
			char msg[BUFFER_SIZE];

			printf("Message : ");
			fgets(msg, BUFFER_SIZE, stdin);
		}
	}
	catch (std::exception& e)
	{
		printf("%s\n", e.what());
	}
	return 0;
}