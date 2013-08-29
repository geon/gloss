#ifndef SCENEOBJECTPLANE_H
#define SCENEOBJECTPLANE_H

#include "SceneObject.h"
#include "Plane.h"
#include "Material.h"
#include "Intersection.h"
#include "Ray.h"
#include "Photon.h"
#include "PhotonContainer.h"
#include "SceneObjectPointerContainer.h"


typedef struct {
	const SceneObject parent;
	Plane plane;
	const Material *material;
} SceneObjectPlane;


SceneObjectPlane makeSceneObjectPlane(const Plane plane, const Material *material);

Intersection sceneObjectPlaneIntersectRay(const SceneObject *object, const Ray ray);
bool sceneObjectPlaneEmitPhotons(const SceneObject *object, const int numPhotons, PhotonContainer *photons);
Color sceneObjectPlaneRadiantFlux(const SceneObject *object);

extern const SceneObjectVTable sceneObjectPlaneVTable;


#endif // SCENEOBJECTPLANE_H