#include "Box.h"
#include "Ray.h"
#include "Plane.h"
#include <math.h>


Box makeBox(const Vector size) {

	return (Box) {size};
}

Intersection bIntersect(const Box box, const Ray ray) {

	Plane sides[] = {
		makePlane(makeVector( 1,  0,  0), box.size.x),
		makePlane(makeVector( 0,  1,  0), box.size.y),
		makePlane(makeVector( 0,  0,  1), box.size.z),
		makePlane(makeVector(-1,  0,  0), box.size.x),
		makePlane(makeVector( 0, -1,  0), box.size.y),
		makePlane(makeVector( 0,  0, -1), box.size.z)
	};

	
	Intersection closestIntersection;
	closestIntersection.hitType = missed;
	float minDistance = INFINITY;
	
	for (int i=0; i<6; ++i) {
		
		// Find the plane with the closest intersectionpoint.
		Intersection currentIntersection = pIntersect(sides[i], ray);
		if(currentIntersection.hitType && currentIntersection.distance < minDistance){
			
			// Check if the intersection is inside the face.
			bool isInsideFace = true;
			for(int j=0; j<6; ++j){
				
				// Skip the current and the opposite side.
				if(j == i || ((j+3)%6)==i) {
					continue;
				}
				
				// A point is inside the face if it is also inside all other planes.
				if(!pIsInside(sides[j], currentIntersection.position)){
					isInsideFace = false;
					break;
				}
			}
			
			if(isInsideFace){
				
				closestIntersection = currentIntersection;
				
				// OPTIMIZATION HACK THAT DEPENDS ON Plane NOT INTERSECTING Ray GOING *OUT*.
				// If planes can be intersected from the inside, remove this.
				return closestIntersection;
			}
		}
	}
	
	return closestIntersection;
};
