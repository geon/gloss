#include "Material.h"
#include "randf.h"
#include <math.h>

Material makeMaterial(const Color reflectivity, const Color radience, const bool isPerfectBlack) {
	
	return (Material) {reflectivity, radience, isPerfectBlack};
}

Photon materialSampleBRDF(const Material *material, const Intersection intersection, const Photon incoming) {
	
	// Difuse surface
	
	return makePhoton(makeRay(vAdd(intersection.position, vsMul(intersection.normal, vEpsilon)), vSampleHemisphere(intersection.normal)), cMul(incoming.energy, material->reflectivity));
}

Color materialBRDF(const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing) {

	// Diffuse shading
	
	float surfaceIllumination = vDot(intersection.normal, incoming);
	return csMul(material->reflectivity, fmax(0, surfaceIllumination));
}