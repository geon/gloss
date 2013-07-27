#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "SceneObjectVTable.h"
#include "Material.h"

#include "SceneObjectSphere.h"
#include "SceneObjectPlane.h"

typedef struct {
    SceneObjectVTable *vTable;
    Material *material;
    union {
        SceneObjectSphere sphere;
        SceneObjectPlane plane;
        // SceneObjectTransform transform;
        // SceneObjectBox box;
    }
} SceneObject;

#endif // SCENEOBJECT_H
