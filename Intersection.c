#include "Intersection.h"

Intersection makeIntersectionMiss() {

	Intersection i;
	i.hitType = missed;
	
	return i;
}

Intersection makeIntersection(const HitType hitType, const Vector position, const float distance, const Vector normal, const Material* material) {

	return (Intersection) {
		hitType,
		position,
		distance,
		normal,
		material
	};
}