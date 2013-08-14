#ifndef SCENEOBJECTSPHERE_H
#define SCENEOBJECTSPHERE_H

#include "SceneObject.h"
#include "Sphere.h"
#include "Material.h"
#include "Intersection.h"
#include "Ray.h"
#include "Photon.h"
#include "PhotonContainer.h"
#include "SceneObjectVTableForwardDeclaration.h"


SceneObject makeSceneObjectSphere(const Sphere sphere, const Matrix transform, const Material *material);

Intersection sceneObjectSphereIntersectRay(const SceneObject object, const Ray ray);
bool sceneObjectSphereEmitPhotons(const SceneObject object, const int numPhotons, PhotonContainer *photons);

extern const SceneObjectVTable sceneObjectSphereVTable;


#include "SceneObjectVTable.h"

#endif // SCENEOBJECTSPHERE_H