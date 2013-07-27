#ifndef SCENEOBJECTVTABLE_H
#define SCENEOBJECTVTABLE_H

#include "Intersection.h"
#include "SceneObject.h"
#include "Ray.h"
#include "Photon.h"

typedef struct {
    Intersection (*intersectRay)(const SceneObject sceneObject, const Ray ray);
    Photon*      (*emitPhotons) (const SceneObject sceneObject, int numPhotons);
} SceneObjectVTable;

#endif // SCENEOBJECTVTABLE_H