#pragma once
#include "stdafx.h"
#include <queue>
#include <protocol.h>
#include <mutex>
#include <future>

class CMessage
{
private:
	std::queue<ST_PACKET_INFO*> receiveMessage;
	std::queue<ST_PACKET_INFO*> sendMessage;
	std::mutex receiveMessageMutex;
	std::mutex sendMessageMutex;

	CMessage();
	~CMessage();

	void ReceiveMessage();	//서버에서 받은 메시지를 수신하는 기능
	void SendMessage();		//서버로 메시지를 보내는 기능
	void MatchReceiveMessage();	//수신 메시지큐에서 메시지를 읽어, 에이전트 특정 기능과 매칭 시켜주는 기능
	
public:
	static CMessage* GetInstance(void);
	void Init();
	void PushReceiveMessage(ST_PACKET_INFO* stPacketInfo);	//수신 메시지를 수신 메시지큐에 저장하는 기능
	void PushSendMessage(PacketType type, PacketOpcode opcode, std::tstring message);	//송신 메시지를 송신 메시지큐에 저장하는 기능
};

inline CMessage* MessageManager()
{
	return CMessage::GetInstance();
}