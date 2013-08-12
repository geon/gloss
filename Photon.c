#include "Photon.h"

Photon makePhoton(Ray heading, Color energy) {

	return (Photon) {heading, energy};
}
