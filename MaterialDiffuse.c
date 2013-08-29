#include "MaterialDiffuse.h"
#include "randf.h"
#include <math.h>

const MaterialVTable materialDiffuseVTable = (MaterialVTable) {
	&materialDiffuseSampleBRDF,
	&materialDiffuseBRDF,
	&materialDiffuseIrradience
};

MaterialDiffuse makeMaterialDiffuse(const Color reflectivity) {
	
	return (MaterialDiffuse) {makeMaterial(&materialDiffuseVTable), reflectivity};
}

defineAllocator(MaterialDiffuse)

Photon materialDiffuseSampleBRDF(const Material *superObject, const Intersection intersection, const Photon incoming) {
	
	MaterialDiffuse *material = (MaterialDiffuse *) superObject;
	
	return makePhoton(makeRay(vAdd(intersection.position, vsMul(intersection.normal, vEpsilon)), vSampleHemisphere(intersection.normal)), cMul(incoming.energy, material->reflectivity));
}

Color materialDiffuseBRDF(const Material *superObject, const Intersection intersection, const Vector incoming, const Vector outgoing) {
	
	MaterialDiffuse *material = (MaterialDiffuse *) superObject;
	
	float surfaceIllumination = fmax(0, vDot(intersection.normal, incoming));
	
	return csMul(material->reflectivity, surfaceIllumination);
}

Color materialDiffuseIrradience(const Material *material) {
	
	// No light source.
	return makeColorBlack();
}
