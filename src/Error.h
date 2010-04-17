#pragma once
#include "Main.h"

class Error
{
public:
	Error(std::string str);
	~Error();

	std::string getDesc() { return desc; }
private:
	std::string			desc;
};
