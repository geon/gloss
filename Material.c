#include "Material.h"

Material makeMaterial(const MaterialVTable *vTable, const Color reflectivity, const Color radience) {

	return (Material) {vTable, reflectivity, radience};
}

Photon materialSampleBRDF(const Material *material, const Intersection intersection, const Photon incoming) {
	
	return material->vTable->materialSampleBRDF(material, intersection, incoming);
}

Color materialBRDF(const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing) {

	return material->vTable->materialBRDF(material, intersection, incoming, outgoing);
}