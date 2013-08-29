#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "SceneObjectVTableForwardDeclaration.h"
#include "Intersection.h"
#include "PhotonContainer.h"


typedef struct {
	const SceneObjectVTable *vTable;
} SceneObject;


SceneObject makeSceneObject(const SceneObjectVTable *vTable);

Intersection sceneObjectIntersectRay(const SceneObject *object, const Ray ray);
bool sceneObjectEmitPhotons(const SceneObject *object, const int numPhotons, PhotonContainer *photons);
float sceneObjectRadiantFlux(const SceneObject *object);

#include "SceneObjectVTable.h"

#endif // SCENEOBJECT_H
