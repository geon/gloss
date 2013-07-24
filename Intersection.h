#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Vector.h"

// typedef struct Material;

typedef enum {
    Missed,
    Surface,
    PerfectBlack,
    Origin
} HitType;

typedef struct {

    HitType hitType;
    cVector point;
    float distance;
    cVector normal;
    // Material* material;
} Intersection;

makeIntersection(HitType hitType, cVector point, float distance, cVector normal /*, Material* material */);

#endif // INTERSECTION_H
