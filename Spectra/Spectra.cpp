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

	Config *Spectra::config;
	bool Spectra::running;
	int Spectra::targetFPS;

	Spectra::Spectra() {}

	void Spectra::run(Scene *start) {
		config = new Config("spectra.json");

		targetFPS = config->getInt("framerate", 60);

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

		int width = config->getInt("window_width", 800);
		int height = config->getInt("window_height", 600);

		std::string title = config->getString("window_title", "Spectra");
		bool resizeable = config->getBool("window_resizeable", true);

		internal::Clock clock;
		Window::init();
		Vulkan::init(config);
		Window::main = new Window(width, height, title, resizeable, false);
		Vulkan::createLogicalDevice(Window::main);
		Window::main->complete(width, height);

		config->write();

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

		delete config;
	}

	void Spectra::quit() {
		running = false;
	}

	int Spectra::getTargetFPS() {
		return targetFPS;
	}
}