#include "CSample.h"
#include "CMessage.h"

CSample::CSample()
{
	text = "orignal";
}

CSample::~CSample()
{

}
// Text ���� �ʱ�ȭ
void CSample::Event1()
{
	text = "reset";
	MessageManager()->PushSendMessage(RESPONSE, OPCODE1, "complete");
}
// Text ���� Ȯ��
void CSample::Event2()
{
	MessageManager()->PushSendMessage(RESPONSE, OPCODE2, text);
}
// Text ���� ����� ����
void CSample::Event3(std::string text_)
{
	text = text_;
}

