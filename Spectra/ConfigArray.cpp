#include "ConfigArray.h"
#include "Config.h"

namespace spectra {

	ConfigArray::ConfigArray() {}

	ConfigArray::~ConfigArray() {}

	int ConfigArray::getInt(int index, int def) {
		if (index >= getLength()) {
			setLength(index + 1);
			values[index].set(def);
		}

		return values[index].value.intVal;
	}

	long long ConfigArray::getLong(int index, long long def) {
		if (index >= getLength()) {
			setLength(index + 1);
			values[index].set(def);
		}

		return values[index].value.longVal;
	}

	float ConfigArray::getFloat(int index, float def) {
		if (index >= getLength()) {
			setLength(index + 1);
			values[index].set(def);
		}

		return values[index].value.floatVal;
	}

	bool ConfigArray::getBool(int index, float def) {
		if (index >= getLength()) {
			setLength(index + 1);
			values[index].set(def);
		}

		return values[index].value.floatVal;
	}

	std::string ConfigArray::getString(int index, std::string def) {
		if (index >= getLength()) {
			setLength(index + 1);
			values[index].set(def);
		}

		return values[index].value.stringVal;
	}

	Config & ConfigArray::getConfig(int index) {
		if (index >= getLength()) {
			setLength(index + 1);
			Config *cfg = new Config();
			values[index].type = internal::ConfigValueType::t_config;
			values[index].value.configVal = cfg;
		}

		return *values[index].value.configVal;
	}

	ConfigArray & ConfigArray::getArray(int index) {
		if (index >= getLength()) {
			setLength(index + 1);
			ConfigArray *arr = new ConfigArray();
			values[index].type = internal::ConfigValueType::t_array;
			values[index].value.arrayVal = arr;
		}

		return *values[index].value.arrayVal;
	}

	void ConfigArray::setInt(int index, int val) {
		if (index >= getLength()) {
			setLength(index + 1);
		}

		values[index].set(val);
	}

	void ConfigArray::setLong(int index, long long val) {
		if (index >= getLength()) {
			setLength(index + 1);
		}

		values[index].set(val);
	}

	void ConfigArray::setFloat(int index, float val) {
		if (index >= getLength()) {
			setLength(index + 1);
		}

		values[index].set(val);
	}

	void ConfigArray::setBool(int index, bool val) {
		if (index >= getLength()) {
			setLength(index + 1);
		}

		values[index].set(val);
	}

	void ConfigArray::setString(int index, std::string val) {
		if (index >= getLength()) {
			setLength(index + 1);
		}

		values[index].set(val);
	}

	int ConfigArray::getLength() {
		return values.size();
	}

	void ConfigArray::setLength(int length) {
		values.resize(length);
	}

	void ConfigArray::read(const Json::Value &node) {
		values.resize(node.size());
		for (int i = 0; i < node.size(); i++) {
			values[i].read(node[i]);
		}
	}

	void ConfigArray::write(Json::Value &node) {
		node = Json::arrayValue;

		node.resize(values.size());
		for (int i = 0; i < node.size(); i++) {
			values[i].write(node[i]);
		}
	}
}
