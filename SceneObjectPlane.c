#include "SceneObject.h"
#include "SceneObjectPlane.h"

const SceneObjectVTable sceneObjectPlaneVTable = (SceneObjectVTable) {
	&sceneObjectPlaneIntersectRay,
	&sceneObjectPlaneEmitPhoton
};

SceneObject makeSceneObjectPlane (const SceneObjectPlane plane, const Material *material) {

    return (SceneObject) {&sceneObjectPlaneVTable, material, plane};
}

Intersection sceneObjectPlaneIntersectRay(const SceneObject object, const Ray ray) {

    cIntersection intersection = pIntersects(object.plane, ray);

    if (intersection.hitType && material->isPerfectBlack) {

        intersection.hitType = perfectBlack;
    }

    intersection.material = object.material;

    return intersection;
}

bool sceneObjectPlaneEmitPhotons(const SceneObject object, const int numPhotons, Photon photons[]) {

    // You can't really emit a finite number of photons from an infinite plane.

	return false;
}











