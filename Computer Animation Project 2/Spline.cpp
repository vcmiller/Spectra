/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Implementation of Spline.
*
* Date        : 2017/23/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/
#include "Spline.h"
#include <sstream>
#include "FMath.h"

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
		readInt(strm); // Number of splines in file, ignored.
		numPoints = readInt(strm); // Read num points.
		time = readFloat(strm); // Read float.
		for (int i = 0; i < numPoints; i++) {
			Point p;
			p.position = readVector(strm); // Read position
			p.euler = readVector(strm) * FMath::toRadians; // Read angles and convert to radians
			points.add(p);
		}
	} else {
		Log::log("Could not open file!");
	}
}

std::string Spline::readLine(std::istream & strm) {
	std::string line;

	do {
		if (!std::getline(strm, line)) { // Error at EOF.
			Log::log("Error: end of file reached!");
			return "";
		}
	} while (line[0] == '#' || line[0] == '\n' || line[0] == '\r' || line[0] == '\0'); // Skip comments and blank lines.

	return line;
}

float Spline::readFloat(std::ifstream & strm) {

	std::istringstream iss(readLine(strm));
	float f;
	if (!(iss >> f)) { // Parse into float using input string stream
		Log::log("Error: Couldn't read float!");
		return -1;
	}

	return f;
}

int Spline::readInt(std::ifstream & strm) {
	std::string s = readLine(strm);
	std::istringstream iss(s);
	int i;
	if (!(iss >> i)) { // Parse int using input string stream
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
	iss.getline(s2, 32, ','); // Read to first comma.
	x = atof(s2); // Convert to float.
	iss.getline(s2, 32, ','); // Read to second comma.
	y = atof(s2); // Convert to float.
	iss >> z; // Read last float, no comma.

	return Vector3(x, y, z);
}
