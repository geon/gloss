#include "Vector.h"
#include "Matrix.h"
#include "randf.h"
#include "pi.h"
#include <math.h>

const float vEpsilon = 1/10000.0;

Vector makeVector(const float x, const float y, const float z) {

	return (Vector) {x, y, z};
}

Vector makeVectorOrigo() {

	return (Vector) {0, 0, 0};
}

bool vEqual(const Vector a, const Vector b) {

	return
		a.x + vEpsilon > b.x && b.x + vEpsilon > a.x &&
		a.y + vEpsilon > b.y && b.y + vEpsilon > a.y &&
		a.z + vEpsilon > b.z && b.z + vEpsilon > a.z
	;
}

Vector vAdd(const Vector a, const Vector b) {

	return (Vector) {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z
	};
}

Vector vSub(const Vector a, const Vector b) {

	return (Vector) {
		a.x - b.x,
		a.y - b.y,
		a.z - b.z
	};
}

Vector vsMul(const Vector v, const float s) {

	return (Vector) {
		v.x * s,
		v.y * s,
		v.z * s
	};
}

Vector vsDiv(const Vector v, const float s) {

	return vsMul(v, 1/s);
}

float  vDot(const Vector a, const Vector b) {

	return
		a.x * b.x +
		a.y * b.y +
		a.z * b.z
	;
}

Vector vCross(const Vector a, const Vector b) {

	return (Vector) {
		a.y * b.z  -  a.z * b.y,
		a.z * b.x  -  a.x * b.z,
		a.x * b.y  -  a.y * b.x
	};
}

float  vLength(const Vector v) {
	
	return sqrtf(vLengthSquared(v));
}

float vLengthSquared(const Vector v) {

	return
		v.x * v.x +
		v.y * v.y +
		v.z * v.z
	;
}

Vector vNormalized(const Vector v) {

	return vsDiv(v, vLength(v));
}

Vector vNegated(const Vector v) {

	return (Vector) {
		-v.x,
		-v.y,
		-v.z
	};
}

Vector vTangent(const Vector v) {
	
	// Rotate the order of the coordinates, and negate when wrapping around.
	// This will create a tangent for any unit vector.
	// Apply again to the result for 2 tangents.
	return (Vector) {
		-v.z,
		 v.x,
		 v.y
	};
}

Vector vRotated(const Vector v, const Vector axis, const float angle) {

	return vAdd(vAdd(vsMul(v, cosf(angle)), vsMul(vCross(axis, v), sinf(angle))), vsMul(axis, vDot(axis, v) * (1-cosf(angle))));
}

Vector vReflected(const Vector v, const Vector normal) {

	return vSub(v, vsMul(vsMul(normal, vDot(v, normal)), 2));
}

 Vector vSampleHemisphere(const Vector normal) {

	 return vRotated(
		 vRotated(normal, vTangent(normal), acosf(sqrtf(randf()))),
		 normal,
		 randf() * 2*PI
	 );
 }
