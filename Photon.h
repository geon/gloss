#ifndef PHOTON_H
#define PHOTON_H

#include "Ray.h"
#include "Color.h"

typedef struct {
    Ray heading;
    Color energy;
} Photon;

Photon makePhoton(Ray heading, Color energy);

#endif // PHOTON_H
