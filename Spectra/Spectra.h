#pragma once

#include "Scene.h"
#include "Config.h"

namespace spectra {
	class Spectra {
	public:
		void run(Scene* start);
		void quit();

	private:
		Spectra();

		Config config;
	};
}