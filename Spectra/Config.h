#pragma once

#include "jsoncpp/json.h"
#include <string>

namespace spectra {
	class Config {
	public:
		Config(std::string file);
		Config();
		Config(const Config& cfg);

		void operator=(const Config& cfg);

		Config operator[] (std::string key);

		Config operator[] (int index);

		int length();

		int intValue();
		long long int64Value();
		float floatValue();
		bool boolValue();
		std::string stringValue();

		void operator=(int);
		void operator=(long long);
		void operator=(float);
		void operator=(bool);
		void operator=(std::string);

		void setArray();
		void setObject();

		bool isLoaded();

		void write();
		void write(std::string filename);

	private:
		Json::Value nodeVal;
		Json::Value *nodePtr;
		bool loaded;

		std::string filename;

		Config(Json::Value *node);
	};
}