#include "SceneObject.h"
#include "SceneObjectVTable.h"

Intersection sceneObjectIntersectRay(const SceneObject object, const Ray ray) {

	return ((SceneObjectVTable *)object.vTable)->intersectRay(object, ray);
}

bool sceneObjectEmitPhotons(const SceneObject object, const int numPhotons, PhotonContainer *photons) {

	return ((SceneObjectVTable *)object.vTable)->emitPhotons(object, numPhotons, photons);
}