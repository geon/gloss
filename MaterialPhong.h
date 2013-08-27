#ifndef MATERIALPHONG_H
#define MATERIALPHONG_H

#include "MaterialDiffuse.h"
#include "allocator.h"


typedef struct {
	const MaterialDiffuse parent;
	float specularity;
	float exponent;
	bool  metallic;
} MaterialPhong;


MaterialPhong makeMaterialPhong(const Color reflectivity, const float specularity, const float exponent, const bool metallic);

declareAllocator(MaterialPhong)

Photon materialPhongSampleBRDF(const Material *material, const Intersection intersection, const Photon incoming);
Color materialPhongBRDF(const Material *material, const Intersection intersection, const Vector incoming, const Vector outgoing);

extern const MaterialVTable materialPhongVTable;


#endif // MATERIALPHONG_H