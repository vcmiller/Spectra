#include "Property.h"
#include "Vector.h"
#include "Matrix.h"
#include "Log.h"
#include "List.h"
#include "Object.h"
#include "ObjectSet.h"
#include "Config.h"
#include "Spectra.h"
#include "Clock.h"
#include "Time.h"
#include "Scene.h"
#include "GameObject.h"
#include "Component.h"
#include "Window.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Input.h"
#include "Key.h"

using namespace util;
using namespace spectra;
using namespace spectra::internal;
using namespace std;

class Spinner : public Component {
public:

	Spinner() { }
	void onCreate() {
		//Input::setCursorMode(CursorMode::Locked);
	}

	void update() {
		if (Input::getKey(Key::Space)) {
			transform.rotate(Quaternion::euler(Vector3(0, Time::delta(), 0)));
		}

		if (Input::getKeyDown(Key::LeftShift)) {
			transform.setLocalScale(Vector3(1, -1, 1));
		}

		if (Input::getKeyUp(Key::LeftShift)) {
			transform.setLocalScale(Vector3(1, 1, 1));
		}

		if (Input::getKeyDown(Key::Escape)) {
			Input::setCursorMode(CursorMode::Normal);
		}

		if (Input::getTypedText().length() > 0) {
			Log::log(Input::getTypedText());
		}

		transform.translate(Vector3::forward * Input::getMouseWheelDelta() * 0.3f);

		if (Input::getMouseButton(1)) {
			transform.translate(Vector3::forward * Input::getMouseDelta().y * 0.01f);
		}
		
		if (Input::getMouseButton(0)) {
			transform.rotate(Vector3::up, -Input::getMouseDelta().x * .005f, Space::Global);
			transform.rotate(Quaternion::euler(Vector3(Input::getMouseDelta().y * 0.005f, 0, 0)), Space::Global);
			//transform.rotate(Vector3::right, Input::getMouseDelta().y * .02f, Space::Global);
		}
	}

	void onDestroy() {
	}
};

class TestScene : public Scene {
public:
	Shader *shader;
	Texture *texture;
	Material *material;
	Mesh *mesh;
	//Window *window;

	void populate() {
		//window = new Window(400, 300, "Test Window", true, true);

		shader = new Shader("Shaders/triangle");
		texture = new Texture("Textures/spas.png");
		material = new Material(shader, texture);
		mesh = new Mesh("Models/spas.obj");

		GameObject *bob = new GameObject();
		bob->addComponent<MeshRenderer>()->init(mesh, material);
		bob->addComponent<Spinner>();

		bob->transform.setPosition(Vector3(0, 0, 0));

		GameObject *camera = new GameObject();
		camera->addComponent<Camera>();// ->setRenderWindow(window);
		camera->transform.setPosition(Vector3(0, 0, -1));

		Camera *cam = camera->getComponent<Camera>();
		cam->setBackgroundColor(Color(0.2f, 0.2f, 0.2f));

		//cam->setProjection(45, .1f, 100.f);

		add(bob);
		add(camera);
	}

	void depopulate() {
		Scene::depopulate();

		delete mesh;
		delete material;
		delete texture;
		delete shader;
	}
};

void main() {
	Log::setFlush(true);
	Spectra::run(new TestScene());
}