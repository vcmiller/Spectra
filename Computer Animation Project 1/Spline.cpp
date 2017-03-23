#include "Spline.h"
#include <sstream>

Spline::Spline(std::string file) {
	read(file);
}

float Spline::getTime() {
	return time;
}

int Spline::getNumPoints() {
	return numPoints;
}

Vector3 Spline::getPosition(int p) {
	return points[p].position;
}

Vector3 Spline::getRotation(int p) {
	return points[p].euler;
}

void Spline::read(std::string file) {
	std::ifstream strm(file);
	if (strm.is_open()) {
		readInt(strm);
		numPoints = readInt(strm);
		time = readFloat(strm);
		for (int i = 0; i < numPoints; i++) {
			Point p;
			p.position = readVector(strm);
			p.euler = readVector(strm);
			points.add(p);
		}
	} else {
		Log::log("Could not open file!");
	}
}

std::string Spline::readLine(std::istream & strm) {
	std::string line;

	do {
		if (!std::getline(strm, line)) {
			Log::log("Error: end of file reached!");
			return "";
		}
	} while (line[0] == '#' || line[0] == '\n' || line[0] == '\r' || line[0] == '\0');

	return line;
}

float Spline::readFloat(std::ifstream & strm) {

	std::istringstream iss(readLine(strm));
	float f;
	if (!(iss >> f)) {
		Log::log("Error: Couldn't read float!");
		return -1;
	}

	return f;
}

int Spline::readInt(std::ifstream & strm) {
	std::string s = readLine(strm);
	std::istringstream iss(s);
	int i;
	if (!(iss >> i)) {
		Log::log("Error: Couldn't read int!");
		return -1;
	}
	
	return i;
}

Vector3 Spline::readVector(std::ifstream & strm) {
	std::string s = readLine(strm);
	std::istringstream iss(s);
	float x, y, z;

	char s2[32];
	iss.getline(s2, 32, ',');
	x = atof(s2);
	iss.getline(s2, 32, ',');
	y = atof(s2);
	iss >> z;

	return Vector3(x, y, z);
}
