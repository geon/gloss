#ifndef SPHERE_H
#define SPHERE_H

#include "Vector.h"
#include "Intersection.h"
#include "Ray.h"

typedef struct {
	Vector position;
	float radius;
} Sphere;

Sphere makeSphere(const Vector position, const float radius);

Intersection sIntersect	(const Sphere sphere, const Ray ray);
Vector       sSampleSurface(const Sphere s);
float        sSurfaceArea  (const Sphere s);

#endif // SPHERE_H