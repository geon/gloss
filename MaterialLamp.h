#ifndef MATERIALLAMP_H
#define MATERIALLAMP_H

#include "Material.h"
#include "allocator.h"
#include "SceneObject.h"


typedef struct {
	const Material parent;
	const Color irradience;
} MaterialLamp;


MaterialLamp makeMaterialLamp(const Color irradience);

declareAllocator(MaterialLamp)

Photon materialLampSampleBRDF(const Material *material, const Intersection intersection, const Photon incoming);
Color materialLampBRDF(const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing);
Color materialLampIrradience(const Material *material);

extern const MaterialVTable materialLampVTable;


#endif // MATERIALLAMP_H