#include "SceneObjectPlane.h"

const SceneObjectVTable sceneObjectPlaneVTable = (SceneObjectVTable) {
	&sceneObjectPlaneIntersectRay,
	&sceneObjectPlaneEmitPhotons
};

SceneObject makeSceneObjectPlane (const Plane plane, const Matrix transform, const Material *material) {
	
	return (SceneObject) {&sceneObjectPlaneVTable, material, transform, mInversed(transform), {.plane = plane}};
}

Intersection sceneObjectPlaneIntersectRay(const SceneObject object, const Ray ray) {

	Intersection intersection = pIntersect(object.plane, mrMul(object.inversedTransform, ray));

	if (intersection.hitType) {
		
		intersection.normal   = mvMulDir(object.transform, intersection.normal  );
		intersection.position = mvMul   (object.transform, intersection.position);

		if (intersection.material->isPerfectBlack) {
			intersection.hitType = perfectBlack;
		}
	}

	intersection.material = object.material;
	
	return intersection;
}

bool sceneObjectPlaneEmitPhotons(const SceneObject object, const int numPhotons, PhotonContainer *photons) {

	// You can't really emit a finite number of photons from an infinite plane.

	return false;
}











