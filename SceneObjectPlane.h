#ifndef SCENEOBJECTPLANE_H
#define SCENEOBJECTPLANE_H

extern const SceneObjectVTable sceneObjectPlaneVTable;

SceneObject makeSceneObjectPlane (const SceneObjectPlane plane, const Material *material);

Intersection sceneObjectPlaneIntersectRay(const SceneObject object, const Ray ray);
bool sceneObjectPlaneEmitPhotons(const SceneObject object, const int numPhotons, Photon photons[]);

#endif // SCENEOBJECTPLANE_H