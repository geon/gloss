#ifndef MATERIAL_H
#define MATERIAL_H

#include "MaterialForwardDeclaration.h"
#include "Color.h"
#include "bool.h"
#include "Photon.h"
#include "Intersection.h"


struct MaterialStruct {
	Color reflectivity;
	Color radience;
	bool isPerfectBlack;
};

Material makeMaterial(const Color reflectivity, const Color radience, const bool isPerfectBlack);


Photon materialSampleBRDF(const Material *material, const Intersection intersection, const Photon incoming);
Color materialBRDF(const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing);

#endif // MATERIAL_H
