#include "SceneObjectSphere.h"
#include <math.h>
#include "randf.h"
#include "pi.h"

const SceneObjectVTable sceneObjectSphereVTable = (SceneObjectVTable) {
	&sceneObjectSphereIntersectRay,
	&sceneObjectSphereEmitPhotons
};

SceneObject makeSceneObjectSphere (const Sphere sphere, const Matrix transform, const Material *material) {

	return (SceneObject) {&sceneObjectSphereVTable, material, transform, mInversed(transform),  {.sphere = sphere}};
}

Intersection sceneObjectSphereIntersectRay(const SceneObject object, const Ray ray) {

	Intersection intersection = sIntersect(object.sphere, mrMul(object.inversedTransform, ray));

	if (intersection.hitType) {
		
		intersection.normal   = mvMulDir(object.transform, intersection.normal  );
		intersection.position = mvMul   (object.transform, intersection.position);
		intersection.material = object.material;
		
		if (intersection.material->isPerfectBlack) {
			intersection.hitType = perfectBlack;
		}
	}

	return intersection;
}

bool sceneObjectSphereEmitPhotons(const SceneObject object, const int numPhotons, PhotonContainer *photons) {
	
	int numPhotonsU = ceil(sqrt(numPhotons));
	int numPhotonsV = sqrt(numPhotons);
	
	for (int iU = 0; iU < numPhotonsU; ++iU) {
		
		for (int iV = 0; iV < numPhotonsV; ++iV) {
			
			int currentPhoton = iU*numPhotonsV + iV;
			
			if (currentPhoton >= numPhotons) {
				break;
			}
			
			Vector normal = vRotated(
				vRotated(makeVector(object.sphere.radius, 0, 0), makeVector(0, 1, 0), acosf(((iU + randf())/numPhotonsU * 2)-1)),
				makeVector(1, 0, 0),
				((iV + randf())/numPhotonsV * 2)-1 * 2*PI
			);
			
			Vector position = vAdd(object.sphere.position, vsMul(normal, 1+vEpsilon));

			photonContainerAddValue(photons, makePhoton(mrMul(object.transform, makeRay(position, normal)), csMul(object.material->radience, 1.0 / numPhotons)));
		}
	}

	return true;
}
