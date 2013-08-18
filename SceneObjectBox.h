#ifndef SCENEOBJECTBOX_H
#define SCENEOBJECTBOX_H

#include "SceneObject.h"
#include "Box.h"
#include "Material.h"
#include "Intersection.h"
#include "Ray.h"
#include "Photon.h"
#include "PhotonContainer.h"
#include "SceneObjectVTableForwardDeclaration.h"


SceneObject makeSceneObjectBox (const Vector size, const Matrix transform, const Material *material);

Intersection sceneObjectBoxIntersectRay(const SceneObject object, const Ray ray);
bool sceneObjectBoxEmitPhotons(const SceneObject object, const int numPhotons, PhotonContainer *photons);

extern const SceneObjectVTable sceneObjectBoxVTable;


#include "SceneObjectVTable.h"

#endif // SCENEOBJECTBOX_H