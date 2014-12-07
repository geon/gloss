#include "Sphere.h"
#include "Ray.h"
#include <math.h>
//#include "randf.h"
#include "pi.h"


Sphere makeSphere(const Vector position, const float radius) {

	return (Sphere) {position, radius};
}

Intersection sIntersect(const Sphere sphere, const Ray ray) {

	Intersection intersection;
	intersection.hitType = missed;

	float b = vDot(vSub(ray.origin, sphere.position), ray.direction);
	float c = vLengthSquared(vSub(ray.origin, sphere.position)) - sphere.radius*sphere.radius;
	float det = b*b - c;

	if (det>=0) {

		det = sqrtf(det);

		float t1 = -b - det;
		if (t1>0.0001) {

			intersection.distance = t1;

			// Identical to next block.
			intersection.hitType = surface;
			intersection.position = vAdd(ray.origin, vsMul(ray.direction, intersection.distance));
			intersection.normal = vNormalized(vSub(intersection.position, sphere.position));

		} else {

			float t2=-b+det;
			if (t2>0.0001) {

				intersection.distance = t2;

				// Identical to previous block.
				intersection.hitType = surface;
				intersection.position = vAdd(ray.origin, vsMul(ray.direction, intersection.distance));
				intersection.normal = vNormalized(vSub(intersection.position, sphere.position));
			}
		}
	}

	return intersection;
}

//Vector sSampleSurface(const Sphere s) {
//
//	return vAdd(s.position, vRotated(
//		vRotated(makeVector(s.radius, 0, 0), makeVector(0, 1, 0), acosf(randf()*2-1)),
//		makeVector(1, 0, 0),
//		randf() * 2*PI
//	));
//}

float sSurfaceArea(const Sphere s) {
	return s.radius * s.radius * PI * 4;
}
