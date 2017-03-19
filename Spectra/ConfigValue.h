#pragma once

#include "jsoncpp/json.h"
#include <string>
#include "Log.h"

namespace spectra {
	class Config;
	class ConfigArray;

	namespace internal {
		enum class ConfigValueType {
			t_int, t_long, t_float, t_bool, t_string, t_config, t_array, t_none
		};

		struct ConfigValue {
			union  ConfigValueUnion {
				int intVal;
				long long longVal;
				float floatVal;
				bool boolVal;
				const char* stringVal;
				Config* configVal;
				ConfigArray* arrayVal;

				ConfigValueUnion();
				~ConfigValueUnion();
			} value;

			ConfigValueType type;

			void read(const Json::Value &node);
			void write(Json::Value &node);

			void set(int val);
			void set(long long val);
			void set(float val);
			void set(bool val);
			void set(std::string val);
			void set(Config* val);
			void set(ConfigArray* val);

			ConfigValue();
			ConfigValue(const ConfigValue &v);
			~ConfigValue();
		};
	}
	
	
}