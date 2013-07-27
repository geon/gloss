#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Vector.h"
#include "Material.h"

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
    Material* material;
} Intersection;

Intersection makeIntersection(HitType hitType, Vector position, float distance, Vector normal , Material* material);

#endif // INTERSECTION_H
