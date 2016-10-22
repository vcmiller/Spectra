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

using namespace util;
using namespace spectra;
using namespace spectra::internal;
using namespace std;

class FramePrinter : public Component {
public:
	Window* window;

	FramePrinter() { }
	void onCreate() {
		window = new Window(200, 200, "Custom Window");
	}

	void update() {
		//Log::log << "Delta time: " << Time::delta() << "\n";
		//string s;
		//cin >> s;
	}

	void onDestroy() {
		delete window;
	}
};

class TestScene : public Scene {
public:
	void populate() {
		GameObject *obj = new GameObject();
		obj->addComponent<FramePrinter>();
	}
};

void main() {
	Log::setFlush(true);
	Spectra::run(new TestScene());
}