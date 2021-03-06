#include "Material.h"

Material makeMaterial(const MaterialVTable *vTable) {

	return (Material) {vTable};
}

Photon materialSampleBRDF(const Material *material, const Intersection intersection, const Photon incoming) {
	
	return material->vTable->materialSampleBRDF(material, intersection, incoming);
}

Color materialBRDF(const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing) {

	return material->vTable->materialBRDF(material, intersection, incoming, outgoing);
}

Color materialIrradience(const Material *material) {
	
	return material->vTable->materialIrradience(material);
}
