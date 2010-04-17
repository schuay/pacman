#pragma once
#include "Main.h"

class Log
{
public:
	Log();
	~Log();

	bool setFilename(std::string fn);

	bool print(std::string txt);
private:
	std::string		filename;
};
