#include "Spectra.h"
#include "Log.h"
#include "World.h"
#include "Clock.h"
#include "Time.h"
#include "Vulkan.h"
#include "Window.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Window.h"
#include "Light.h"

namespace spectra {
	using namespace internal;

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
			config["enable_validation"] = false;

			config.write();
		}

		targetFPS = config["framerate"].intValue();

		float spf;
		ulong uspf;
		
		if (targetFPS > 0) {
			spf = 1.0f / targetFPS;
			uspf = 1000000 / targetFPS;
		} else {
			spf = 0;
			uspf = 0;
		}

		running = true;
		
		bool clockInit = internal::Clock::init();
		if (!clockInit) {
			Log::log("Failed to initialize timing.");
		}

		int width = config["window_width"].intValue();
		int height = config["window_height"].intValue();

		internal::Clock clock;
		Window::init();
		Vulkan::init(&config);
		Window::main = new Window(width, height, "Spectra", true, false);
		Vulkan::createLogicalDevice(Window::main);
		Window::main->complete(width, height);

		Camera::init();
		Light::init();

		if (start) {
			World::load(start, false);
		}

		Time::init(spf);


		while (running && !Window::main->closeRequested()) {
			clock.reset();
			Window::pollEvents();
			World::update();
			Light::updateAll();
			
			for (Window *window : Window::allWindows) {
				if (window->repaintOnRender) {
					window->repaint();
				}
			}

			ulong time = clock.elapsed();

			if (targetFPS > 0) {
				if (time < uspf) {
					clock.sleep(uspf - time);
					//Log::log << "Time: " << int(time / 1000) << "ms\n";
				} else {
					Log::log << "Can't keep up! Frame time " << int(time / 1000) << " milliseconds.\n";
				}
			}

			Time::tick();
		}

		Vulkan::getLogicalDevice()->waitIdle();
		World::clear();

		Camera::cleanup();
		Light::cleanup();

		internal::Clock::stop();

		Window::main = nullptr;
		Window::closeAll();
	}

	void Spectra::quit() {
		running = false;
	}

	int Spectra::getTargetFPS() {
		return targetFPS;
	}
}