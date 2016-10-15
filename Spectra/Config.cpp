#include "Config.h"
#include "Log.h"

#include <fstream>
#include <exception>

namespace spectra {
	Config::Config(Json::Value *node) : nodePtr(node) { 
		filename = "";
		loaded = false;
	}

	Config::Config(std::string file) : nodePtr(&nodeVal) {
		filename = file;
		std::string text;
		std::ifstream strm(file);
		if (strm.is_open()) {
			strm.seekg(0, std::ios::end);
			text.reserve(strm.tellg());
			strm.seekg(0, std::ios::beg);
			text.assign((std::istreambuf_iterator<char>(strm)), std::istreambuf_iterator<char>());
			strm.close();
			Json::Reader reader;
			loaded = reader.parse(text, nodeVal);

			if (!loaded) {
				Log::log(reader.getFormattedErrorMessages());
			}
		} else {
			loaded = false;
			nodeVal = Json::objectValue;
		}
	}

	Config::Config() : nodePtr(&nodeVal) { 
		filename = "";
		loaded = false;
	}

	Config::Config(const Config& cfg) : nodePtr(cfg.nodePtr) {
		filename = "";
		loaded = false;
	}

	void Config::operator=(const Config & cfg) {
		filename = "";
		loaded = false;
	}

	Config Config::operator[](int index) {
		if (nodePtr->isArray()) {
			return Config(&(*nodePtr)[index]);
		} else {
			throw std::logic_error("Attempt to read index from non-array value.");
		}
	}

	Config Config::operator[](std::string key) {
		if (nodePtr->isObject()) {
			return Config(&(*nodePtr)[key]);
		} else {
			throw std::logic_error("Attempt to read key " + key + " from non-object value.");
		}
	}

	int Config::length() {
		if (nodePtr->isArray()) {
			return nodePtr->size();
		} else {
			throw std::logic_error("Attempt to read length from non-array value.");
		}
	}

	int Config::intValue() {
		if (nodePtr->isInt()) {
			return nodePtr->asInt();
		} else {
			throw std::logic_error("Attempt to read int from non-int value.");
		}
	}

	long long Config::int64Value() {
		if (nodePtr->isInt64()) {
			return nodePtr->asInt64();
		} else {
			throw std::logic_error("Attempt to read int64 from non-int64 value.");
		}
	}

	float Config::floatValue() {
		if (nodePtr->isDouble()) {
			return nodePtr->asDouble();
		} else {
			throw std::runtime_error("Attempt to read float from non-float value.");
		}
	}

	bool Config::boolValue() {
		if (nodePtr->isBool()) {
			return nodePtr->asBool();
		} else {
			throw std::logic_error("Attempt to read bool from non-bool value.");
		}
	}

	std::string Config::stringValue() {
		if (nodePtr->isString()) {
			return nodePtr->asString();
		} else {
			throw std::logic_error("Attempt to read string from non-string value.");
		}
	}

	void Config::operator=(int value) {
		*nodePtr = value;
	}

	void Config::operator=(long long value) {
		*nodePtr = value;
	}

	void Config::operator=(float value) {
		*nodePtr = value;
	}

	void Config::operator=(bool value) {
		*nodePtr = value;
	}

	void Config::operator=(std::string value) {
		*nodePtr = value;
	}

	void Config::setArray() {
		*nodePtr = Json::arrayValue;
	}

	void Config::setObject() {
		*nodePtr = Json::objectValue;
	}

	bool Config::isLoaded() {
		return loaded;
	}

	void Config::write(std::string filename) {
		this->filename = filename;
		write();
	}

	void Config::write() {
		if (filename != "") {
			Json::StyledStreamWriter writer;

			std::ofstream strm(filename);
			writer.write(strm, *nodePtr);

			strm.close();
		} else {
			throw std::runtime_error("Not loaded from file, use write(string) instead.");
		}
	}
}