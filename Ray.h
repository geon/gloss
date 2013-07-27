#ifndef _Ray_
#define _Ray_

#include "Vector.h"

typedef struct {
	Vector origin, direction;
} Ray;

Ray makeRay(Vector origin, Vector direction);

#endif
