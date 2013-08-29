#include "SceneObjectBox.h"
#include "randf.h"
#include <math.h>

const SceneObjectVTable sceneObjectBoxVTable = (SceneObjectVTable) {
	&sceneObjectBoxIntersectRay,
	&sceneObjectBoxEmitPhotons
};

SceneObjectBox makeSceneObjectBox (const Vector size, const Material *material) {

	return (SceneObjectBox) {makeSceneObject(&sceneObjectBoxVTable), makeBox(size), material};
}

defineAllocator(SceneObjectBox)

Intersection sceneObjectBoxIntersectRay(const SceneObject *superobject, const Ray ray) {

	const SceneObjectBox *object = (SceneObjectBox *) superobject;

	Intersection intersection = bIntersect(object->box, ray);
	intersection.material = object->material;

	return intersection;
}

bool sceneObjectBoxEmitPhotons(const SceneObject *superobject, const int numPhotons, PhotonContainer *photons) {

	// TODO: Implement this. Use code from SceneObjectSphere.
	
	return false;
}

float sceneObjectBoxRadiantFlux(const SceneObject *superobject) {
	
	const SceneObjectBox *object = (SceneObjectBox *) superobject;
	
	Color averageIrradiance = materialIrradience(object->material);

	Vector size = object->box.size;
	float surfaceArea = (
		size.x * size.y +
		size.y * size.z +
		size.z * size.x
	) * 2;
	
	return surfaceArea * cBrightness(averageIrradiance);
}
