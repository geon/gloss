#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
#include "Ray.h"

typedef struct {
	float values[4][4];
} Matrix;

__attribute__((const))
Matrix makeMatrixZero(void);
__attribute__((const))
Matrix makeMatrixIdentity(void);
__attribute__((const))
Matrix makeMatrixTranslation(const Vector v);
__attribute__((const))
Matrix makeMatrixAxisAngle(const Vector axis, const float angle);

__attribute__((const))
bool mEqual(const Matrix a, const Matrix b);

__attribute__((const))
Matrix mMul(const Matrix a, const Matrix b);

__attribute__((const))
Vector mvMul   (const Matrix Matrix, const Vector Vector);
__attribute__((const))
Vector mvMulDir(const Matrix Matrix, const Vector Vector);

__attribute__((const))
Ray mrMul(const Matrix matrix, const Ray ray);

__attribute__((const))
Matrix mInversed(const Matrix);

#endif // MATRIX_H
