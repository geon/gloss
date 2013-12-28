#include "MaterialDiffuse.h"
#include "randf.h"
#include "pi.h"
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
	
//	2*acos(1-x)/pi

	Vector reflectedDirection = vRotated(
		vRotated(intersection.normal, vTangent(intersection.normal), acosf(1-randf())),
		intersection.normal,
		randf() * 2*PI
	);
	
	return makePhoton(makeRay(vAdd(intersection.position, vsMul(intersection.normal, vEpsilon)), reflectedDirection), cMul(incoming.energy, material->reflectivity));
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
