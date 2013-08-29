#include "SceneObjectSphere.h"
#include <math.h>
#include "randf.h"
#include "pi.h"
#include <stdio.h>

const SceneObjectVTable sceneObjectSphereVTable = (SceneObjectVTable) {
	&sceneObjectSphereIntersectRay,
	&sceneObjectSphereEmitPhotons,
	&sceneObjectSphereRadiantFlux
};

SceneObjectSphere makeSceneObjectSphere (const Sphere sphere, const Material *material) {

	return (SceneObjectSphere) {makeSceneObject(&sceneObjectSphereVTable), sphere, material};
}

defineAllocator(SceneObjectSphere)

Intersection sceneObjectSphereIntersectRay(const SceneObject *superobject, const Ray ray) {

	const SceneObjectSphere *object = (SceneObjectSphere *) superobject;

	Intersection intersection = sIntersect(object->sphere, ray);
	intersection.material = object->material;

	return intersection;
}

bool sceneObjectSphereEmitPhotons(const SceneObject *superobject, const int numPhotons, PhotonContainer *photons) {
	
	const SceneObjectSphere *object = (SceneObjectSphere *) superobject;
	
	Color flux = sceneObjectSphereRadiantFlux(superobject);
	
	/*

	 I use stratified sampling to reduce noise. It means I divide the surface into a grid, then
	 sample randomly within each cell. This creates a more even spread without any clustering.
	 
	 But if numPhotons isn't a power of 2, a simple n*n grid can't be used. I want to spawn the exact
	 number of photons requested, not just an approximation. To still spread the photons evenly,
	 the last row needs to be thinner than the others, like this:
	 	 
	 +----+----+----+----+
	 | .  |    |   .|  . |
	 |    | .  |    |    |
	 +----+----+----+----+
	 |    |    | .  |    |
	 | .  | .  |    |  . |
	 +----+----+----+----+
	 |    | .  |    |.   |
	 |   .|    |.   |    |
	 +----+-+--+--+-+----+
	 |   .  |.    |    . |
	 +------+-----+------+

	 (Actually, for this sphere I use a n*2n grid, but still.)

	 */

	// Divide the photons onto a grid of n*m, most closely matching the wanted number.
	int numPhotonsU = ceil(sqrt(numPhotons/2.0))*2;
	int numPhotonsV = numPhotons/numPhotonsU;
	
	// How many photons in the last row, and how tall it is.
	int lastRowNumPhotonsU = numPhotons - numPhotonsU*numPhotonsV;
	float lastRowFactor = lastRowNumPhotonsU / (float) numPhotons;
	
	
//	printf("numPhotonsU: %i\n", numPhotonsU);
//	printf("numPhotonsV: %i\n", numPhotonsV);
//	printf("lastRowNumPhotonsU: %i\n", lastRowNumPhotonsU);
//	printf("lastRowFactor: %f\n", lastRowFactor);

	
	for (int iV = 0; iV < numPhotonsV; ++iV) {

		for (int iU = 0; iU < numPhotonsU; ++iU) {
		
			float u = (iU + randf()) / numPhotonsU;
			float v = (iV + randf()) / numPhotonsV * (1-lastRowFactor);
			
			Vector normal = vRotated(
				vRotated(makeVector(object->sphere.radius, 0, 0), makeVector(0, 1, 0), acosf(v*2 - 1)),
				makeVector(1, 0, 0),
				u * 2*PI
			);
			
			Vector position = vAdd(object->sphere.position, vsMul(normal, 1+vEpsilon));

			photonContainerAddValue(photons, makePhoton(makeRay(position, vSampleHemisphere(normal)), csMul(flux, 1.0 / numPhotons)));
		}
	}

	for (int iU = 0; iU < lastRowNumPhotonsU; ++iU) {
		
		float u = (iU + randf()) / lastRowNumPhotonsU;
		float v = (1 - lastRowFactor) + randf() * lastRowFactor;
		
		Vector normal = vRotated(
			vRotated(makeVector(object->sphere.radius, 0, 0), makeVector(0, 1, 0), acosf(v*2 - 1)),
			makeVector(1, 0, 0),
			u * 2*PI
		);
		
		Vector position = vAdd(object->sphere.position, vsMul(normal, 1+vEpsilon));
		
		photonContainerAddValue(photons, makePhoton(makeRay(position, vSampleHemisphere(normal)), csMul(materialIrradience(object->material), 1.0 / numPhotons)));
	}
	
	return true;
}

Color sceneObjectSphereRadiantFlux(const SceneObject *superobject) {
	
	const SceneObjectSphere *object = (SceneObjectSphere *) superobject;
	
	Color averageIrradiance = materialIrradience(object->material);

	float r = object->sphere.radius;
	float surfaceArea = 4 * PI * r*r;
	
	return csMul(averageIrradiance, surfaceArea);
}
