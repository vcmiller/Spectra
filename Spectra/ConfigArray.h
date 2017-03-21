#pragma once

#include "jsoncpp/json.h"
#include <string>
#include "ConfigValue.h"

namespace spectra {
	class Config;
	class ConfigArray {
		friend class internal::ConfigValue;
	public:
		ConfigArray();
		~ConfigArray();

		int getInt(int index, int def = 0);
		long long getLong(int index, long long def = 0);
		float getFloat(int index, float def = 0.0f);
		bool getBool(int index, float def = false);
		std::string getString(int index, std::string def = "");

		Config& getConfig(int index);
		ConfigArray& getArray(int index);

		void setInt(int index, int val);
		void setLong(int index, long long val);
		void setFloat(int index, float val);
		void setBool(int index, bool val);
		void setString(int index, std::string val);

		int getLength();
		void setLength(int length);
		void write(std::ostream &strm, int tab);

	private:
		std::vector<internal::ConfigValue> values;

		void read(const Json::Value &node);
		void write(Json::Value &node);
	};
}