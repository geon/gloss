#ifndef MATERIALLAMP_H
#define MATERIALLAMP_H

#include "Material.h"
#include "allocator.h"


typedef struct {
	const Material parent;
	const Color radience
} MaterialLamp;


MaterialLamp makeMaterialLamp(const Color radience);

declareAllocator(MaterialLamp)

Photon materialLampSampleBRDF(const Material *material, const Intersection intersection, const Photon incoming);
Color materialLampBRDF(const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing);

extern const MaterialVTable materialLampVTable;


#endif // MATERIALLAMP_H