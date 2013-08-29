#include "MaterialPhong.h"
#include "randf.h"
#include "pi.h"
#include <math.h>

const MaterialVTable materialPhongVTable = (MaterialVTable) {
	&materialPhongSampleBRDF,
	&materialPhongBRDF,
	&materialPhongIrradience
};

MaterialPhong makeMaterialPhong(const Color reflectivity, const float specularity, const float exponent, const bool metallic) {
	
	return (MaterialPhong) {{makeMaterial(&materialPhongVTable), reflectivity}, specularity, exponent, metallic};
}

defineAllocator(MaterialPhong)

Photon materialPhongSampleBRDF(const Material *superObject, const Intersection intersection, const Photon incoming) {
	
	MaterialPhong *material = (MaterialPhong *) superObject;
	
	// "Interpolate" diffuse and specular based on material specularity.
	if(randf() > material->specularity){

		// Diffuse component.
		return materialDiffuseSampleBRDF(superObject, intersection, incoming);

	}else{

		// Specular component.
		Vector perfectReflection = vReflected(incoming.heading.direction, intersection.normal);
		Vector tangent = vCross(intersection.normal, perfectReflection);
		
		// Try n times to find a valid light direction. Fallback to the perfect reflection.
		// I just wanted this to be a guaranteed finite loop.
		Vector reflectedDirection = perfectReflection;
		for (int i=0; i<100; ++i) {

			// Rotate the perfect reflection randomly, weighted by the Phong specularity function.
			Vector randomizedDirection = vRotated(
				vRotated(perfectReflection, tangent, acosf(powf(randf(), 1/(material->exponent+1)))),
				perfectReflection,
				randf() * 2*PI
			);
			
			// When a valid direction is found, we're done.
			if(vDot(randomizedDirection, intersection.normal) > 0) {
				
				reflectedDirection = randomizedDirection;
				
				break;
			}
		}
		
		return makePhoton(makeRay(vAdd(intersection.position, vsMul(intersection.normal, vEpsilon)), reflectedDirection), (material->metallic ? cMul(incoming.energy, material->parent.reflectivity) : incoming.energy));
	}
}

Color materialPhongBRDF(const Material *superObject, const Intersection intersection, const Vector incoming, const Vector outgoing) {
	
	MaterialPhong *material = (MaterialPhong *) superObject;
	
	Vector perfectReflection = vReflected(incoming, intersection.normal);
	
	float  diffuseSurfaceIllumination = fmax(0, vDot(intersection.normal, incoming));
	float specularSurfaceIllumination = fmax(0, powf(vDot(outgoing, perfectReflection), material->exponent));
	
	if (material->metallic) {
		
		return csMul(material->parent.reflectivity,
			 diffuseSurfaceIllumination * (1 - material->specularity) +
			specularSurfaceIllumination *      material->specularity
		);
		
	} else {

		return cAdd(
			csMul(material->parent.reflectivity, diffuseSurfaceIllumination * (1 - material->specularity)),
			makeColorLightness(specularSurfaceIllumination * material->specularity)
		);
	}

}

Color materialPhongIrradience(const Material *material) {
	
	// No light source.
	return makeColorBlack();
}
