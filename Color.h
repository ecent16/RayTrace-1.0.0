#ifndef COLOR_H
#define COLOR_H

#include "Interval.h"
#include "Vec3.h"

// Initialize vec3Class:
using color = Vec3;
using namespace std;

inline float linear_to_gamma(float linear_component) {
	if (linear_component > 0) { return sqrt(linear_component); }
	return 0;
}

void write_color(ostream& out, const color& px_color) {
	// Output the pixel colors to the screen:
	auto r = px_color.x();
	auto g = px_color.y();
	auto b = px_color.z();

	// Apply a linear to gamma transform for gamma 2:
	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	// Translate the [0, 1] component values to the byte range [0, 255]:
	static const Interval intensity(0.000, 0.999);

	int rByte = int(256 * intensity.clamp(r));
	int gByte = int(256 * intensity.clamp(g));
	int bByte = int(256 * intensity.clamp(b));

	// Write out the pixel color components:
	out << rByte << ' ' << gByte << ' ' << bByte << '\n';

}

#endif
