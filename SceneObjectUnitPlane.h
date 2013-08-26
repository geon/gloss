#ifndef SCENEOBJECTUNITPLANE_H
#define SCENEOBJECTUNITPLANE_H

#include "SceneObject.h"
#include "Plane.h"
#include "Material.h"
#include "Intersection.h"
#include "Ray.h"
#include "Photon.h"
#include "PhotonContainer.h"
#include "allocator.h"


typedef struct {
	const SceneObject parent;
	Plane plane;
	const Material *material;
} SceneObjectUnitPlane;


SceneObjectUnitPlane makeSceneObjectUnitPlane(const Plane plane, const Material *material);

declareAllocator(SceneObjectUnitPlane)

Intersection sceneObjectUnitPlaneIntersectRay(const SceneObject *object, const Ray ray);
bool sceneObjectUnitPlaneEmitPhotons(const SceneObject *object, const int numPhotons, PhotonContainer *photons);

extern const SceneObjectVTable sceneObjectPlaneVTable;


#endif // SCENEOBJECTUNITPLANE_H