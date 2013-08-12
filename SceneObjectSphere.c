#include "SceneObjectSphere.h"
#include "SceneObject.h"
#include "SceneObjectVTableForwardDeclaration.h"

const SceneObjectVTable sceneObjectSphereVTable = (SceneObjectVTable) {
	&sceneObjectSphereIntersectRay,
	&sceneObjectSphereEmitPhotons
};

SceneObject makeSceneObjectSphere (const Sphere sphere, const Material *material) {

	return (SceneObject) {&sceneObjectSphereVTable, material,  {.sphere = sphere}};
}

Intersection sceneObjectSphereIntersectRay(const SceneObject object, const Ray ray) {

	Intersection intersection = sIntersect(object.sphere, ray);

	if (intersection.hitType && intersection.material->isPerfectBlack) {

		intersection.hitType = perfectBlack;
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
