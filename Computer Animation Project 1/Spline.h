#pragma once

#include <string>
#include "Vector.h"
#include "List.h"

#include <fstream>

using namespace spectra;

class Spline {
public:
	Spline(std::string file);

	struct Point {
		Vector3 position;
		Vector3 euler;
	};

private:
	List<Point> points;
	void read(std::string file);
	std::string readLine(std::istream &strm);
	float readFloat(std::ifstream &strm);
	int readInt(std::ifstream &strm);
	Vector3 readVector(std::ifstream &strm);

	int numPoints;
	float time;
};