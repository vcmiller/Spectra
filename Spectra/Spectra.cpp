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
		Window::init();
		Vulkan::init(&config);
		Window::main = new Window(config["window_width"].intValue(), config["window_height"].intValue(), "Spectra", false, false);
		Vulkan::createLogicalDevice(Window::main);
		Window::main->complete();


		Shader *shader = new Shader("Shaders/triangle");
		Texture *texture = new Texture("Textures/spas.png");
		Material *material = new Material(Window::main, shader, texture);
		Mesh *mesh = new Mesh("Models/spas.obj");

		if (start) {
			World::load(start, false);
		}

		Time::init(spf);

		GameObject *bob = new GameObject();
		bob->addComponent<MeshRenderer>()->init(mesh, material);

		bob->transform.setPosition(Vector3(-.5f, 0, 2));

		GameObject *joe = new GameObject();
		joe->addComponent<MeshRenderer>()->init(mesh, material);
		joe->transform.setPosition(Vector3(.5f, 0, 2));

		GameObject *camera = new GameObject();
		camera->addComponent<Camera>();
		camera->transform.setPosition(Vector3(0, 0, -1));

		while (running && !Window::main->closeRequested()) {
			clock.reset();
			Window::pollEvents();
			World::update();
			bob->transform.rotate(Quaternion::euler(Vector3(0, Time::delta(), 0)));
			//bob->transform.translate(Vector3(0, 0, Time::delta() * 0.2f));

			Window::main->acquireNextImage();

			for (Camera *camera : Camera::allCameras) {
				camera->capture();
			}

			Window::main->display();

			ulong time = clock.elapsed();

			if (time < uspf) {
				clock.sleep(uspf - time);
				//Log::log << "Time: " << int(time / 1000) << "ms\n";
			} else {
				Log::log << "Can't keep up! Frame time " << int(time / 1000) << " milliseconds.\n";
			}

			Time::tick();
		}

		Vulkan::getLogicalDevice()->waitIdle();
		World::clear();

		delete material;
		delete shader;
		delete texture;
		delete mesh;

		internal::Clock::stop();

		delete Window::main;
		Window::main = nullptr;
	}

	void Spectra::quit() {
		running = false;
	}

	int Spectra::getTargetFPS() {
		return targetFPS;
	}
}