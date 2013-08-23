#include "SceneObjectUnitPlane.h"
#include "randf.h"
#include <math.h>

const SceneObjectVTable sceneObjectUnitPlaneVTable = {
	&sceneObjectUnitPlaneIntersectRay,
	&sceneObjectUnitPlaneEmitPhotons
};

SceneObject makeSceneObjectUnitPlane (const Plane plane, const Matrix transform, const Material *material) {

	return (SceneObject) {&sceneObjectUnitPlaneVTable, material, transform, mInversed(transform), {.plane = plane}};
}

Intersection sceneObjectUnitPlaneIntersectRay(const SceneObject object, const Ray ray) {

	Intersection intersection = pIntersect(object.plane, mrMul(object.inversedTransform, ray));

	if (intersection.hitType) {

		// Intersection outside the unit cube should be ignored.
		if (
			intersection.position.x < -1 || intersection.position.x > 1 ||
			intersection.position.y < -1 || intersection.position.y > 1 ||
			intersection.position.z < -1 || intersection.position.z > 1
		) {
			
			intersection.hitType = missed;
			
			// Return early.
			return intersection;
		}

		intersection.normal   = mvMulDir(object.transform, intersection.normal  );
		intersection.position = mvMul   (object.transform, intersection.position);
		intersection.material = object.material;
		
		if (intersection.material->isPerfectBlack) {
			intersection.hitType = perfectBlack;
		}
	}

	return intersection;
}

bool sceneObjectUnitPlaneEmitPhotons(const SceneObject object, const int numPhotons, PhotonContainer *photons) {

	const Vector normal = object.plane.normal;
	
	Vector u = makeVector(normal.y, normal.z, normal.x);
	Vector v = makeVector(normal.z, normal.x, normal.y);

	int numU = ceil(sqrtf(numPhotons));
	int numV = sqrtf(numPhotons);

	for (int iU = 0; iU < numU; ++iU) {

		for (int iV = 0; iV < numV; ++iV) {

			int currentPhoton = iU*numV + iV;

			if (currentPhoton >= numPhotons) {
				break;
			}

			Vector position = vAdd(vAdd(
				vsMul(u, ((iU + randf())/numU * 2)-1),
				vsMul(v, ((iV + randf())/numV * 2)-1)
			), vsMul(normal, vEpsilon));

			photonContainerAddValue(photons, makePhoton(makeRay(position, vSampleHemisphere(normal)), csMul(object.material->radience, 1.0 / numPhotons)));
		}
	}

	return true;
}
