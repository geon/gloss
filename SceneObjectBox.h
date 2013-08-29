#ifndef SCENEOBJECTBOX_H
#define SCENEOBJECTBOX_H

#include "SceneObject.h"
#include "Box.h"
#include "Material.h"
#include "Intersection.h"
#include "Ray.h"
#include "Photon.h"
#include "PhotonContainer.h"
#include "allocator.h"


typedef struct {
	const SceneObject parent;
	Box box;
	const Material *material;
} SceneObjectBox;


SceneObjectBox makeSceneObjectBox (const Vector size, const Material *material);

declareAllocator(SceneObjectBox)

Intersection sceneObjectBoxIntersectRay(const SceneObject *object, const Ray ray);
bool sceneObjectBoxEmitPhotons(const SceneObject *object, const int numPhotons, PhotonContainer *photons);
Color sceneObjectBoxRadiantFlux(const SceneObject *object);

extern const SceneObjectVTable sceneObjectBoxVTable;


#endif // SCENEOBJECTBOX_H