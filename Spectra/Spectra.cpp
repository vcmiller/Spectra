#include "Spectra.h"
#include "Log.h"
#include "World.h"
#include "Clock.h"
#include "Time.h"

namespace spectra {
	Config Spectra::config;
	bool Spectra::running;
	int Spectra::targetFPS;

	Spectra::Spectra() {}

	void Spectra::run(Scene *start) {
		config = Config("spectra.json");

		if (!config.isLoaded()) {
			config["window_width"] = 800;
			config["window_height"] = 600;
			config["framerate"] = 60;

			config.write();
		}

		targetFPS = config["framerate"].intValue();
		float spf = 1.0f / targetFPS;
		ulong uspf = 1000000 / targetFPS;

		running = true;
		
		bool clockInit = internal::Clock::init();
		if (!clockInit) {
			Log::log("Failed to initialize timing.");
		}

		internal::Clock clock;
		Time::init(spf);

		if (start) {
			World::load(start, false);
		}

		while (running) {
			clock.reset();
			World::update();
			World::render();

			ulong time = clock.elapsed();

			if (time < uspf) {
				clock.sleep(uspf - time);
			} else {
				Log::log << "Can't keep up! Frame time " << int(time / 1000) << " milliseconds.\n";
			}

			Time::tick();
		}

		internal::Clock::stop();
	}

	void Spectra::quit() {
		running = false;
	}

	int Spectra::getTargetFPS() {
		return targetFPS;
	}
}