#include "Log.h"

extern App app;

bool Log::print(std::string txt) {

	try {
		std::ofstream file(filename.c_str(), std::ios::app);

		if ( !file)
			throw Error("Unable to open logfile");

		file << txt << std::endl;

		file.close();
	}

	catch ( Error& err ) {
		std::cerr << err.getDesc();
		app.setQuit(true);
		return false;
	}
	catch ( ... ) {
		std::cerr << "Unexpected exception";
		app.setQuit(true);
		return false;
	}

	return true;
}

bool Log::setFilename(std::string fn) {

	filename = fn;

	try {
		std::ofstream file(filename.c_str());

		if ( !file)
			throw Error("Unable to open logfile");

		file << WNDTITLE << std::endl;

		file.close();

	}
	catch ( Error& err ) {
		std::cerr << err.getDesc();
		app.setQuit(true);
		return false;
	}
	catch ( ... ) {
		std::cerr << "Unexpected exception";
		app.setQuit(true);
		return false;
	}

	return true;
}

Log::Log()
{
}

Log::~Log()
{
}
