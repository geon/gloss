#include "SceneObjectBox.h"
#include "randf.h"
#include <math.h>

const SceneObjectVTable sceneObjectBoxVTable = (SceneObjectVTable) {
	&sceneObjectBoxIntersectRay,
	&sceneObjectBoxEmitPhotons
};

SceneObject makeSceneObjectBox (const Vector size, const Matrix transform, const Material *material) {

	return (SceneObject) {&sceneObjectBoxVTable, material, transform, mInversed(transform), {.box = makeBox(size)}};
}

Intersection sceneObjectBoxIntersectRay(const SceneObject object, const Ray ray) {

	Intersection intersection = bIntersect(object.box, mrMul(object.inversedTransform, ray));

	if (intersection.hitType) {

		intersection.normal   = mvMulDir(object.transform, intersection.normal  );
		intersection.position = mvMul   (object.transform, intersection.position);
		intersection.material = object.material;
		
		if (intersection.material->isPerfectBlack) {
			intersection.hitType = perfectBlack;
		}
	}

	return intersection;
}

bool sceneObjectBoxEmitPhotons(const SceneObject object, const int numPhotons, PhotonContainer *photons) {


	// TODO: Implement this.
	
	return false;
}
