#ifndef CYLINDER_H
#define CYLINDER_H

#include "Vector.h"
#include "Intersection.h"
#include "Ray.h"

typedef struct {
	Vector endA;
	Vector endB;
	float radius;
} Cylinder;

Cylinder makeCylinder(const Vector endA, const Vector endB, const float radius);

Intersection cIntersect	(const Cylinder cylinder, const Ray ray);
Vector       cSampleSurface(const Cylinder c);
float        cSurfaceArea  (const Cylinder c);

#endif // CYLINDER_H