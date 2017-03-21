#include "ConfigValue.h"
#include "jsoncpp\json.h"
#include "Config.h"
#include "ConfigArray.h"
#include <cstring>

namespace spectra {
	namespace internal {

		void ConfigValue::read(const Json::Value& val) {

			if (val.isInt()) {
				value.intVal = val.asInt();
				type = ConfigValueType::t_int;
			} else if (val.isInt64()) {
				value.longVal = val.asInt64();
				type = ConfigValueType::t_long;
			} else if (val.isDouble()) {
				value.floatVal = val.asFloat();
				type = ConfigValueType::t_float;
			} else if (val.isBool()) {
				value.boolVal = val.asBool();
				type = ConfigValueType::t_bool;
			} else if (val.isString()) {
				value.stringVal = strdup(val.asCString());
				type = ConfigValueType::t_string;
			} else if (val.isObject()) {
				value.configVal = new Config();
				type = ConfigValueType::t_config;
				value.configVal->loaded = true;
				value.configVal->read(val);
			} else if (val.isArray()) {
				value.arrayVal = new ConfigArray();
				value.arrayVal->read(val);
				type = ConfigValueType::t_array;
			}
		}

		void ConfigValue::write(Json::Value & node) {
			if (type == ConfigValueType::t_int) {
				node = value.intVal;
			} else if (type == ConfigValueType::t_long) {
				node = value.longVal;
			} else if (type == ConfigValueType::t_float) {
				node = value.floatVal;
			} else if (type == ConfigValueType::t_bool) {
				node = value.boolVal;
			} else if (type == ConfigValueType::t_string) {
				node = value.stringVal;
			} else if (type == ConfigValueType::t_config) {
				node = Json::Value();
				value.configVal->write(node);
			} else if (type == ConfigValueType::t_array) {
				node = Json::Value();
				value.arrayVal->write(node);
			}
		}

		void ConfigValue::set(int val) {
			value.intVal = val;
			type = ConfigValueType::t_int;
		}

		void ConfigValue::set(long long val) {
			value.longVal = val;
			type = ConfigValueType::t_long;
		}

		void ConfigValue::set(float val) {
			value.floatVal = val;
			type = ConfigValueType::t_float;
		}

		void ConfigValue::set(bool val) {
			value.boolVal = val;
			type = ConfigValueType::t_bool;
		}

		void ConfigValue::set(std::string val) {
			value.stringVal = strdup(val.c_str());
			type = ConfigValueType::t_string;
		}

		void ConfigValue::set(Config * val) {
			value.configVal = val;
			type = ConfigValueType::t_config;
		}

		void ConfigValue::set(ConfigArray * val) {
			value.arrayVal = val;
			type = ConfigValueType::t_array;
		}

		ConfigValue::ConfigValue() {
			type = ConfigValueType::t_none;
		}

		ConfigValue::ConfigValue(const ConfigValue & v) {
			type = v.type;
			std::memcpy(&value, &v.value, sizeof(ConfigValueUnion));
		}

		ConfigValue::~ConfigValue() {}

		void ConfigValue::write(std::ostream & strm, int tab) {
			if (type == ConfigValueType::t_int) {
				strm << value.intVal;
			} else if (type == ConfigValueType::t_long) {
				strm << value.longVal;
			} else if (type == ConfigValueType::t_float) {
				strm << value.floatVal;
			} else if (type == ConfigValueType::t_bool) {
				strm << value.boolVal ? "true" : "false";
			} else if (type == ConfigValueType::t_string) {
				strm << "\"" << value.stringVal << "\"";
			} else if (type == ConfigValueType::t_config) {
				value.configVal->write(strm, tab);
			} else if (type == ConfigValueType::t_array) {
				value.arrayVal->write(strm, tab);
			}
		}


		ConfigValue::ConfigValueUnion::ConfigValueUnion() {}
		ConfigValue::ConfigValueUnion::~ConfigValueUnion() {}
	}
}

