#include "SceneObjectUnitPlane.h"
#include "randf.h"
#include <math.h>

const SceneObjectVTable sceneObjectUnitPlaneVTable = (SceneObjectVTable) {
	&sceneObjectUnitPlaneIntersectRay,
	&sceneObjectUnitPlaneEmitPhotons,
	&sceneObjectUnitPlaneRadiantFlux
};

SceneObjectUnitPlane makeSceneObjectUnitPlane (const Plane plane, const Material *material) {

	return (SceneObjectUnitPlane) {makeSceneObject(&sceneObjectUnitPlaneVTable), plane, material};
}

defineAllocator(SceneObjectUnitPlane)

Intersection sceneObjectUnitPlaneIntersectRay(const SceneObject *superobject, const Ray ray) {

	const SceneObjectUnitPlane *object = (SceneObjectUnitPlane *) superobject;
	
	Intersection intersection = pIntersect(object->plane, ray);

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

		intersection.material = object->material;
	}

	return intersection;
}

bool sceneObjectUnitPlaneEmitPhotons(const SceneObject *superobject, const int numPhotons, PhotonContainer *photons) {

	if (!numPhotons) {
		return false;
	}
	
	const SceneObjectUnitPlane *object = (SceneObjectUnitPlane *) superobject;
	
	Color flux = sceneObjectUnitPlaneRadiantFlux(superobject);
	
	// See comments in SceneObjectSphere.

	// The U and V axis in the plane.
	Vector n = object->plane.normal;
	Vector U = makeVector(n.y, n.z, n.x);
	Vector V = makeVector(n.z, n.x, n.y);
	
	// Divide the photons onto a grid of n*m, most closely matching the wanted number.
	int numPhotonsU = ceil(sqrt(numPhotons/2.0))*2;
	int numPhotonsV = numPhotons/numPhotonsU;

	// How many photons in the last row, and how tall it is.
	int lastRowNumPhotonsU = numPhotons - numPhotonsU*numPhotonsV;
	float lastRowFactor = lastRowNumPhotonsU / (float) numPhotons;
	
	for (int iV = 0; iV < numPhotonsV; ++iV) {
		
		for (int iU = 0; iU < numPhotonsU; ++iU) {
			
			float u = (iU + randf()) / numPhotonsU;
			float v = (iV + randf()) / numPhotonsV * (1-lastRowFactor);
			
			Vector position = vAdd(vsMul(U, u*2-1), vsMul(V, v*2-1));
			
			photonContainerAddValue(photons, makePhoton(makeRay(position, vSampleHemisphere(object->plane.normal)), csMul(flux, 1.0 / numPhotons)));
		}
	}
	
	for (int iU = 0; iU < lastRowNumPhotonsU; ++iU) {
		
		float u = (iU + randf()) / lastRowNumPhotonsU;
		float v = (1 - lastRowFactor) + randf() * lastRowFactor;
		
		Vector position = vAdd(vsMul(U, u*2-1), vsMul(V, v*2-1));
		
		photonContainerAddValue(photons, makePhoton(makeRay(position, vSampleHemisphere(object->plane.normal)), csMul(flux, 1.0 / numPhotons)));
	}
	
	return true;
}

Color sceneObjectUnitPlaneRadiantFlux(const SceneObject *superobject) {
	
	const SceneObjectUnitPlane *object = (SceneObjectUnitPlane *) superobject;
	
	Color averageIrradiance = materialIrradience(object->material);
	
	// The plane is [-1, 1] in 2 dimensions.
	return csMul(averageIrradiance, 4);
}
