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

using namespace util;
using namespace spectra;
using namespace spectra::internal;
using namespace std;

class FramePrinter : public Component {
public:
	FramePrinter() { }
	void onCreate() {
		Log::log("Created");
	}

	void update() {
		Log::log << "Delta time: " << Time::delta() << "\n";
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