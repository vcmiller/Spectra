#pragma once

#include "jsoncpp/json.h"
#include <string>
#include "ConfigArray.h"

namespace spectra {
	class Config {
	public:
		Config(std::string file);
		Config();
		~Config();

		int getInt(std::string key, int def = 0);
		long long getLong(std::string key, long long def = 0);
		float getFloat(std::string key, float def = 0.0f);
		bool getBool(std::string key, float def = false);
		std::string getString(std::string key, std::string def = "");

		Config& getConfig(std::string key);
		ConfigArray& getArray(std::string key);

		void setInt(std::string key, int val);
		void setLong(std::string key, long long val);
		void setFloat(std::string key, float val);
		void setBool(std::string key, bool val);
		void setString(std::string key, std::string val);

		

		bool isLoaded();

		void write();

	private:
		std::map<std::string, int> ints;
		std::map<std::string, long long> longs;
		std::map<std::string, float> floats;
		std::map<std::string, bool> bools;
		std::map<std::string, std::string> strings;
		std::map<std::string, Config*> configs;
		std::map<std::string, ConfigArray*> arrays;

		bool loaded;
		Config* parent;

		std::string filename;

		template <typename T> void writeMap(std::map<std::string, T> &map, Json::Value &node) {
			for (auto pair : map) {
				node[pair.first] = pair.second;
			}
		}

		void read(const Json::Value &node);
		void write(Json::Value &node);
	};
}