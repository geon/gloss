#include "Cylinder.h"
#include "Plane.h"
#include "Ray.h"
#include "pi.h"
#include <math.h>
//#include "randf.h"


Cylinder makeCylinder(const Vector endA, const Vector endB, const float radius) {

	return (Cylinder) {endA, endB, radius};
}


Intersection makeCapIntersection(const Cylinder cylinder, const Ray ray, const Plane cap, const Vector relevantEnd) {

	Intersection capIntersection = pIntersect(cap, ray);
	if (capIntersection.hitType == missed || vLengthSquared(vSub(capIntersection.position, relevantEnd)) < cylinder.radius * cylinder.radius) {
		
		return capIntersection;
	}

	return makeIntersectionMiss();
}
Intersection makeCylinderIntersection(const float distance, const Cylinder cylinder, const Ray ray, const Vector endAInCylinderPlane, const Vector cylinderDirection, const float endAAlongDirection, const float endBAlongDirection) {

	Vector position = vAdd(ray.origin, vsMul(ray.direction, distance));

	// Clip to beginning and end.
	float intersectionAlongDirection = vDot(position, cylinderDirection);
	if (intersectionAlongDirection > endAAlongDirection && intersectionAlongDirection < endBAlongDirection) {

		Intersection intersection;
		
		intersection.hitType = surface;
		intersection.distance = distance;
		intersection.position = position;
		Vector intersectionProjectedOntoCylinderDirection = vAdd(endAInCylinderPlane, vsMul(cylinderDirection, vDot(intersection.position, cylinderDirection)));
		intersection.normal = vsDiv(vSub(intersection.position, intersectionProjectedOntoCylinderDirection), cylinder.radius);

		return intersection;
	}
	
	return makeIntersectionMiss();
}
Intersection cIntersect(const Cylinder cylinder, const Ray ray) {

	// TODO: Store endA, direction and length instead. Compute in the constructor.
	// Normalize stuff.
	Vector cylinderVector = vSub(cylinder.endB, cylinder.endA);
	float cylinderLength = vLength(cylinderVector);
	Vector cylinderDirection = vsDiv(cylinderVector, cylinderLength);

	// Working in 2D is easier, so project the ray onto the plane with the cylinder as it's normal.
	
	
	float rayDirectionAlongDirection = vDot(ray.direction, cylinderDirection);
	Vector rayDirectionProjectedOntoCylinderVector = vsMul(cylinderDirection, rayDirectionAlongDirection);
	Vector rayDirectionInCylinderPlane = vNormalized(vSub(ray.direction, rayDirectionProjectedOntoCylinderVector));
	
	float endAAlongDirection = vDot(cylinder.endA, cylinderDirection);
	float endBAlongDirection = vDot(cylinder.endB, cylinderDirection);
	Vector endAProjectedOntoCylinderVector = vsMul(cylinderDirection, endAAlongDirection);
	Vector endAInCylinderPlane = vSub(cylinder.endA, endAProjectedOntoCylinderVector);
	Vector rayOriginProjectedOntoCylinderVector = vAdd(endAInCylinderPlane, vsMul(cylinderDirection, vDot(ray.origin, cylinderDirection)));
		

	// Modified sphere intersetion test.

	float b = vDot(vSub(ray.origin, rayOriginProjectedOntoCylinderVector), rayDirectionInCylinderPlane);
	float c = vLengthSquared(vSub(ray.origin, rayOriginProjectedOntoCylinderVector)) - cylinder.radius*cylinder.radius;
	float det = b*b - c;

	Intersection cylinderIntersection = makeIntersectionMiss();
	if (det>=0) {

		det = sqrtf(det);

		float t1 = -b - det;
		if (t1>0.0001) {
		
			// t1 is the distance *in the cylinder plane*.
		
			cylinderIntersection = makeCylinderIntersection(
				t1 / sqrtf(1-rayDirectionAlongDirection*rayDirectionAlongDirection),
				cylinder,
				ray,
				endAInCylinderPlane,
				cylinderDirection,
				endAAlongDirection,
				endBAlongDirection
			);

		} else {

			float t2=-b+det;
			if (t2>0.0001) {

				cylinderIntersection = makeCylinderIntersection(
					t2 / sqrtf(1-rayDirectionAlongDirection*rayDirectionAlongDirection),
					cylinder,
					ray,
					endAInCylinderPlane,
					cylinderDirection,
					endAAlongDirection,
					endBAlongDirection
				);
			}
		}
	}


	// Intersect end caps.
	Intersection capIntersection;
	float rayDirectionAlongCylinderDirection = vDot(ray.direction, cylinderDirection);
	if (rayDirectionAlongCylinderDirection > 0) {

		capIntersection = makeCapIntersection(
			cylinder,
			ray,
			makePlane(vNegated(cylinderDirection), -endAAlongDirection),
			cylinder.endA
		);

	} else {

		capIntersection = makeCapIntersection(
			cylinder,
			ray,
			makePlane(cylinderDirection, endBAlongDirection),
			cylinder.endB
		);
	}
	
	
	// Return the closest intersection.
	if (cylinderIntersection.hitType != missed && capIntersection.hitType != missed) {
		
		return cylinderIntersection.distance < capIntersection.distance ? cylinderIntersection : capIntersection;

	} else  if (cylinderIntersection.hitType != missed) {

		return cylinderIntersection;

	} else {
	
		return capIntersection;
	}
}

//Vector cSampleSurface(const Cylinder s) {
//
//	return vAdd(s.position, vRotated(
//		vRotated(makeVector(s.radius, 0, 0), makeVector(0, 1, 0), acosf(randf()*2-1)),
//		makeVector(1, 0, 0),
//		randf() * 2*PI
//	));
//}

float cSurfaceArea(const Cylinder c) {

	float capArea = c.radius * c.radius * PI;
	
	float length = vLength(vSub(c.endA, c.endB));
	float circumference = c.radius * PI * 2;
	float sideArea = circumference * length;
	
	return capArea * 2 + sideArea;
}
