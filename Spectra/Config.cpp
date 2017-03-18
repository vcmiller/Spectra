#include "Config.h"
#include "Log.h"

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
			}
		} else {
			loaded = false;
		}
	}

	Config::Config() { 
		filename = "";
		loaded = false;
	}


	void Config::read(const Json::Value &node) {
		
		for (std::string key : node.getMemberNames()) {
			Json::Value val = node[key];

			if (val.isInt()) {
				ints[key] = val.asInt();
			} else if (val.isInt64()) {
				longs[key] = val.asInt64();
			} else if (val.isDouble()) {
				floats[key] = val.asDouble();
			} else if (val.isBool()) {
				bools[key] = val.asBool();
			} else if (val.isObject()) {
				configs[key] = new Config();
				configs[key]->parent = this;
				configs[key]->loaded = true;
				configs[key]->read(val);
			} else if (val.isArray()) {
				arrays[key] = new ConfigArray(val);
			}
		}
	}

	void Config::write(Json::Value &node) {
		writeMap(ints, node);
		writeMap(longs, node);
		writeMap(floats, node);
		writeMap(bools, node);
		writeMap(strings, node);

		for (auto sub : configs) {
			Json::Value v;
			sub.second->write(v);
			node[sub.first] = v;
		}

		for (auto arr : arrays) {
			node[arr.first] = arr.second->arr;
		}
	}

	bool Config::isLoaded() {
		return loaded;
	}

	void Config::write() {
		if (filename != "") {
			Json::Value node;
			write(node);

			Json::StyledStreamWriter writer;

			std::ofstream strm(filename);
			writer.write(strm, node);

			strm.close();
		} else {
			throw std::runtime_error("Not loaded from file.");
		}
	}
}