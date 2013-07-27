#include "SceneObject.h"
#include "SceneObjectSphere.h"

const SceneObjectVTable sceneObjectSphereVTable = (SceneObjectVTable) {
	&sceneObjectSphereIntersectRay,
	&sceneObjectSphereEmitPhoton
};

SceneObject makeSceneObjectSphere (const SceneObjectSphere sphere, const Material *material) {

    return (SceneObject) {&sceneObjectSphereVTable, material, sphere};
}

Intersection sceneObjectSphereIntersectRay(const SceneObject object, const Ray ray) {

    cIntersection intersection = sIntersects(object.sphere, ray);

    if (intersection.hitType && material->isPerfectBlack {

        intersection.hitType = perfectBlack;
    }

    intersection.material = object.material;

    return intersection;
}

bool sceneObjectSphereEmitPhotons(const SceneObject object, const int numPhotons, Photon photons[]) {

	// TODO: Implement stratified sampling.

	return false;

    // cVector position = shape.sampleSurface();
    // return cPhoton(cRay(position, position.Normalized().SampleHemisphere()), material->lightSourceIntensity() * (shape.surfaceArea()/numPhotons));
}
