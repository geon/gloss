#ifndef MATERIALCHECKERED_H
#define MATERIALCHECKERED_H

#include "Material.h"
#include "allocator.h"


typedef struct {
	const Material parent;
	const Material *a;
	const Material *b;
} MaterialCheckered;


MaterialCheckered makeMaterialCheckered(const Material *a, const Material *b);

declareAllocator(MaterialCheckered)

Photon materialCheckeredSampleBRDF(const Material *material, const Intersection intersection, const Photon incoming);
Color materialCheckeredBRDF(const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing);
Color materialCheckeredIrradience(const Material *material);

extern const MaterialVTable materialCheckeredVTable;


#endif // MATERIALCHECKERED_H