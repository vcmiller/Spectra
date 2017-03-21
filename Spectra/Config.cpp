#include "Config.h"
#include "Log.h"
#include "ConfigArray.h"

#include <fstream>
#include <exception>

namespace spectra {

	Config::Config(std::string file) {
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

			Json::Value node;

			loaded = reader.parse(text, node);

			if (loaded) {
				read(node);
			} else {
				Log::log("Failed to load config!");
			}
		} else {
			loaded = false;
		}
	}

	Config::Config() { 
		filename = "";
		loaded = false;
	}

	Config::~Config() {
		for (auto v : values) {
			if (v.value.type == internal::ConfigValueType::t_config) {
				delete v.value.value.configVal;
			} else if (v.value.type == internal::ConfigValueType::t_array) {
				delete v.value.value.arrayVal;
			}
		}
	}

	int Config::getInt(std::string key, int def) {
		if (!values.contains(key)) {
			values[key].set(def);
		}

		return values[key].value.intVal;
	}

	long long Config::getLong(std::string key, long long def) {
		if (!values.contains(key)) {
			values[key].set(def);
		}

		return values[key].value.longVal;
	}

	float Config::getFloat(std::string key, float def) {
		if (!values.contains(key)) {
			values[key].set(def);
		}

		return values[key].value.floatVal;
	}

	bool Config::getBool(std::string key, bool def) {
		if(!values.contains(key)) {
			values[key].set(def);
		}

		return values[key].value.boolVal;
	}

	std::string Config::getString(std::string key, std::string def) {
		if (!values.contains(key)) {
			values[key].set(def);
		}

		return values[key].value.stringVal;
	}

	Config & Config::getConfig(std::string key) {
		if (!values.contains(key)) {
			Config *cfg = new Config();
			values[key].type = internal::ConfigValueType::t_config;
			values[key].value.configVal = cfg;
		}

		return *values[key].value.configVal;
	}

	ConfigArray & Config::getArray(std::string key) {
		if (!values.contains(key)) {
			ConfigArray *arr = new ConfigArray();
			values[key].type = internal::ConfigValueType::t_array;
			values[key].value.arrayVal = arr;
		}

		return *values[key].value.arrayVal;
	}

	void Config::setInt(std::string key, int val) {
		values[key].set(val);
	}

	void Config::setLong(std::string key, long long val) {
		values[key].set(val);
	}

	void Config::setFloat(std::string key, float val) {
		values[key].set(val);
	}

	void Config::setBool(std::string key, bool val) {
		values[key].set(val);
	}

	void Config::setString(std::string key, std::string val) {
		values[key].set(val);
	}

	void Config::read(const Json::Value &node) {
		
		for (std::string key : node.getMemberNames()) {
			Json::Value val = node[key];

			values[key].read(val);
		}
	}

	void Config::write(Json::Value &node) {
		node = Json::objectValue;

		for (auto v : values) {
			Json::Value &val = node[v.key];

			v.value.write(val);
		}
	}

	bool Config::isLoaded() {
		return loaded;
	}

	void Config::write() {
		if (filename != "") {
			std::ofstream strm(filename);
			write(strm, 0);

			strm.close();
		} else {
			throw std::runtime_error("Not loaded from file.");
		}
	}

	void Config::write(std::ostream & strm, int tab) {
		std::string tabstr = "";

		for (int i = 0; i < tab; i++) {
			tabstr += "\t";
		}

		strm << "{\n";

		int pos = 0;
		for (auto val : values) {
			if (val.key != "") {
				strm << tabstr << "\t\"" << val.key << "\" : ";

				if (val.value.type == internal::ConfigValueType::t_array || val.value.type == internal::ConfigValueType::t_config) {
					//strm << "\n" << tabstr << "\t";
				}

				val.value.write(strm, tab + 1);

				if (pos < values.size() - 1) {
					strm << ",";
				}

				strm << "\n";
			}

			pos++;
		}
		strm << tabstr << "}";
	}
}