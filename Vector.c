#include "Vector.h"
#include "Matrix.h"
#include "randf.h"
#include "pi.h"
#include <math.h>

const float vEpsilon = 1/10000.0;

Vector makeVector(float x, float y, float z) {

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

Vector vRotated(const Vector v, const Vector axis, float angle) {

	return mvMul(makeMatrixAxisAngle(axis, angle), v);
}

Vector vReflected(const Vector v, const Vector normal) {

	return vSub(v, vsMul(vsMul(normal, vDot(v, normal)), 2));
}

 Vector vSampleHemisphere(const Vector normal) {

	 Vector tangent = vNormalized(vCross(normal, makeVector(randf(), randf(), randf())));

	 return vRotated(
		 vRotated(normal, tangent, acosf(sqrtf(randf()))),
		 normal,
		 randf() * 2*PI
	 );
 }
