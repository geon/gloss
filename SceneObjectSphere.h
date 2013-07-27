#ifndef SCENEOBJECTSPHERE_H
#define SCENEOBJECTSPHERE_H

extern const SceneObjectVTable sceneObjectSphereVTable;

SceneObject makeSceneObjectSphere (const SceneObjectSphere sphere, const Material *material);

Intersection sceneObjectSphereIntersectRay(const SceneObject object, const Ray ray);
bool sceneObjectSphereEmitPhotons(const SceneObject object, const int numPhotons, Photon photons[]);

#endif // SCENEOBJECTSPHERE_H