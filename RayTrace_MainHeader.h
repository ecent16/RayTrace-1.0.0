#ifndef RAYTRACE_MAINHEADER_H
#define RAYTRACE_MAINHEADER_H
/*
	Description: Main utility header for constants and headers.

	Notes:
	- Aliasing: harsh 'stair step' edges of a rendered object. Antialiasing is used to blend those jagged pixels of
		an rander image.
*/

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

#include <random>

using namespace std;
// ************************* C++ Stf Usings ***********************************
using std::make_shared;
using std::shared_ptr;

// ************************* Constant Variables ***********************************
const float infinity = std::numeric_limits<float>::infinity();
const double pi = 3.1415926535897932385;

// ************************* Utilities ***********************************
inline double degrees_to_radians(double degrees) { return degrees * pi / 180.0; }

inline float random_float() { 
	// New random number generator function:
	static uniform_real_distribution<float> distribution(0.0, 1.0);
	static mt19937 generator;
	return distribution(generator);
}

inline float random_float(float min, float max) {
	return min + (max - min) * random_float();
}

// ************************* Header Files ***********************************
#include "Color.h"
#include "Interval.h"
#include "Ray.h"
#include "Vec3.h"

#endif