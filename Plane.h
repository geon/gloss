#ifndef PLANE_H
#define PLANE_H

#include "Vector.h"
#include "Intersection.h"
#include "Ray.h"

typedef struct {
	Vector normal;
	float distance;
} Plane;

Plane makePlane(Vector normal, float distance);

Intersection pIntersect(const Plane plane, const Ray ray);
bool pIsInside(const Plane plane, const Vector point);

#endif // PLANE_H