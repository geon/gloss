#ifndef MATERIALVTABLE_H
#define MATERIALVTABLE_H

#include "MaterialVTableForwardDeclaration.h"
#include "Material.h"
#include "Photon.h"
#include "Intersection.h"

struct MaterialVTableStruct {
	Photon (*materialSampleBRDF)(const Material *material, const Intersection intersection, const Photon incoming);
	Color  (*materialBRDF)      (const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing);
// TODO: Add possibility for nonuniform surface brightness.
//	Color  (*materialIrradience)(const Material *material, const Vector position, const Vector outgoing);
	Color  (*materialIrradience)(const Material *material);
};

#endif // MATERIALVTABLE_H