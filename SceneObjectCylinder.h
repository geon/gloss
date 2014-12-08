#ifndef SCENEOBJECTCYLINDER_H
#define SCENEOBJECTCYLINDER_H

#include "SceneObject.h"
#include "Cylinder.h"
#include "Material.h"
#include "Intersection.h"
#include "Ray.h"
#include "Photon.h"
#include "PhotonContainer.h"
#include "allocator.h"


typedef struct {
	const SceneObject parent;
	Cylinder cylinder;
	const Material *material;
} SceneObjectCylinder;


SceneObjectCylinder makeSceneObjectCylinder (const Vector endA, const Vector endB, const float radius, const Material *material);

declareAllocator(SceneObjectCylinder)

Intersection sceneObjectCylinderIntersectRay(const SceneObject *object, const Ray ray);
bool sceneObjectCylinderEmitPhotons(const SceneObject *object, const int numPhotons, PhotonContainer *photons);
Color sceneObjectCylinderRadiantFlux(const SceneObject *object);

extern const SceneObjectVTable sceneObjectCylinderVTable;


#endif // SCENEOBJECTCYLINDER_H