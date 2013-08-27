#ifndef MATERIALDIFFUSE_H
#define MATERIALDIFFUSE_H

#include "Material.h"
#include "allocator.h"


typedef struct {
	const Material parent;
	Color reflectivity;
} MaterialDiffuse;


MaterialDiffuse makeMaterialDiffuse(const Color reflectivity);

declareAllocator(MaterialDiffuse)

Photon materialDiffuseSampleBRDF(const Material *material, const Intersection intersection, const Photon incoming);
Color materialDiffuseBRDF(const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing);

extern const MaterialVTable materialDiffuseVTable;


#endif // MATERIALDIFFUSE_H