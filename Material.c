#include "Material.h"

Material makeMaterial(const Color reflectivity, const Color radience, const bool isPerfectBlack) {
	
	return (Material) {reflectivity, radience, isPerfectBlack};
}

Photon materialSampleBRDF(const Material *material, const Intersection intersection, const Photon incoming) {
	
	Photon outgoing;
	
	// TODO: Actually implement this.
	
	return outgoing;
}

Color materialBRDF(const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing) {

	// TODO: Actually implement this.
	
	return makeColorLightness(0.8);
}