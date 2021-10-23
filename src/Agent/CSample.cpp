#include "CSample.h"
#include "CMessage.h"

CSample::CSample()
{
	text = "orignal";
}

CSample::~CSample()
{

}
// Text 문자 초기화
void CSample::Event1()
{
	text = "reset";
	MessageManager()->PushSendMessage(RESPONSE, OPCODE1, "complete");
}
// Text 문자 확인
void CSample::Event2()
{
	MessageManager()->PushSendMessage(RESPONSE, OPCODE2, text);
}
// Text 문자 사용자 설정
void CSample::Event3(std::string text_)
{
	text = text_;
}

