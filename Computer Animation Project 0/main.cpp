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
#include "FMath.h"

using namespace util;
using namespace spectra;
using namespace spectra::internal;
using namespace std;

class Spinner : public Component {
public:
	Vector3 euler;

	Spinner() {}
	void onCreate() {
		//Input::setCursorMode(CursorMode::Locked);
		euler = Vector3(0, 0, 0);
	}

	void update() {
		float f = Time::delta() * FMath::halfCircle;
		euler += Vector3(f, f, f);

		transform.setRotation(Quaternion::euler(euler));

		transform.setPosition(Vector3(0, FMath::sin(4 * Time::time()), 0));
	}

	void onDestroy() {}
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
		texture = new Texture("Textures/Ornate.jpg");
		material = new Material(shader, texture);
		mesh = new Mesh("Models/Suzanne.obj");

		GameObject *bob = new GameObject();
		bob->addComponent<MeshRenderer>()->init(mesh, material);
		bob->addComponent<Spinner>();

		bob->transform.setPosition(Vector3(0, 0, 0));

		GameObject *camera = new GameObject();
		camera->addComponent<Camera>();// ->setRenderWindow(window);
		camera->transform.setPosition(Vector3(0, 0, -5));

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