#include <stdio.h>
#include "minunit.h"
#include "Vector.h"
#include "Plane.h"
#include "Sphere.h"

int tests_run = 0;

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

// static char * test_vRotated() {

//	 Vector x = makeVector(1, 0, 0);
//	 Vector y = makeVector(0, 1, 0);
//	 Vector z = makeVector(0, 0, 1);

//	 Vector r = vRotated(x, y, PI/2);

//	 mu_assert("vNormalized", vEqual(r, z));

//	 return 0;
// }

static char * test_vReflected() {

	Vector v1 = makeVector(1,-1, 0);
	Vector n  = makeVector(0, 1, 0);
	Vector v2 = makeVector(1, 1, 0);

	Vector r = vReflected(v1, n);

	mu_assert("test_vReflected", vEqual(r, v2));

	return 0;
}





static char * test_pIntersects() {

	Plane p = makePlane(makeVector(1, 0, 0), 0);

	mu_assert("test_pIntersects a", pIntersect(p, makeRay(makeVector( 1, 0, 0), makeVector(-1, 0, 0))).hitType != missed);
	mu_assert("test_pIntersects a", pIntersect(p, makeRay(makeVector( 1, 0, 0), makeVector( 1, 0, 0))).hitType == missed);
	mu_assert("test_pIntersects a", pIntersect(p, makeRay(makeVector(-1, 0, 0), makeVector(-1, 0, 0))).hitType == missed);

	return 0;
}

static char * test_pIsInside() {
	
	Plane p = makePlane(makeVector(1, 0, 0), 0);
	
	mu_assert("test_pIsInside a",  pIsInside(p, makeVector(-1, 0, 0)));
	mu_assert("test_pIsInside b", !pIsInside(p, makeVector(1, 0, 0)));
	
	return 0;
}





static char * test_sIntersect() {
	
	Sphere s = makeSphere(makeVector(0, 0, 0), 1);
	
	Ray hit  = makeRay(makeVector(2, 0, 0), makeVector(-1, 0, 0));
	Ray miss = makeRay(makeVector(2, 0, 0), makeVector(1, 0, 0));
	
	mu_assert("sIntersect a", sIntersect(s,  hit).hitType != missed);
	mu_assert("sIntersect b", sIntersect(s, miss).hitType == missed);
	
	return 0;
}



static char * all_tests() {

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
	// mu_run_test(test_vRotated);
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