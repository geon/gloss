#include "SceneObjectPlane.h"
#include "SceneObject.h"
#include "SceneObjectVTable.h"
#include "Material.h"

const SceneObjectVTable sceneObjectPlaneVTable = (SceneObjectVTable) {
	&sceneObjectPlaneIntersectRay,
	&sceneObjectPlaneEmitPhotons
};

SceneObject makeSceneObjectPlane (const Plane plane, const Material *material) {
	
    return (SceneObject) {&sceneObjectPlaneVTable, material, {.plane = plane}};
}

Intersection sceneObjectPlaneIntersectRay(const SceneObject object, const Ray ray) {

    Intersection intersection = pIntersect(object.plane, ray);

    if (intersection.hitType && intersection.material->isPerfectBlack) {

        intersection.hitType = perfectBlack;
    }

    intersection.material = object.material;

    return intersection;
}

bool sceneObjectPlaneEmitPhotons(const SceneObject object, const int numPhotons, PhotonContainer *photons) {

    // You can't really emit a finite number of photons from an infinite plane.

	return false;
}











