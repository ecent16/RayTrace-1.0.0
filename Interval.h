#ifndef INTERVAL_H
#define INTERVAL_H

using namespace std;
class Interval {
	/*
		- Purpose: To manage real-valued intervals with min and max values.
	*/
	public:
		float min, max;

		// Default intervals:
		// variable infinity comes from the RayTrace_MainHeader file:
		Interval() : min(+infinity), max(-infinity) {}

		Interval(float min, float max) : min(min), max(max) {}

		float size() const { return max - min; }

		bool contains(float x) const { return min <= x && x <= max; }

		bool surrounds(float x) const { return min < x && x < max;  }

		float clamp(float x) const {
			// Generate pixels using multiple samples:
			if (x < min) { return min; }
			else if (x > max) { return max; }
			else { return x; }
		}

		static const Interval empty, universe;

};

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);

#endif
