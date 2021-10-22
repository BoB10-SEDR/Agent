#include "stdafx.h"
#include "CMessage.h"

#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
	LoggerManager()->Info("Start Agent Program!");
	try
	{
		std::future<void> a = std::async(std::launch::async, &CMessage::Init, MessageManager());
	}
	catch (std::exception& e)
	{
		LoggerManager()->Error(e.what());
	}
	LoggerManager()->Info("Terminate Agent Program!");
	return 0;
}