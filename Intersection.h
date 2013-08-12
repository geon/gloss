#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Vector.h"
#include "MaterialForwardDeclaration.h"

typedef enum {
	missed,
	surface,
	perfectBlack,
	origin
} HitType;

typedef struct {

	HitType hitType;
	Vector position;
	float distance;
	Vector normal;
	const Material* material;
} Intersection;

Intersection makeIntersection(HitType hitType, Vector position, float distance, Vector normal , Material* material);

#include "Material.h"

#endif // INTERSECTION_H
