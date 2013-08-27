#ifndef MATERIAL_H
#define MATERIAL_H

#include "MaterialVTableForwardDeclaration.h"
#include "MaterialForwardDeclaration.h"
#include "Color.h"
#include "bool.h"
#include "Photon.h"
#include "Intersection.h"


struct MaterialStruct {
	const MaterialVTable *vTable;
	Color radience;
};

Material makeMaterial(const MaterialVTable *vTable, const Color radience);


Photon materialSampleBRDF(const Material *material, const Intersection intersection, const Photon incoming);
Color materialBRDF(const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing);

#include "MaterialVTable.h"


#endif // MATERIAL_H
