#pragma once
#include "stdafx.h"

class CSample
{
private:
	std::string text;
public:
	CSample();
	~CSample();
	void Event1();
	void Event2();
	void Event3(std::string text_);
};


