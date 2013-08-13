#include "Ray.h"

Ray makeRay(const Vector origin, const Vector direction) {

	return (Ray) {origin, direction};
}
