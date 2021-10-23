#include "CMessage.h"
#include "CTcpClient.h"
#include "CSample.h"
#include "CLogger.h"

CMessage::CMessage()
{
}

CMessage::~CMessage()
{  
	ClientManager()->Disconnet();
}

void CMessage::Init() 
{
	LoggerManager()->Info("CMessage INIT");
	ClientManager()->Connect();

	std::future<void> c = std::async(std::launch::async, &CMessage::MatchReceiveMessage, this);
	std::future<void> b = std::async(std::launch::async, &CMessage::ReceiveMessage, this);
	std::future<void> a = std::async(std::launch::async, &CMessage::SendMessage, this);

	a.get();
	b.get();
	c.get();
}

void CMessage::SendMessage()
{
	LoggerManager()->Info("Start SendMessage\n");
	while (1) {
		sleep(0);
		std::lock_guard<std::mutex> lock_guard(sendMessagemutex);

		if (sendMessage.empty())
			continue;

		ST_PACKET_INFO* stPacketSend = sendMessage.front();
		sendMessage.pop();

		std::tstring jsPacketSend;
		core::WriteJsonToString(stPacketSend, jsPacketSend);
		ClientManager()->Send(jsPacketSend);
	}
}

void CMessage::ReceiveMessage()
{
	LoggerManager()->Info("Start ReceiveMessage\n");
	while(1){
		sleep(0);
		ClientManager()->Recv();
	}
}

void CMessage::PushSendMessage(PacketType type, PacketOpcode opcode, std::string message)
{
	sleep(0);
	std::lock_guard<std::mutex> lock_guard(sendMessagemutex);
	ST_PACKET_INFO* stPacketSend = new ST_PACKET_INFO(AGENT, SERVER, type, opcode, message);
	sendMessage.push(stPacketSend);
}

void CMessage::PushReceiveMessage(ST_PACKET_INFO* stPacketInfo)
{
	sleep(0);
	std::lock_guard<std::mutex> lock_guard(receiveMessagemutex);
	receiveMessage.push(stPacketInfo);
}

void CMessage::MatchReceiveMessage()
{
	LoggerManager()->Info("Start MatchReceiveMessage\n");
	std::future<void> result;
	CSample* sample = new CSample();

	while(1)
	{
		sleep(0);
		std::lock_guard<std::mutex> lock_guard(receiveMessagemutex);

		if (receiveMessage.empty())
			continue;

		ST_PACKET_INFO* stPacketSend = receiveMessage.front();
		receiveMessage.pop();
		
		switch (stPacketSend->opcode) {
		case OPCODE1:
			result = std::async(std::launch::async, &CSample::Event1, sample);
			break;
		case OPCODE2:
			result = std::async(std::launch::async, &CSample::Event2, sample);
			break;
		case OPCODE3:
			result = std::async(std::launch::async, &CSample::Event3, sample, stPacketSend->data.c_str());
			break;
		default:
			LoggerManager()->Error(stPacketSend->data.c_str());
			break;
		}
	}
}

CMessage* CMessage::GetInstance()
{
	static CMessage instance;
	return &instance;
}