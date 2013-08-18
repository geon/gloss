#include "Scene.h"
#include "Matrix.h"
#include "Color.h"
#include "Intersection.h"
#include "PhotonEndpoint.h"
#include "PhotonEndPointContainer.h"
#include "PhotonContainer.h"
#include "pi.h"
#include "randf.h"
#include <stddef.h>
#include <math.h>
#include "SceneObjectSphere.h"
#include "SceneObjectUnitPlane.h"
#include "SceneObjectBox.h"
#include "bool.h"


Scene makeScene () {

	return (Scene) {makeSceneObjectContainer(10), makeMaterialContainer(10), makePhotonEndPointContainer(100), makeMatrixIdentity(), makeColor(0.5, 0.7, 1), 0.5};
}

Color sceneTraceRayAtPixel(const Scene *scene, const int currentPixel, const int width, const int height, const int numCameraRayBounces) {
	
	const float cameraFov = 40;
	const float cameraAspectRatio = 1;

	float maxX = tanf(cameraFov/360.0*PI);
	float x =  ((((currentPixel % width) + randf()) / width ) * 2 - 1) * maxX;
	float y = -((((currentPixel / width) + randf()) / height) * 2 - 1) * maxX / cameraAspectRatio;

	return sceneTraceRay(scene, mrMul(mInversed(scene->cameraOrientation), makeRay(makeVectorOrigo(), vNormalized(makeVector(x, y, 1)))), numCameraRayBounces);
}

void sceneGeneratePhotons(Scene *scene, const int lightRayBounces, const int numPhotonsPerLightSource) {
	
	photonEndPointContainerClear(&scene->photons);
	
	containerForeach (SceneObject, object, scene->objects) {
		
		// TODO: Not only perfect black bodies emits light, and not all do. Emit photons based on light intensity.
		if(object->material->isPerfectBlack) {

			// Spawn photons on the surface of the lightsource.
			PhotonContainer emittedPhotons = makePhotonContainer(numPhotonsPerLightSource);
			sceneObjectEmitPhotons(*object, numPhotonsPerLightSource, &emittedPhotons);

			containerForeach (Photon, photon, emittedPhotons) {
			
				// Save it for rendering.
				photonEndPointContainerAddValue(&scene->photons, makePhotonEndPoint(*photon, makeIntersection(origin, makeVectorOrigo(), 0, makeVectorOrigo(), NULL)));

				for(int bounce=0; bounce<lightRayBounces; ++bounce){
										
					// Check if the photon hits anyting.
					Intersection intersection = sceneIntersectRay(*scene, photon->heading);
					if(intersection.hitType != surface)
						break;
					
					// Bounce the photon, filtering it by the reflectance.
					*photon = materialSampleBRDF(intersection.material, intersection, *photon);
					
					// Save it for rendering.
					photonEndPointContainerAddValue(&scene->photons, makePhotonEndPoint(*photon, intersection));
				}

			}
			
			// Done with them.
			photonContainerDestroy(&emittedPhotons);
		}
	}
}


Color sceneTraceRay(const Scene *scene, const Ray ray, int numCameraRayBounces) {
	
	Photon bouncedRay = makePhoton(ray, makeColorWhite());
	Color collectedLight = makeColorBlack();
	int numSampledCameraRayBounces = 0;
	for (int bounce=0; bounce<numCameraRayBounces; ++bounce) {
		++numSampledCameraRayBounces;

		// Intersect the ray with the scene.
		Intersection intersection = sceneIntersectRay(*scene, bouncedRay.heading);

		// No hit, so use sky color.
		if (intersection.hitType == missed){

			collectedLight = cAdd(collectedLight, cMul(scene->skyColor, bouncedRay.energy));
			break;
		}

		// Add the emitted light from any intersected lightsource object.
		collectedLight = cAdd(collectedLight, cMul(intersection.material->radience, bouncedRay.energy));

		// Hit a perfectly black surface, so stop.
		if (intersection.hitType == perfectBlack) {
			break;
		}

		// Collect light from all photons.
		Color sumPhotonLight = makeColorBlack();
		containerForeach (PhotonEndPoint, photonEndPoint, scene->photons) {

			// Create shadow ray.
			Vector lightVector = vSub(photonEndPoint->photon.heading.origin, intersection.position);
			float lightDistance = vLength(lightVector);
			Vector lightDirection = vsDiv(lightVector, lightDistance);
			Ray shadowRay = makeRay(intersection.position, lightDirection);

			// Cast shadow ray.
			Intersection occluder = sceneIntersectRay(*scene, shadowRay);

			// For any unoccluded scene.photons, collect the energy.
			if(occluder.hitType == missed || occluder.distance > lightDistance){

				// Incoming intensity.
				Color incomingLight = photonEndPoint->photon.energy;

				// Anyhting but the origin has a BRDF.
				if (photonEndPoint->intersection.hitType != origin) {

					Color photonBRDF = materialBRDF(intersection.material, photonEndPoint->intersection, vNegated(lightDirection), bouncedRay.heading.direction);
					incomingLight = cMul(incomingLight, photonBRDF);
				}

				// The camera ray BRDF.
				Color cameraRayBRDF = materialBRDF(intersection.material, intersection, lightDirection, vNegated(bouncedRay.heading.direction));
				incomingLight = cMul(incomingLight, cameraRayBRDF);

				sumPhotonLight = cAdd(sumPhotonLight, incomingLight);
			}
		}
		collectedLight = cAdd(collectedLight, cMul(sumPhotonLight, bouncedRay.energy));

		// Bounce the camera ray, filtering it by the reflectance.
		bouncedRay = materialSampleBRDF(intersection.material, intersection, bouncedRay);
	}

	// Return the average of all sampled paths.
	return csMul(collectedLight, 1.0/numSampledCameraRayBounces);
}






Intersection sceneIntersectRay(const Scene scene, const Ray ray) {

	// Find the object with the closest intersection, or return  a miss.
	
	Intersection closestIntersection;
	closestIntersection.hitType = missed;
	float minDistance = INFINITY;

	containerForeach (SceneObject, object, scene.objects) {

		Intersection currentIntersection = sceneObjectIntersectRay(*object, ray);

		if(currentIntersection.hitType != missed && currentIntersection.distance < minDistance) {

			minDistance = currentIntersection.distance;
			closestIntersection = currentIntersection;
		}
	}

	return closestIntersection;
}

void buildCornellBox(Scene *scene) {

	// Camera	
	scene->cameraOrientation = makeMatrixTranslation(makeVector(0, 0, 3.8));

	
	
	Material *whiteMaterial = materialContainerAddValue(&scene->materials, makeMaterial(makeColorLightness(scene->standardReflectivity), makeColorBlack(), 0));
	Material *lampMaterial  = materialContainerAddValue(&scene->materials, makeMaterial(makeColorBlack(), makeColorLightness(10), 1));
	
	
	// Lights
	scene->skyColor = makeColorBlack();
	float lr = 0.2;
	sceneObjectContainerAddValue(&scene->objects, makeSceneObjectSphere(makeSphere(makeVector(0, 1-lr - 0.05, 0), .2), makeMatrixIdentity(), lampMaterial));


	// Boxes
	sceneObjectContainerAddValue(&scene->objects, makeSceneObjectBox(
		makeVector(.3, .7, .3),
		mMul(makeMatrixAxisAngle(makeVector(0, 1, 0), .3), makeMatrixTranslation(makeVector(-.3, -.3, .3))),
		whiteMaterial
	));
	sceneObjectContainerAddValue(&scene->objects, makeSceneObjectBox(
		makeVector(.3, .3, .3),
		mMul(makeMatrixAxisAngle(makeVector(0, 1, 0), -.3), makeMatrixTranslation(makeVector(.3, -.75, -.3))),
		whiteMaterial
	));

	
	// Walls
	
	// Floor
	sceneObjectContainerAddValue(&scene->objects, makeSceneObjectUnitPlane(
		makePlane(makeVector(0, 1, 0), -1+vEpsilon),
		makeMatrixIdentity(),
		whiteMaterial
	));
	// Ceiling
	sceneObjectContainerAddValue(&scene->objects, makeSceneObjectUnitPlane(
		makePlane(makeVector(0, -1, 0), -1+vEpsilon),
		makeMatrixIdentity(),
		whiteMaterial
	));
	// Middle
	sceneObjectContainerAddValue(&scene->objects, makeSceneObjectUnitPlane(
		makePlane(makeVector(0, 0, -1), -1+vEpsilon),
		makeMatrixIdentity(),
		whiteMaterial
	));
	// Left
	sceneObjectContainerAddValue(&scene->objects, makeSceneObjectUnitPlane(
		makePlane(makeVector(1, 0, 0), -1+vEpsilon),
		makeMatrixIdentity(),
		materialContainerAddValue(&scene->materials, makeMaterial(csMul(makeColor(1, 0.5, 0.1), scene->standardReflectivity), makeColorBlack(), 0))
	));
	// Right
	sceneObjectContainerAddValue(&scene->objects, makeSceneObjectUnitPlane(
		makePlane(makeVector(-1, 0, 0), -1+vEpsilon),
		makeMatrixIdentity(),
		materialContainerAddValue(&scene->materials, makeMaterial(csMul(makeColor(0.1, 0.3, 0.5), scene->standardReflectivity), makeColorBlack(), 0))
	));
 
}

// void buildCornellBoxSpheres(Scene *scene) {


//sceneObjectContainerAddValue(&scene->objects, makeSceneObjectSphere(
//																	makeSphere(makeVector(0, -1+.3, 0), .3),
//																	makeMatrixIdentity(),
//																	whiteMaterial
//																	));
//

//	 // Spheres
//	 float r = 0.4;
//	 scene.push_back(new cSphereSceneObject(cSphere(makeVector(0, -1+r, 0), r), new cDiffuseMaterial(Color(1, 1, 1)*.5)));
//	 scene.push_back(new cSphereSceneObject(cSphere(makeVector(-.4, -1+r, -.2), r), new cPhongMaterial(Color(1, 1, .01)*.9, .8, 50, true)));
//	 scene.push_back(new cSphereSceneObject(cSphere(makeVector(.4, -1+r, .2), r), new cPhongMaterial(Color(1, 1, 1)*.5, .3, 10)));

//	 // scene.push_back(new cSceneObjectTransform(new cSphereSceneObject(cSphere(makeVector(0), r), new cPhongMaterial(Color(1, 1, .01)*.9, .8, 50, true)), cMatrix(makeVector(-.4, -1+r, -.2))));
//	 // scene.push_back(new cSceneObjectTransform(new cSphereSceneObject(cSphere(makeVector(0), r), new cPhongMaterial(Color(1, 1, 1)*.5, .3, 10)), cMatrix(1).Scaled(makeVector(1))*cMatrix(makeVector(.4, -1+r, .2))));
// }

