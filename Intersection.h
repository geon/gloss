#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Vector.h"
#include "MaterialForwardDeclaration.h"

typedef enum {
	missed,
	surface,
	origin
} HitType;

typedef struct {

	HitType hitType;
	Vector position;
	float distance;
	Vector normal;
	const Material* material;
} Intersection;

Intersection makeIntersectionMiss();
Intersection makeIntersection(const HitType hitType, const Vector position, const float distance, const Vector normal, const Material* material);

#include "Material.h"

#endif // INTERSECTION_H
