#ifndef PHOTONENDPOINT_H
#define PHOTONENDPOINT_H

#include "Intersection.h"
#include "Photon.h"

typedef struct {
    Intersection intersection;
    Photon photon;
} PhotonEndPoint;

PhotonEndPoint makePhotonEndPoint(Photon photon, Intersection intersection);

#endif // PHOTONENDPOINT_H
