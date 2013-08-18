#ifndef BOX_H
#define BOX_H

#include "Vector.h"
#include "Intersection.h"
#include "Ray.h"

typedef struct {
	Vector size;
} Box;

Box makeBox(const Vector size);

Intersection bIntersect(const Box box, const Ray ray);

#endif // BOX_H