#ifndef PHOTONENDPOINT_H
#define PHOTONENDPOINT_H

#include "Intersection.h"
#include "Photon.h"

typedef struct {
	Photon photon;
	Intersection intersection;
} PhotonEndPoint;

PhotonEndPoint makePhotonEndPoint(const Photon photon, const Intersection intersection);

#endif // PHOTONENDPOINT_H
