#include "Ray.h"

Ray makeRay(Vector origin, Vector direction) {

    return (Ray) {origin, direction};
}
