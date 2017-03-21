#pragma once

#include <string>
#include "Vector.h"
#include "List.h"

using namespace spectra;

class Spline {
public:
	Spline(std::string file);

	class Point {
		Vector3 position;
		Vector3 euler;
	};

private:
	List<Point> points;
};