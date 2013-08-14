#include "SceneObjectSphere.h"

const SceneObjectVTable sceneObjectSphereVTable = (SceneObjectVTable) {
	&sceneObjectSphereIntersectRay,
	&sceneObjectSphereEmitPhotons
};

SceneObject makeSceneObjectSphere (const Sphere sphere, const Matrix transform, const Material *material) {

	return (SceneObject) {&sceneObjectSphereVTable, material, transform, mInversed(transform),  {.sphere = sphere}};
}

Intersection sceneObjectSphereIntersectRay(const SceneObject object, const Ray ray) {

	Intersection intersection = sIntersect(object.sphere, mrMul(object.inversedTransform, ray));

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

bool sceneObjectSphereEmitPhotons(const SceneObject object, const int numPhotons, PhotonContainer *photons) {

	// TODO: Implement stratified sampling.

	return false;

	// cVector position = shape.sampleSurface();
	// return cPhoton(cRay(position, position.Normalized().SampleHemisphere()), material->lightSourceIntensity() * (shape.surfaceArea()/numPhotons));
}
