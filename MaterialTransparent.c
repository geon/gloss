#include "MaterialTransparent.h"
#include "randf.h"
#include "pi.h"
#include "ColorHSL.h"
#include <math.h>
#include <stdio.h>

const MaterialVTable materialTransparentVTable = (MaterialVTable) {
	&materialTransparentSampleBRDF,
	&materialTransparentBRDF,
	&materialTransparentIrradience
};

MaterialTransparent makeMaterialTransparent(const Color reflectivity, const float specularity, const float exponent, const bool metallic, const float surfaceTransparency, const float refractiveIndex, const Color halfAbsorbDistance) {
	
	return (MaterialTransparent) {{{makeMaterial(&materialTransparentVTable), reflectivity}, specularity, exponent, metallic}, surfaceTransparency, refractiveIndex, halfAbsorbDistance};
}

defineAllocator(MaterialTransparent)

Photon materialTransparentSampleBRDF(const Material *superObject, const Intersection intersection, const Photon incoming) {
	
	MaterialTransparent *material = (MaterialTransparent *) superObject;
	
	Photon component;
	if (randf() > material->surfaceTransparency){

		component = materialPhongSampleBRDF(superObject, intersection, incoming);

	} else {

		// Interior component.
		
		float refractiveIndex = material->refractiveIndex;
		//		ColorHSL hsl = cHSL(incoming.energy);
		//		const float spread = 0.1;
		//		float refractiveIndex = expf(logf(material->refractiveIndex) * (1 + (1 - 2*hsl.hue) * spread));
		// Snell's Law: http://en.wikipedia.org/wiki/Snell%27s_law

		Vector normal = intersection.normal;
		float incomingAngle = acosf(vDot(intersection.normal, vNegated(incoming.heading.direction)));
		if (incomingAngle > PI/2) {

			//  The ray comes from the inside the object, so inverse all variables.
			refractiveIndex = 1/refractiveIndex;
			incomingAngle = 2*PI - incomingAngle;
			normal = vNegated(normal);
		}
		
		float outgoingAngle = asinf(sinf(incomingAngle) / refractiveIndex) ;
		float maxOutgoingAngle = asinf(sinf(PI/2) / refractiveIndex) ;
//		printf("%f\n", outgoingAngle);
		
		Vector outgoingDirection;
		if (outgoingAngle > maxOutgoingAngle) {
			
			// Total internal reflection
			outgoingDirection = vReflected(incoming.heading.direction, normal);
			
		} else {

			// Refraction
			Vector tangent = vCross(normal, incoming.heading.direction);
			outgoingDirection = vRotated(vNegated(normal), tangent, outgoingAngle);
		}

		
		Vector tangent = vTangent(outgoingDirection);
		outgoingDirection = vRotated(
			vRotated(outgoingDirection, tangent, acosf(powf(randf(), 1/material->parent.specularity))),
			outgoingDirection,
			randf() * 2*PI
		);

		
		Vector offset = vsMul(outgoingDirection, vEpsilon); // Avoid hitting the same surface again.
		
		component = makePhoton(makeRay(vAdd(intersection.position, offset), outgoingDirection), incoming.energy);

		// TODO: Handle attenuation.
	
		
	}

	return component;
}

Color materialTransparentBRDF(const Material *superObject, const Intersection intersection, const Vector incoming, const Vector outgoing) {
	
//	MaterialTransparent *material = (MaterialTransparent *) superObject;

	// Just ignore transparency here.
	return materialPhongBRDF(superObject, intersection, incoming, outgoing);
}

Color materialTransparentIrradience(const Material *material) {
	
	// No light source.
	return makeColorBlack();
}
