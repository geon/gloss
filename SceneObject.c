#include "SceneObject.h"


SceneObject makeSceneObject(const SceneObjectVTable *vTable) {
	
	return (SceneObject) {vTable};
}

Intersection sceneObjectIntersectRay(const SceneObject *object, const Ray ray) {

	return object->vTable->intersectRay(object, ray);
}

bool sceneObjectEmitPhotons(const SceneObject *object, const int numPhotons, PhotonContainer *photons) {

	return object->vTable->emitPhotons(object, numPhotons, photons);
}

float sceneObjectRadiantFlux(const SceneObject *object) {
	
	return object->vTable->sceneObjectRadiantFlux(object);
}
