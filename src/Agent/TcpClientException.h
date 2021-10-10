#pragma once
#include "stdafx.h"
#include <string>
#include <exception>

class CTcpClientException : public std::exception
{
private:
	std::string message;
public:
	CTcpClientException(std::string _message) : message(_message) {};
	virtual const char* what() const throw() { return message.c_str(); };
};

