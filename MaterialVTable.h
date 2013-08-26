#ifndef MATERIALVTABLE_H
#define MATERIALVTABLE_H

#include "MaterialVTableForwardDeclaration.h"
#include "Material.h"
#include "Photon.h"
#include "Intersection.h"

struct MaterialVTableStruct {
	Photon (*materialSampleBRDF)(const Material *material, const Intersection intersection, const Photon incoming);
	Color  (*materialBRDF)      (const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing);
};

#endif // MATERIALVTABLE_H