#pragma once

#include "jsoncpp/json.h"
#include <string>

namespace spectra {
	class Config {
	public:
		Config(std::string file);
		Config();
		Config(Config& cfg);

		void operator=(const Config& cfg);

		Config operator[] (std::string key);

		Config operator[] (int index);

		int length();

		operator int();
		operator long long();
		operator float();
		operator bool();
		operator std::string();

		void operator=(int);
		void operator=(long long);
		void operator=(float);
		void operator=(bool);
		void operator=(std::string);

		bool isLoaded();

	private:
		Json::Value dummyNode;
		Json::Value &node;
		Json::Reader reader;
		bool loaded;

		Config(Json::Value &node);
	};
}