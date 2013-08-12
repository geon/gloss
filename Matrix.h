#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
#include "Ray.h"

typedef struct {
    float values[4][4];
} Matrix;

Matrix makeMatrixZero();
Matrix makeMatrixIdentity();
Matrix makeMatrixTranslation(Vector);
Matrix makeMatrixAxisAngle(Vector axis, float angle);

Matrix mMul(const Matrix a, const Matrix b);

Vector mvMul   (const Matrix Matrix, const Vector Vector);
Vector mvMulDir(const Matrix Matrix, const Vector Vector);

Ray mrMul(const Matrix matrix, const Ray ray);

Matrix mInversed();

#endif // MATRIX_H
