/*
* Module      : Project 1
* Author      : Vincent Miller
* Email       : vcmiller@wpi.edu
* Course      : CS4732
*
* Description : Used to load and store locations along a spline curve. Does not actually do any interpolation.
*
* Date        : 2017/23/03
*
* (c) Copyright 2013, Worcester Polytechnic Institute.
*/
#pragma once

#include <string>
#include "Vector.h"
#include "List.h"

#include <fstream>

using namespace spectra;

class Spline {
public:
	// Constructor (load from file).
	Spline(std::string file);

	// Point along the spline.
	struct Point {
		// Location of the point.
		Vector3 position;

		// Euler angles in radians.
		Vector3 euler;
	};

	// Get the time taken to traverse the spline.
	float getTime();

	// Get number of points in the spline.
	int getNumPoints();

	// Get position given index.
	Vector3 getPosition(int p);

	// Get rotation given index.
	Vector3 getRotation(int p);

private:
	// List of all control points.
	List<Point> points;

	// Read from a file.
	void read(std::string file);

	// Read a string from a file.
	std::string readLine(std::istream &strm);

	// Read a float from a file.
	float readFloat(std::ifstream &strm);

	// Read an int from a file.
	int readInt(std::ifstream &strm);

	// Read a vector from a file.
	Vector3 readVector(std::ifstream &strm);

	// Number of samples.
	int numPoints;

	// Time limit.
	float time;
};