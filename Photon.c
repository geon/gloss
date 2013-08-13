#include "Photon.h"

Photon makePhoton(const Ray heading, const Color energy) {

	return (Photon) {heading, energy};
}
