#ifndef _Vector_
#define _Vector_

#include "bool.h"

extern const float vEpsilon;

typedef struct {
	float x, y, z;
} Vector;

Vector makeVector(const float x, const float y, const float z);
Vector makeVectorOrigo(void);

bool vEqual(const Vector a, const Vector b);

Vector vAdd (const Vector a, const Vector b);
Vector vSub (const Vector a, const Vector b);
Vector vsMul(const Vector v, const float s);
Vector vsDiv(const Vector v, const float s);

float  vDot  (const Vector a, const Vector b);
Vector vCross(const Vector a, const Vector b);

float  vLength       (const Vector v);
float  vLengthSquared(const Vector v);
Vector vNormalized   (const Vector v);
Vector vNegated      (const Vector v);

Vector vRotated(const Vector v, const Vector axis, const float angle);
Vector vReflected(const Vector v, const Vector normal);
Vector vSampleHemisphere(const Vector normal);


#endif
