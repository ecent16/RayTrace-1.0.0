#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

using namespace std;
class Ray {
	/*
		Description: 
		- Rays can be described in a function P(t) = A + (t)b, where P = Point position, A = point of Origin,
			b = direction of the ray, and t = new point location based on the point of origin and the direction of 
			the ray.
		- Ray Tracers send rays through pixals and computes the colors seen in the direction of those rays.
		- Steps for Sending Rays into a Scene:
			1. Calculate the ray from the 'eye' through the pixel
			2. Determine which object the ray intersects
			3. Compute a color for the closest intersection point
		- Aspect ratio is usually 16:9, which is the ideal ratio.

	*/
	private:
		point3 orig;
		Vec3 dir;

	public:
		Ray() {}

		Ray(const point3& origin, const Vec3& direction) : orig(origin), dir(direction) {}

		// Return functions for origin and direction:
		const point3& origin() const { return orig; }
		const Vec3& direction() const { return dir; }

		// Function to return the current postion of a point:
		point3 at(float t) const { return orig + t * dir;  }
};

#endif
