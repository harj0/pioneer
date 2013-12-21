// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef _MATHUTIL_H
#define _MATHUTIL_H

#include "vector3.h"

namespace MathUtil {

// random point on a sphere, distributed uniformly by area
vector3d RandomPointOnSphere(double minRadius, double maxRadius);
inline vector3d RandomPointOnSphere(double radius) { return RandomPointOnSphere(radius, radius); }

vector3d RandomPointInCircle(double minRadius, double maxRadius);
inline vector3d RandomPointInCircle(double radius) { return RandomPointInCircle(0.0, radius); }
inline vector3d RandomPointOnCircle(double radius) { return RandomPointInCircle(radius, radius); }

template <typename T>
inline T LinearInterp(T a, T b, T delta) {
	return a + ((b - a) * delta);
}

template <typename T>
inline T QuadInterpIn(T a, T b, T delta) {
	return ((b - a) * delta * delta) + a;
}

template <typename T>
inline T QuadInterpOut(T a, T b, T delta) {
	return -(b - a) * delta * (delta - 2) + a;
}

template <typename T>
inline T ExpInterpIn(T a, T b, T delta) {
	return (b - a) * pow(2, 10 * (delta - 1)) + a;
}

template <typename T>
inline T ExpInterpOut(T a, T b, T delta) {
	return (b - a) * (-pow(2, -10 * delta) + 1) + a;
}

template <typename T>
inline T CubicInterpIn(T a, T b, T delta) {
	return ((b - a) * delta * delta * delta) + a;
}

template <typename T>
inline T CubicInterpOut(T a, T b, T delta) {
	delta--;
	return (b - a) * (delta * delta * delta + 1) + a;
}

}

#endif
