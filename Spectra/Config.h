#pragma once

#include "jsoncpp/json.h"
#include <string>
#include "ConfigValue.h"
#include "Map.h"

namespace spectra {
	class ConfigArray;
	class Config {
		friend struct internal::ConfigValue;
	public:
		Config(std::string file);
		Config();
		~Config();

		int getInt(std::string key, int def = 0);
		long long getLong(std::string key, long long def = 0);
		float getFloat(std::string key, float def = 0.0f);
		bool getBool(std::string key, bool def = false);
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

		Map<std::string, internal::ConfigValue> values;

		bool loaded;

		std::string filename;

		void read(const Json::Value &node);
		void write(Json::Value &node);
	};
}