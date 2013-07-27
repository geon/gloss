#include "Intersection.h"

Intersection makeIntersection(HitType hitType, Vector point, float distance, Vector normal, Material* material) {

	return (Intersection) {
		hitType,
	    point,
	    distance,
	    normal,
	    material
    };
}