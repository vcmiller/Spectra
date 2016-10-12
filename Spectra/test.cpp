#include "Property.h"
#include "Vector.h"
#include "Matrix.h"
#include "Log.h"
#include "List.h"
#include "Object.h"
#include "ObjectSet.h"

using namespace util;
using namespace spectra;
using namespace std;

class Test {
public:
	Property<int> prop{ &val, getter, setter };

	Test() {
	}

private:
	int val;

	static int getter(int val) {
		cout << "Getter!\n";
		return val;
	}

	static int setter(int val) {
		cout << "Setter!\n";
		return val;
	}
};

void main() {
	ObjectSet<Object*> objs;

	Object bob;
	bob.setName("Bob");

	Object fred;
	fred.setName("Fred");

	Object steve;
	steve.setName("Steve");

	objs.add(&bob);
	objs.add(&fred);
	objs.add(&steve);

	for (Object *obj : objs) {
		Log::log << obj->getName() << ", " << obj->getIndex(&objs) << "\n";
	}

	Log::log("========");

	objs.remove(&bob);

	for (Object *obj : objs) {
		Log::log << obj->getName() << ", " << obj->getIndex(&objs) << "\n";
	}
}