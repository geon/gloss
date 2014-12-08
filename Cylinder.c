#include "Cylinder.h"
#include "Ray.h"
#include <math.h>
//#include "randf.h"
#include "pi.h"


Cylinder makeCylinder(const Vector endA, const Vector endB, const float radius) {

	return (Cylinder) {endA, endB, radius};
}

Intersection cIntersect(const Cylinder cylinder, const Ray ray) {

	// TODO: Store endA, direction and length instead. Compute in the constructor.
	// Normalize stuff.
	Vector cylinderVector = vSub(cylinder.endB, cylinder.endA);
	float cylinderLength = vLength(cylinderVector);
	Vector cylinderDirection = vsDiv(cylinderVector, cylinderLength);

	// Working in 2D is easier, so project the ray onto the plane with the cylinder as it's normal.
	
	Vector rayDirectionProjectedOntoCylinderVector = vsMul(cylinderDirection, vDot(ray.direction, cylinderDirection));
	Vector rayDirectionInCylinderPlane = vNormalized(vSub(ray.direction, rayDirectionProjectedOntoCylinderVector));
	
	Vector cylinderEndProjectedOntoCylinderVector = vsMul(cylinderDirection, vDot(cylinder.endA, cylinderDirection));
	Vector cylinderEndInCylinderPlane = vSub(cylinder.endA, cylinderEndProjectedOntoCylinderVector);
	Vector rayOriginProjectedOntoCylinderVector = vAdd(cylinderEndInCylinderPlane, vsMul(cylinderDirection, vDot(ray.origin, cylinderDirection)));
		

	// Modified sphere intersetion test.

	Intersection intersection;
	intersection.hitType = missed;

	float b = vDot(vSub(ray.origin, rayOriginProjectedOntoCylinderVector), rayDirectionInCylinderPlane);
	float c = vLengthSquared(vSub(ray.origin, rayOriginProjectedOntoCylinderVector)) - cylinder.radius*cylinder.radius;
	float det = b*b - c;

	if (det>=0) {

		det = sqrtf(det);

		float t1 = -b - det;
		if (t1>0.0001) {

			intersection.distance = t1;

			// Identical to next block.
			intersection.hitType = surface;
			intersection.position = vAdd(ray.origin, vsMul(ray.direction, intersection.distance));
			Vector intersectionProjectedOntoCylinder = vsMul(cylinderDirection, vDot(intersection.position, cylinderDirection));
			intersection.normal = vsDiv(vSub(intersection.position, intersectionProjectedOntoCylinder), cylinder.radius);

		} else {

			float t2=-b+det;
			if (t2>0.0001) {

				intersection.distance = t2;

				// Identical to previous block.
				intersection.hitType = surface;
				intersection.position = vAdd(ray.origin, vsMul(ray.direction, intersection.distance));
				Vector intersectionProjectedOntoCylinder = vsMul(cylinderDirection, vDot(intersection.position, cylinderDirection));
				intersection.normal = vsDiv(vSub(intersection.position, intersectionProjectedOntoCylinder), cylinder.radius);
			}
		}
	}

	return intersection;
}

//Vector cSampleSurface(const Cylinder s) {
//
//	return vAdd(s.position, vRotated(
//		vRotated(makeVector(s.radius, 0, 0), makeVector(0, 1, 0), acosf(randf()*2-1)),
//		makeVector(1, 0, 0),
//		randf() * 2*PI
//	));
//}

float cSurfaceArea(const Cylinder s) {
	return s.radius * s.radius * PI * 4;
}
