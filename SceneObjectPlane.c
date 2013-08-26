#include "SceneObjectPlane.h"

const SceneObjectVTable sceneObjectPlaneVTable = (SceneObjectVTable) {
	&sceneObjectPlaneIntersectRay,
	&sceneObjectPlaneEmitPhotons
};

SceneObjectPlane makeSceneObjectPlane (const Plane plane, const Material *material) {
	
	return (SceneObjectPlane) {makeSceneObject(&sceneObjectPlaneVTable), plane, material};
}

Intersection sceneObjectPlaneIntersectRay(const SceneObject *superobject, const Ray ray) {
	
	SceneObjectPlane *object = (SceneObjectPlane *) superobject;

	Intersection intersection = pIntersect(object->plane, ray);

	if (intersection.hitType) {
		
		intersection.material = object->material;

		if (intersection.material->isPerfectBlack) {
			intersection.hitType = perfectBlack;
		}
	}
	
	return intersection;
}

bool sceneObjectPlaneEmitPhotons(const SceneObject *superobject, const int numPhotons, PhotonContainer *photons) {

	// You can't really emit a finite number of photons from an infinite plane.

	return false;
}











