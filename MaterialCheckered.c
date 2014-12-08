#include "MaterialCheckered.h"
#include "randf.h"
#include "pi.h"
#include <math.h>

const MaterialVTable materialCheckeredVTable = (MaterialVTable) {
	&materialCheckeredSampleBRDF,
	&materialCheckeredBRDF,
	&materialCheckeredIrradience
};

MaterialCheckered makeMaterialCheckered(const Material *a, const Material *b) {
	
	return (MaterialCheckered) {makeMaterial(&materialCheckeredVTable), a, b};
}

defineAllocator(MaterialCheckered)

bool materialCheckeredFIsEven(float f) {
	
	return 0 != ((int)floor(f)) % 2;
}

bool materialCheckeredVIsInA(Vector v) {
	
	return
		materialCheckeredFIsEven(v.x) !=
		materialCheckeredFIsEven(v.y) !=
		materialCheckeredFIsEven(v.z)
	;
}

Photon materialCheckeredSampleBRDF(const Material *superObject, const Intersection intersection, const Photon incoming) {
	
	MaterialCheckered *material = (MaterialCheckered *) superObject;
	
	if(materialCheckeredVIsInA(intersection.position)){

		return materialSampleBRDF(material->a, intersection, incoming);

	}else{

		return materialSampleBRDF(material->b, intersection, incoming);
	}
}

Color materialCheckeredBRDF(const Material *superObject, const Intersection intersection, const Vector incoming, const Vector outgoing) {
	
	MaterialCheckered *material = (MaterialCheckered *) superObject;
	
	if(materialCheckeredVIsInA(intersection.position)){
		
		return materialBRDF(material->a, intersection, incoming, outgoing);
		
	}else{
		
		return materialBRDF(material->b, intersection, incoming, outgoing);
	}
}

Color materialCheckeredIrradience(const Material *superObject) {
	
	MaterialCheckered *material = (MaterialCheckered *) superObject;
	
	return csMul(cAdd(materialIrradience(material->a), materialIrradience(material->b)), 0.5);
}
