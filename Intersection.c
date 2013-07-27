#include "Intersection.h"

Intersection makeIntersection(HitType hitType, Vector position, float distance, Vector normal, Material* material) {

	return (Intersection) {
		hitType,
	    position,
	    distance,
	    normal,
	    material
    };
}