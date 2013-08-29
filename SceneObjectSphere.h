#ifndef SCENEOBJECTSPHERE_H
#define SCENEOBJECTSPHERE_H

#include "SceneObject.h"
#include "Sphere.h"
#include "Material.h"
#include "Intersection.h"
#include "Ray.h"
#include "Photon.h"
#include "PhotonContainer.h"
#include "SceneObjectPointerContainer.h"
#include "allocator.h"


typedef struct {
	const SceneObject parent;
	Sphere sphere;
	const Material *material;
} SceneObjectSphere;


SceneObjectSphere makeSceneObjectSphere(const Sphere sphere, const Material *material);

declareAllocator(SceneObjectSphere)

Intersection sceneObjectSphereIntersectRay(const SceneObject *object, const Ray ray);
bool sceneObjectSphereEmitPhotons(const SceneObject *object, const int numPhotons, PhotonContainer *photons);
Color sceneObjectSphereRadiantFlux(const SceneObject *object);

extern const SceneObjectVTable sceneObjectSphereVTable;


#endif // SCENEOBJECTSPHERE_H