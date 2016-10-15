#include "Property.h"
#include "Vector.h"
#include "Matrix.h"
#include "Log.h"
#include "List.h"
#include "Object.h"
#include "ObjectSet.h"
#include "Config.h"

using namespace util;
using namespace spectra;
using namespace std;

void main() {
	Config c("config.json");
	c["bob"] = 1.0f;
	c["dog"] = true;
	c.write();
}