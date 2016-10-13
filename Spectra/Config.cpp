#include "Config.h"

namespace spectra {
	Config::Config(Json::Value &node) : node(node) { }

	Config::Config(std::string file) : node(dummyNode) {
		loaded = reader.parse(file, node);
	}

	Config::Config() : node(dummyNode) { }

	Config::Config(Config& cfg) : node(cfg.node) { }

	Config Config::operator[](int index) {
		if (node.isArray()) {
			return Config(node[index]);
		} else {
			throw std::runtime_error("Attempt to read index from non-array value.");
		}
	}

	Config Config::operator[](std::string key) {
		if (node.isObject()) {
			return Config(node[key]);
		} else {
			throw std::runtime_error("Attempt to read key " + key + " from non-object value.");
		}
	}

	int Config::length() {
		if (node.isArray()) {
			return node.size();
		} else {
			throw std::runtime_error("Attempt to read length from non-array value.");
		}
	}

	Config::operator int() {
		if (node.isInt()) {
			return node.asInt();
		} else {
			throw std::runtime_error("Attempt to read int from non-int value.");
		}
	}

	Config::operator long long() {
		if (node.isInt64()) {
			return node.asInt64();
		} else {
			throw std::runtime_error("Attempt to read int64 from non-int64 value.");
		}
	}

	Config::operator float() {
		if (node.isDouble()) {
			return node.asDouble();
		} else {
			throw std::runtime_error("Attempt to read float from non-float value.");
		}
	}

	Config::operator bool() {
		if (node.isBool()) {
			return node.asBool();
		} else {
			throw std::runtime_error("Attempt to read bool from non-bool value.");
		}
	}

	Config::operator std::string() {
		if (node.isString()) {
			return node.asString();
		} else {
			throw std::runtime_error("Attempt to read string from non-string value.");
		}
	}

	void Config::operator=(int value) {
		node = value;
	}

	void Config::operator=(long long value) {
		node = value;
	}

	void Config::operator=(float value) {
		node = value;
	}

	void Config::operator=(bool value) {
		node = value;
	}

	void Config::operator=(std::string value) {
		node = value;
	}

	bool Config::isLoaded() {
		return loaded;
	}

}