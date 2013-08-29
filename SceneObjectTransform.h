#ifndef SCENEOBJECTTRANSFORM_H
#define SCENEOBJECTTRANSFORM_H

#include "SceneObject.h"
#include "Matrix.h"
#include "Material.h"
#include "Intersection.h"
#include "Ray.h"
#include "Photon.h"
#include "PhotonContainer.h"
#include "allocator.h"


typedef struct {
	SceneObject parent;
	const SceneObject *subject;
	Matrix transform;
	Matrix inversedTransform;
} SceneObjectTransform;


SceneObjectTransform makeSceneObjectTransform(const Matrix transform, const SceneObject *subject);

declareAllocator(SceneObjectTransform)

Intersection sceneObjectTransformIntersectRay(const SceneObject *object, const Ray ray);
bool sceneObjectTransformEmitPhotons(const SceneObject *object, const int numPhotons, PhotonContainer *photons);
Color sceneObjectTransformRadiantFlux(const SceneObject *object);

extern const SceneObjectVTable sceneObjectTransformVTable;


#endif // SCENEOBJECTTRANSFORM_H
