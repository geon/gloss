#include "Cylinder.h"
#include "Plane.h"
#include "Ray.h"
#include "pi.h"
#include <math.h>
//#include "randf.h"


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
	
	float endAAlongDirection = vDot(cylinder.endA, cylinderDirection);
	Vector endAProjectedOntoCylinderVector = vsMul(cylinderDirection, endAAlongDirection);
	Vector endInCylinderPlane = vSub(cylinder.endA, endAProjectedOntoCylinderVector);
	Vector rayOriginProjectedOntoCylinderVector = vAdd(endInCylinderPlane, vsMul(cylinderDirection, vDot(ray.origin, cylinderDirection)));
		

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
			goto cylinderIntersection;
			
			// Yeah, I know the stigma around GOTO, but it was the best way to reduce code duplication.

		} else {

			float t2=-b+det;
			if (t2>0.0001) {

				intersection.distance = t2;
				goto cylinderIntersection;
			}
		}
	}
	
	//  No hit, so return miss.
	return intersection;


cylinderIntersection:;

	intersection.hitType = surface;
	intersection.position = vAdd(ray.origin, vsMul(ray.direction, intersection.distance));
	Vector intersectionProjectedOntoCylinder = vsMul(cylinderDirection, vDot(intersection.position, cylinderDirection));
	intersection.normal = vsDiv(vSub(intersection.position, intersectionProjectedOntoCylinder), cylinder.radius);


	// Check ends.
	
	float intersectionAlongDirection = vDot(intersection.position, cylinderDirection);
	float endBAlongDirection = vDot(cylinder.endB, cylinderDirection);

	Plane cap;
	Vector relevantEnd;
	if (intersectionAlongDirection < endAAlongDirection) {

		// TODO: Store the endcaps?
		cap = makePlane(vNegated(cylinderDirection), -endAAlongDirection);
		relevantEnd = cylinder.endA;
		goto capIntersection;
	}
	if (intersectionAlongDirection > endBAlongDirection) {

		cap = makePlane(cylinderDirection, -endBAlongDirection);
		relevantEnd = cylinder.endB;
		goto capIntersection;
	}

	// No hit, so return cylinder/miss.
	return intersection;


capIntersection:;

	Intersection capIntersection = pIntersect(cap, ray);
	if (vLengthSquared(vSub(capIntersection.position, relevantEnd)) < cylinder.radius * cylinder.radius) {
		
		intersection = capIntersection;
		
	} else {
		
		intersection.hitType = missed;
	}

	// The cap intersection.
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
