#include "SceneObjectCylinder.h"
#include "pi.h"
#include "randf.h"
#include <math.h>

const SceneObjectVTable sceneObjectCylinderVTable = (SceneObjectVTable) {
	&sceneObjectCylinderIntersectRay,
	&sceneObjectCylinderEmitPhotons,
	&sceneObjectCylinderRadiantFlux
};

SceneObjectCylinder makeSceneObjectCylinder (const Vector endA, const Vector endB, const float radius, const Material *material) {

	return (SceneObjectCylinder) {makeSceneObject(&sceneObjectCylinderVTable), makeCylinder(endA, endB, radius), material};
}

defineAllocator(SceneObjectCylinder)

Intersection sceneObjectCylinderIntersectRay(const SceneObject *superobject, const Ray ray) {

	const SceneObjectCylinder *object = (SceneObjectCylinder *) superobject;

	Intersection intersection = cIntersect(object->cylinder, ray);
	intersection.material = object->material;

	return intersection;
}

bool sceneObjectCylinderEmitPhotons(const SceneObject *superobject, const int numPhotons, PhotonContainer *photons) {

	// TODO: Implement this. Use code from SceneObjectSphere.
	
	return false;
}

Color sceneObjectCylinderRadiantFlux(const SceneObject *superobject) {
	
	const SceneObjectCylinder *object = (SceneObjectCylinder *) superobject;
	
	Color averageIrradiance = materialIrradience(object->material);

	return csMul(averageIrradiance, cSurfaceArea(object->cylinder));
}
