#ifndef SCENEOBJECTUNITPLANE_H
#define SCENEOBJECTUNITPLANE_H

#include "SceneObject.h"
#include "Plane.h"
#include "Material.h"
#include "Intersection.h"
#include "Ray.h"
#include "Photon.h"
#include "PhotonContainer.h"
#include "SceneObjectVTableForwardDeclaration.h"


SceneObject makeSceneObjectPlane (const Plane plane, const Material *material);

Intersection sceneObjectUnitPlaneIntersectRay(const SceneObject object, const Ray ray);
bool sceneObjectUnitPlaneEmitPhotons(const SceneObject object, const int numPhotons, PhotonContainer *photons);

extern const SceneObjectVTable sceneObjectPlaneVTable;


#include "SceneObjectVTable.h"

#endif // SCENEOBJECTUNITPLANE_H