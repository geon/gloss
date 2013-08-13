#include <stdio.h>
#include "minunit.h"
#include "Vector.h"
#include "Plane.h"
#include "Sphere.h"
#include "pi.h"
#include "Matrix.h"


int tests_run = 0;




static char * test_mEqual() {
	
	Matrix a = makeMatrixZero();
	Matrix b = makeMatrixIdentity();
	
	mu_assert("mEqual a",  mEqual(a, a));
	mu_assert("mEqual b", !mEqual(a, b));
	
	return 0;
}

static char * test_mMul() {
	
	Matrix a = makeMatrixTranslation(makeVector(1, 2, 3));
	Matrix b = makeMatrixIdentity();
	
	mu_assert("mMul",  mEqual(a, mMul(b, a)));
	
	return 0;
}

static char * test_mvMul() {
	
	Vector v = makeVector(1, 2, 3);
	Matrix m = makeMatrixTranslation(v);
	
	mu_assert("mvMul", vEqual(vsMul(v, 2), mvMul(m, v)));
	
	return 0;
}

static char * test_mvMulDir() {
	
	Vector v  = makeVector(1, 2, 3);
	Matrix m1 = makeMatrixTranslation(v);
	Matrix m2 = makeMatrixAxisAngle(makeVector(1, 0, 0), PI);
	
	mu_assert("mvMulDir a",  vEqual(v, mvMulDir(m1, v)));
	mu_assert("mvMulDir b", !vEqual(v, mvMulDir(m2, v)));
	mu_assert("mvMulDir c",  vEqual(makeVector(1, -2, -3), mvMulDir(m2, v)));
	
	return 0;
}

static char * test_mInversed() {
	
	Matrix m = mMul(
		makeMatrixTranslation(makeVector(1, 2, 3)),
		makeMatrixAxisAngle(makeVector(4, 5, 6), 7)
	);
	
	mu_assert("mInversed",  mEqual(mMul(m, mInversed(m)), makeMatrixIdentity()));
	
	return 0;
}





static char * test_makeVector() {

	float x = 1, y = 2, z = 3;
	Vector v = makeVector(x, y, z);
	mu_assert("makeVector", v.x == x && v.y == y && v.z == z);

	return 0;
}

static char * test_vEqual() {

	Vector a   = makeVector(1+6, 2+5, 3+4);
	Vector b   = makeVector( 7,   7,   7 );

	mu_assert("makeVector", vEqual(a, b));

	return 0;
}

static char * test_vAdd() {

	Vector a   = makeVector(1, 2, 3);
	Vector b   = makeVector(6, 5, 4);
	Vector sum = makeVector(7, 7, 7);

	Vector v = vAdd(a, b);

	mu_assert("vAdd", vEqual(v, sum));

	return 0;
}

static char * test_vSub() {

	Vector a	= makeVector(7, 7, 7);
	Vector b	= makeVector(6, 5, 4);
	Vector diff = makeVector(1, 2, 3);

	Vector v = vSub(a, b);

	mu_assert("vSub", vEqual(v, diff));

	return 0;
}

static char * test_vsMul() {

	Vector a = makeVector( 1,  2,  3);
	Vector s = makeVector(10, 20, 30);

	Vector v = vsMul(a, 10);

	mu_assert("vsMul", vEqual(v, s));

	return 0;
}

static char * test_vsDiv() {

	Vector a = makeVector(10, 20, 30);
	Vector s = makeVector( 1,  2,  3);

	Vector v = vsDiv(a, 10);

	mu_assert("vsDiv", vEqual(v, s));

	return 0;
}

static char * test_vDot() {

	Vector a = makeVector(1, 0, 0);
	Vector b = makeVector(10,  20,  30);

	mu_assert("vDot", vDot(a, b) == 10);

	return 0;
}

static char * test_vCross() {

	Vector x = makeVector(1, 0, 0);
	Vector y = makeVector(0, 1, 0);
	Vector z = makeVector(0, 0, 1);

	Vector c = vCross(x, y);

	mu_assert("vCross", vEqual(c, z));

	return 0;
}

static char * test_vLength() {

	Vector v = makeVector(3, 4, 0);

	mu_assert("vLength", vLength(v) == 5);

	return 0;
}

static char * test_vNormalized() {

	Vector a = makeVector(13,	  17,	  23	 );
	Vector b = makeVector(13 * 10, 17 * 10, 23 * 10);

	mu_assert("vNormalized", vEqual(vNormalized(a), vNormalized(b)));

	return 0;
}

static char * test_vRotated() {

	Vector x = makeVector(1, 0, 0);
	Vector y = makeVector(0, 1, 0);
	Vector z = makeVector(0, 0, 1);

	Vector r1 = vRotated(x, y, -PI/2);
	Vector r2 = mvMul(makeMatrixAxisAngle(y, -PI/2), x);
	
	mu_assert("vRotated a", vEqual(r1, z));
	mu_assert("vRotated b", vEqual(r2, z));

	return 0;
}

static char * test_vReflected() {

	Vector v1 = makeVector(1,-1, 0);
	Vector n  = makeVector(0, 1, 0);
	Vector v2 = makeVector(1, 1, 0);

	Vector r = vReflected(v1, n);

	mu_assert("vReflected", vEqual(r, v2));

	return 0;
}





static char * test_pIntersects() {

	Plane p = makePlane(makeVector(1, 0, 0), 0);

	mu_assert("pIntersect a", pIntersect(p, makeRay(makeVector( 1, 0, 0), makeVector(-1, 0, 0))).hitType != missed);
	mu_assert("pIntersect b", pIntersect(p, makeRay(makeVector( 1, 0, 0), makeVector( 1, 0, 0))).hitType == missed);
	mu_assert("pIntersect c", pIntersect(p, makeRay(makeVector(-1, 0, 0), makeVector(-1, 0, 0))).hitType == missed);

	Plane p2 = makePlane(makeVector(0, 1, 0), 0);

	mu_assert("pIntersect d", pIntersect(p2, makeRay(makeVector(0, 1, 0), vNormalized(makeVector(-1, -0.0001, 0)))).hitType != missed);
	mu_assert("pIntersect e", pIntersect(p2, makeRay(makeVector(0, 1, 0), vNormalized(makeVector(-1,  0.0001, 0)))).hitType == missed);

	return 0;
}

static char * test_pIsInside() {
	
	Plane p = makePlane(makeVector(1, 0, 0), 0);
	
	mu_assert("pIsInside a",  pIsInside(p, makeVector(-1, 0, 0)));
	mu_assert("pIsInside b", !pIsInside(p, makeVector( 1, 0, 0)));
	
	return 0;
}





static char * test_sIntersect() {
	
	Sphere s = makeSphere(makeVector(0, 0, 0), 1);

	mu_assert("sIntersect a", sIntersect(s, makeRay(makeVector(2, 0, 0), makeVector(-1, 0, 0))).hitType != missed);
	mu_assert("sIntersect b", sIntersect(s, makeRay(makeVector(2, 0, 0), makeVector( 1, 0, 0))).hitType == missed);
	
	mu_assert("sIntersect c", sIntersect(s, makeRay(makeVector(2, 1, 0), vNormalized(makeVector(-1, -0.0001, 0)))).hitType != missed);
	mu_assert("sIntersect d", sIntersect(s, makeRay(makeVector(2, 1, 0), vNormalized(makeVector(-1,  0.0001, 0)))).hitType == missed);
	
	return 0;
}




static char * all_tests() {

	mu_run_test(test_mEqual);
	mu_run_test(test_mMul);
	mu_run_test(test_mvMul);
	mu_run_test(test_mvMulDir);
	mu_run_test(test_mInversed);

	mu_run_test(test_makeVector);
	mu_run_test(test_vEqual);
	mu_run_test(test_vAdd);
	mu_run_test(test_vSub);
	mu_run_test(test_vsMul);
	mu_run_test(test_vsDiv);
	mu_run_test(test_vDot);
	mu_run_test(test_vCross);
	mu_run_test(test_vLength);
	mu_run_test(test_vNormalized);
	mu_run_test(test_vRotated);
	mu_run_test(test_vReflected);

	mu_run_test(test_pIntersects);
	mu_run_test(test_pIsInside);
	
	mu_run_test(test_sIntersect);
	

	return 0;
}

int main(int argc, char **argv) {
	char *result = all_tests();
	if (result != 0) {
		printf("%s\n", result);
	}
	else {
		printf("ALL TESTS PASSED\n");
	}
	printf("Tests run: %d\n", tests_run);

	return result != 0;
}