#include "PhotonEndPoint.h"

PhotonEndPoint makePhotonEndPoint(Photon photon, Intersection intersection) {

	return (PhotonEndPoint) {photon, intersection};
}
