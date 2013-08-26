#include "MaterialDiffuse.h"
#include "randf.h"
#include <math.h>

const MaterialVTable materialDiffuseVTable = (MaterialVTable) {
	&materialDiffuseSampleBRDF,
	&materialDiffuseBRDF
};

MaterialDiffuse makeMaterialDiffuse(const Color reflectivity, const Color radience) {
	
	return (MaterialDiffuse) {makeMaterial(&materialDiffuseVTable, reflectivity, radience)};
}

defineAllocator(MaterialDiffuse)

Photon materialDiffuseSampleBRDF(const Material *material, const Intersection intersection, const Photon incoming) {
	
	// Diffuse surface
	
	return makePhoton(makeRay(vAdd(intersection.position, vsMul(intersection.normal, vEpsilon)), vSampleHemisphere(intersection.normal)), cMul(incoming.energy, material->reflectivity));
}

Color materialDiffuseBRDF(const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing) {
	
	// Diffuse shading
	
	float surfaceIllumination = vDot(intersection.normal, incoming);
	return csMul(material->reflectivity, fmax(0, surfaceIllumination));
}