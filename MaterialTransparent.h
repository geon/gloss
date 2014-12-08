#ifndef MATERIALTRANSPARENT_H
#define MATERIALTRANSPARENT_H

#include "MaterialPhong.h"
#include "allocator.h"


typedef struct {
	const MaterialPhong parent;
	float surfaceTransparency;
	float refractiveIndex;
	Color halfAbsorbDistance;
} MaterialTransparent;


MaterialTransparent makeMaterialTransparent(const Color reflectivity, const float specularity, const float exponent, const bool metallic, const float surfaceTransparency, const float refractiveIndex, const Color halfAbsorbDistance);

declareAllocator(MaterialTransparent)

Photon materialTransparentSampleBRDF(const Material *material, const Intersection intersection, const Photon incoming);
Color materialTransparentBRDF(const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing);
Color materialTransparentIrradience(const Material *material);

extern const MaterialVTable materialTransparentVTable;


#endif // MATERIALTRANSPARENT_H