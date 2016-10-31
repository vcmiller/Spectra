#include "Log.h"

namespace spectra {
	Log Log::instance;
	const Log &Log::log = instance;

	Log::Log() {
		flush = false;
		writeFile = true;
		writeConsole = true;

		file = std::ofstream("spectra.log");
		fileOpen = file.is_open();

		if (!fileOpen) {
			throw std::runtime_error("Fail to open log file for writing!");
		}
	}

	Log::~Log() { }

	Log::Log(const Log& l) {
		file.close();
	}

	void Log::operator=(const Log& l) {}

	void Log::setWriteMode(bool console, bool file) {
		instance.writeConsole = console;
		instance.writeFile = file;
	}

	void Log::setFlush(bool flush) {
		instance.flush = flush;
	}
}