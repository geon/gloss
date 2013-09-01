#include "SceneObjectTransform.h"

const SceneObjectVTable sceneObjectTransformVTable = (SceneObjectVTable) {
	&sceneObjectTransformIntersectRay,
	&sceneObjectTransformEmitPhotons,
	&sceneObjectTransformRadiantFlux
};

SceneObjectTransform makeSceneObjectTransform (const Matrix transform, const SceneObject *subject) {
	
	return (SceneObjectTransform) {makeSceneObject(&sceneObjectTransformVTable), subject, transform, mInversed(transform)};
}

defineAllocator(SceneObjectTransform)

Intersection sceneObjectTransformIntersectRay(const SceneObject *superobject, const Ray ray) {

	const SceneObjectTransform *object = (SceneObjectTransform *) superobject;

	// Inverse-transform the ray, so the subject appears transformed.
	Ray transformedRay = mrMul(object->inversedTransform, ray);
	
	// Forward the actual intersecting.
	Intersection intersection = sceneObjectIntersectRay(object->subject, transformedRay);
	
	// Transform the intersecion back into world space.
	if (intersection.hitType) {
		
		intersection.normal   = mvMulDir(object->transform, intersection.normal  );
		intersection.position = mvMul   (object->transform, intersection.position);
		intersection.distance = intersection.distance * mScale(object->transform);
	}
	
	return intersection;
}

bool sceneObjectTransformEmitPhotons(const SceneObject *superobject, const int numPhotons, PhotonContainer *photons) {

	const SceneObjectTransform *object = (SceneObjectTransform *) superobject;

	// Save the new, local photons for processing.
	PhotonContainer newPhotons = makePhotonContainer(numPhotons);

	// Forward the actual creation of photons.
	bool success = sceneObjectEmitPhotons(object->subject, numPhotons, &newPhotons);

	if (success) {

		// Transform the photons.
		float scale = mScale(object->transform);
		float areaScale = scale * scale;
		containerForeach(Photon, photon, newPhotons) {
			
			photon->heading = mrMul(object->transform, photon->heading);
			photon->energy = csMul(photon->energy, areaScale);
		}
		
		// "Return" the new, transformed photons.
		photonContainerAddValues(photons, newPhotons);
	}

	// Done with the local photons.
	photonContainerDestroy(&newPhotons);
	
	return success;
}

Color sceneObjectTransformRadiantFlux(const SceneObject *superobject) {
	
	const SceneObjectTransform *object = (SceneObjectTransform *) superobject;
	
	// Forward the flux calculation.
	Color radiantFlux = sceneObjectRadiantFlux(object->subject);
	
	float scale = mScale(object->transform);
	
	return csMul(radiantFlux, scale * scale);
}
