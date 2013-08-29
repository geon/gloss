#include "MaterialLamp.h"
#include "randf.h"
#include <math.h>

const MaterialVTable materialLampVTable = (MaterialVTable) {
	&materialLampSampleBRDF,
	&materialLampBRDF,
	&materialLampIrradience
};

MaterialLamp makeMaterialLamp(const Color irradience) {
	
	return (MaterialLamp) {makeMaterial(&materialLampVTable), irradience};
}

defineAllocator(MaterialLamp)

Photon materialLampSampleBRDF(const Material *material, const Intersection intersection, const Photon incoming) {
	
	// Perfectly black surface.	The direction of the reflected photon doesn't matter.
	return makePhoton(makeRay(intersection.position, intersection.normal), makeColorBlack());
}

Color materialLampBRDF(const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing) {
	
	// Perfectly black surface. All light comes from radience, not reflectance.
	return makeColorBlack();
}

Color materialLampIrradience(const Material *superObject) {
	
	MaterialLamp *material = (MaterialLamp *) superObject;
	
	// Uniformly radiating, perfectly diffuse surface.
	return material->irradience;
}
