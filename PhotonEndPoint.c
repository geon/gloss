#include "PhotonEndPoint.h"

PhotonEndPoint makePhotonEndPoint(const Photon photon, const Intersection intersection) {

	return (PhotonEndPoint) {photon, intersection};
}
