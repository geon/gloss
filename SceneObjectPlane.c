#include "SceneObjectPlane.h"

const SceneObjectVTable sceneObjectPlaneVTable = (SceneObjectVTable) {
	&sceneObjectPlaneIntersectRay,
	&sceneObjectPlaneEmitPhotons,
	&sceneObjectPlaneRadiantFlux
};

SceneObjectPlane makeSceneObjectPlane (const Plane plane, const Material *material) {
	
	return (SceneObjectPlane) {makeSceneObject(&sceneObjectPlaneVTable), plane, material};
}

Intersection sceneObjectPlaneIntersectRay(const SceneObject *superobject, const Ray ray) {
	
	SceneObjectPlane *object = (SceneObjectPlane *) superobject;

	Intersection intersection = pIntersect(object->plane, ray);
	intersection.material = object->material;

	return intersection;
}

bool sceneObjectPlaneEmitPhotons(const SceneObject *superobject, const int numPhotons, PhotonContainer *photons) {

	// You can't really emit a finite number of photons from an infinite plane.

	return false;
}

float sceneObjectPlaneRadiantFlux(const SceneObject *object) {
	
	// Since a plane can't emit photons, it shouldn't have any flux.
	
	return 0;
}
