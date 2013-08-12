#ifndef SCENEOBJECTVTABLE_H
#define SCENEOBJECTVTABLE_H

#include "SceneObjectVTableForwardDeclaration.h"
#include "Intersection.h"
#include "SceneObject.h"
#include "Ray.h"
#include "Photon.h"

struct SceneObjectVTableStruct {
	Intersection (*intersectRay)(const SceneObject sceneObject, const Ray ray);
	bool		 (*emitPhotons) (const SceneObject sceneObject, const int numPhotons, PhotonContainer *photons);
};

#endif // SCENEOBJECTVTABLE_H