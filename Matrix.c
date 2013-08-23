#include "Matrix.h"
#include <math.h>

__attribute__((const))
Matrix makeMatrixZero() {

	return (Matrix) {{
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
	}};
}

__attribute__((const))
Matrix makeMatrixIdentity() {

	return (Matrix) {{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	}};
}

__attribute__((const))
Matrix makeMatrixTranslation(const Vector v) {

	return (Matrix) {{
		{1, 0, 0, v.x},
		{0, 1, 0, v.y},
		{0, 0, 1, v.z},
		{0, 0, 0, 1},
	}};
}

__attribute__((const))
Matrix makeMatrixAxisAngle(const Vector axis, const float angle) {
	float length = vLength(axis);
	if(length < 0.0005){

		return makeMatrixIdentity();
	}

	// Normalizing, but reusing the length.
	Vector axisN = vsDiv(axis, length);

	float Sin = sin(-angle);
	float Cos = cos(-angle);

	Matrix matrix = makeMatrixIdentity();
	matrix.values[0][0] = Cos + axisN.x*axisN.x*(1-Cos);
	matrix.values[1][0] = axisN.x*axisN.y*(1-Cos) - axisN.z*Sin;
	matrix.values[2][0] = axisN.y*Sin + axisN.x*axisN.z*(1-Cos);
	matrix.values[0][1] = axisN.z*Sin + axisN.x*axisN.y*(1-Cos);
	matrix.values[1][1] = Cos + axisN.y*axisN.y*(1-Cos);
	matrix.values[2][1] = -axisN.x*Sin + axisN.y*axisN.z*(1-Cos);
	matrix.values[0][2] = -axisN.y*Sin + axisN.x*axisN.z*(1-Cos);
	matrix.values[1][2] = axisN.x*Sin + axisN.y*axisN.z*(1-Cos);
	matrix.values[2][2] = Cos + axisN.z*axisN.z*(1-Cos);

	return matrix;
}

__attribute__((const))
bool mEqual(const Matrix a, const Matrix b) {
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			if (a.values[i][j] + vEpsilon < b.values[i][j] || b.values[i][j] + vEpsilon < a.values[i][j])
				return false;

	return true;
}

__attribute__((pure))
Matrix mMul(const Matrix *a, const Matrix *b) {

	Matrix matrix = makeMatrixZero();

	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			for (int k=0; k<4; k++)
				matrix.values[i][j] += a->values[i][k] * b->values[k][j];

	return matrix;
}

__attribute__((pure))
Vector mvMul(const Matrix *matrix, const Vector *vector) {

	Vector newVector = mvMulDir(matrix, vector);

	// Access x,y,z as an array.
	float* newVectorValues = (float*)&newVector.x;

	for (int i=0; i<3; i++)
		newVectorValues[i] += matrix->values[i][3];

	return newVector;
}

__attribute__((pure))
Vector mvMulDir(const Matrix *matrix, const Vector *vector) {

	Vector newVector = makeVector(0, 0, 0);

	// Access x,y,z as an array.
	float* vectorValues	= (float*)&vector->x;
	float* newVectorValues = (float*)&newVector.x;

	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			newVectorValues[i] +=  matrix->values[i][j] * vectorValues[j];

	return newVector;
}

__attribute__((pure))
Ray mrMul(const Matrix *matrix, const Ray *ray) {

	return makeRay(mvMul(matrix, &ray->origin),
			vNormalized(mvMulDir(matrix, &ray->direction)));
}

__attribute__((pure))
Matrix mInversed(const Matrix *matrix) {
	float* m = (float*)&matrix->values[0][0];
	Matrix returnValue;
	float* out = &returnValue.values[0][0];

/* NB. OpenGL Matrices are COLUMN major. */
#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float *r0, *r1, *r2, *r3;

	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

	r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
	r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
	r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
	r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
	r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
	r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
	r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
	r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
	r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
	r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
	r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
	r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

	/* choose pivot - or die */
	if (fabs(r3[0]) > fabs(r2[0]))
		SWAP_ROWS(r3, r2);
	if (fabs(r2[0]) > fabs(r1[0]))
		SWAP_ROWS(r2, r1);
	if (fabs(r1[0]) > fabs(r0[0]))
		SWAP_ROWS(r1, r0);
	if (0.0 == r0[0])
		return makeMatrixZero();

	/* eliminate first variable	 */
	m1 = r1[0] / r0[0];
	m2 = r2[0] / r0[0];
	m3 = r3[0] / r0[0];
	s = r0[1];
	r1[1] -= m1 * s;
	r2[1] -= m2 * s;
	r3[1] -= m3 * s;
	s = r0[2];
	r1[2] -= m1 * s;
	r2[2] -= m2 * s;
	r3[2] -= m3 * s;
	s = r0[3];
	r1[3] -= m1 * s;
	r2[3] -= m2 * s;
	r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) {
		r1[4] -= m1 * s;
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r0[5];
	if (s != 0.0) {
		r1[5] -= m1 * s;
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r0[6];
	if (s != 0.0) {
		r1[6] -= m1 * s;
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r0[7];
	if (s != 0.0) {
		r1[7] -= m1 * s;
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}

	/* choose pivot - or die */
	if (fabs(r3[1]) > fabs(r2[1]))
		SWAP_ROWS(r3, r2);
	if (fabs(r2[1]) > fabs(r1[1]))
		SWAP_ROWS(r2, r1);
	if (0.0 == r1[1])
		return makeMatrixZero();

	/* eliminate second variable */
	m2 = r2[1] / r1[1];
	m3 = r3[1] / r1[1];
	r2[2] -= m2 * r1[2];
	r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3];
	r3[3] -= m3 * r1[3];
	s = r1[4];
	if (0.0 != s) {
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r1[5];
	if (0.0 != s) {
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r1[6];
	if (0.0 != s) {
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r1[7];
	if (0.0 != s) {
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}

	/* choose pivot - or die */
	if (fabs(r3[2]) > fabs(r2[2]))
		SWAP_ROWS(r3, r2);
	if (0.0 == r2[2])
		return makeMatrixZero();

	/* eliminate third variable */
	m3 = r3[2] / r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
		r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];

	/* last check */
	if (0.0 == r3[3])
		return makeMatrixZero();

	s = 1.0 / r3[3];   /* now back substitute row 3 */
	r3[4] *= s;
	r3[5] *= s;
	r3[6] *= s;
	r3[7] *= s;

	m2 = r2[3];	  /* now back substitute row 2 */
	s = 1.0 / r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
		r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
		r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
		r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

	m1 = r1[2];	  /* now back substitute row 1 */
	s = 1.0 / r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
		r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
		r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

	m0 = r0[1];	  /* now back substitute row 0 */
	s = 1.0 / r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
		r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

	MAT(out, 0, 0) = r0[4];
	MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
	MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
	MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
	MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
	MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
	MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
	MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
	MAT(out, 3, 3) = r3[7];

#undef MAT
#undef SWAP_ROWS

  return returnValue;
}
