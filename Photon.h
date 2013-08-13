#ifndef PHOTON_H
#define PHOTON_H

#include "Ray.h"
#include "Color.h"

typedef struct {
	Ray heading;
	Color energy;
} Photon;

Photon makePhoton(const Ray heading, const Color energy);

#endif // PHOTON_H
