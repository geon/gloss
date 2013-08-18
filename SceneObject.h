#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "SceneObjectVTableForwardDeclaration.h"
#include "Material.h"
#include "Matrix.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "PhotonContainer.h"


typedef struct {
	const SceneObjectVTable *vTable;
	const Material *material;
	Matrix transform;
	Matrix inversedTransform;
	union {
		Sphere sphere;
		Plane plane;
		Box box;
	};
} SceneObject;


Intersection sceneObjectIntersectRay(const SceneObject object, const Ray ray);
bool sceneObjectEmitPhotons(const SceneObject object, const int numPhotons, PhotonContainer *photons);


#include "SceneObjectVTable.h"

#endif // SCENEOBJECT_H
