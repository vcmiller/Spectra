#pragma once

#include "jsoncpp/json.h"
#include <string>

namespace spectra {
	class Config;
	class ConfigArray {
		friend class Config;
	public:
		ConfigArray(const Json::Value &arr);
		ConfigArray();

		int getInt(int index, int def = 0);
		long long getLong(int index, long long def = 0);
		float getFloat(int index, float def = 0.0f);
		bool getBool(int index, float def = false);
		std::string getString(int index, std::string def = "");
		Config& getConfig(int index);

		void setInt(std::string key, int val);
		void setLong(std::string key, long long val);
		void setFloat(std::string key, float val);
		void setBool(std::string key, bool val);
		void setString(std::string key, std::string val);

		int getLength();
		void setLength(int length);

	private:
		Json::Value arr;
	};
}