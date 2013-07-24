#include "Intersection.h"

Intersection makeIntersection(HitType hitType, cVector point, float distance, cVector normal, cMaterial* material) {

	return (Intersection) {
		hitType,
	    point,
	    distance,
	    normal,
	    material
    };
}