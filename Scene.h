#ifndef SCENE_H
#define SCENE_H

#include "SceneObjectContainer.h"
#include "MaterialContainer.h"
#include "PhotonEndPointContainer.h"
#include "Photon.h"
#include "Matrix.h"
#include "Color.h"
#include "Intersection.h"
#include "SceneObject.h"


typedef struct {
	SceneObjectContainer objects;
	MaterialContainer materials;
	PhotonEndPointContainer photons;
	Matrix cameraOrientation;
	Color skyColor;
	float standardReflectivity;
} Scene;


Scene makeScene(void);

Color sceneTraceRayAtPixel(const Scene *scene, const int currentPixel, const int width, const int height, const int numCameraRayBounces);
void sceneGeneratePhotons(Scene *scene, const int lightRayBounces, const int numPhotonsPerLightSource);
Color sceneTraceRay(const Scene *scene, const Ray ray, const int numCameraRayBounces);
Intersection sceneIntersectRay(const Scene scene, const Ray ray);
void buildCornellBox(Scene *scene);

#endif // SCENE_H
