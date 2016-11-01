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

using namespace util;
using namespace spectra;
using namespace spectra::internal;
using namespace std;

class Spinner : public Component {
public:

	Spinner() { }
	void onCreate() {
	}

	void update() {
		transform.rotate(Quaternion::euler(Vector3(0, Time::delta(), 0)));
		//transform.setPosition(Vector3(0, Math::sin(Time::time()), 2));
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

		bob->transform.setPosition(Vector3(-.5f, 0, 2));

		GameObject *joe = new GameObject();
		joe->addComponent<MeshRenderer>()->init(mesh, material);
		joe->transform.setPosition(Vector3(.5f, 0, 2));

		GameObject *camera = new GameObject();
		camera->addComponent<Camera>();// ->setRenderWindow(window);
		camera->transform.setPosition(Vector3(0, 0, -1));

		Camera *cam = camera->getComponent<Camera>();

		GameObject *camera2 = new GameObject();
		camera2->addComponent<Camera>();
		camera2->transform.setPosition(Vector3(0, 0, 5));
		camera2->transform.setRotation(Quaternion::euler(Vector3(0, Math::halfCircle, 0)));

		Camera *cam2 = camera2->getComponent<Camera>();

		cam->setViewport(0, 0, .5f, 1.0f);
		cam->setBackgroundColor(Color(1, 0, 0));
		cam2->setViewport(.5f, 0, .5f, 1.0f);
		cam2->setBackgroundColor(Color(0, 1, 0));

		//cam->setProjection(45, .1f, 100.f);

		add(bob);
		add(joe);
		add(camera);
		add(camera2);
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