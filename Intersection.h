#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Vector.h"
#include "Material.h"

typedef enum {
    Missed,
    Surface,
    PerfectBlack,
    Origin
} HitType;

typedef struct {

    HitType hitType;
    Vector point;
    float distance;
    Vector normal;
    Material* material;
} Intersection;

Intersection makeIntersection(HitType hitType, Vector point, float distance, Vector normal , Material* material);

#endif // INTERSECTION_H
