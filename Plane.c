#include "Plane.h"
#include "Ray.h"
#include <math.h>


Plane makePlane(const Vector normal, const float distance) {

	return (Plane) {normal, distance};
}

Intersection pIntersect(const Plane plane, const Ray ray) {

	Intersection intersection;

	float normalDotRayDirection = vDot(plane.normal, ray.direction);
	if (normalDotRayDirection < 0 && !pIsInside(plane, ray.origin)){

		intersection.hitType  = surface;
		intersection.distance = (plane.distance - vDot(plane.normal, ray.origin)) / normalDotRayDirection;
		intersection.position = vAdd(ray.origin, vsMul(ray.direction, intersection.distance));
		intersection.normal   = plane.normal;

	} else {

		intersection.hitType = missed;
	}

	return intersection;
};

bool pIsInside(const Plane plane, const Vector point) {

	return vDot(point, plane.normal) < plane.distance;
};