#pragma once

#include "Scene.h"
#include "Config.h"

namespace spectra {
	class Spectra {
	public:
		static void run(Scene* start);
		static void quit();

		static int getTargetFPS();

	private:
		Spectra();

		static Config config;
		static bool running;
		static int targetFPS;
	};
}