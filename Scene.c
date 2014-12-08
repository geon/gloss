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
#include "SceneObjectTransform.h"
#include "SceneObjectUnitPlane.h"
#include "SceneObjectPlane.h"
#include "SceneObjectBox.h"
#include "SceneObjectCylinder.h"
#include "bool.h"
#include "MaterialDiffuse.h"
#include "MaterialLamp.h"
#include "MaterialPhong.h"
#include "MaterialTransparent.h"
#include "MaterialCheckered.h"


Scene makeScene () {

	return (Scene) {makeSceneObjectPointerContainer(1000), makeMaterialPointerContainer(10), makePhotonEndPointContainer(1000), makeMatrixIdentity(), makeColor(0.5, 0.7, 1), 0.8};
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
	
	containerForeach (SceneObject *, object, scene->objects) {
		
		// TODO: Distribute photons based on light intensity.
		
		// Only emit photons from object with radiance.
		if (!cEqual(sceneObjectRadiantFlux(*object), makeColorBlack())) {

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
		collectedLight = cAdd(collectedLight, cMul(materialIrradience(intersection.material /*, intersection.position, vNegated(ray.direction) */), bouncedRay.energy));

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
			if(occluder.hitType == missed || occluder.distance > lightDistance - vEpsilon){

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
		
		// When hitting a perfectly black surface, there is no point in tracing further.
		if (cEqual(bouncedRay.energy, makeColorBlack())) {
			break;
		}
	}

	// Return the average of all sampled paths.
	return csMul(collectedLight, 1.0/numSampledCameraRayBounces);
}






Intersection sceneIntersectRay(const Scene scene, const Ray ray) {

	// Find the object with the closest intersection, or return  a miss.
	
	Intersection closestIntersection;
	closestIntersection.hitType = missed;
	float minDistance = INFINITY;

	containerForeach (SceneObject *, object, scene.objects) {

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
	scene->cameraOrientation = makeMatrixTranslation(makeVector(0, 0, 3.74));

	
	
	Material *whiteMaterial = *materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialDiffuse(makeMaterialDiffuse(makeColorLightness(scene->standardReflectivity))));
	Material *lampMaterial  = *materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialLamp(makeMaterialLamp(makeColorLightness(12))));
	
	
	// Lights
	scene->skyColor = makeColorBlack();
//	float lr = 0.2;
//	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectSphere(makeSceneObjectSphere(makeSphere(makeVector(0, 1-lr - 0.05, 0), lr), lampMaterial)));
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectTransform(makeSceneObjectTransform(
		mMul(makeMatrixTranslation(makeVector(0, 1-vEpsilon, 0)), makeMatrixScale(.5)),
		(SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(makePlane(makeVector(0, -1, 0), 0), lampMaterial))
	)));
	

	// Ball
	float br = 0.3;
	sceneObjectPointerContainerAddValue(&scene->objects,
		(SceneObject *) allocateSceneObjectSphere(makeSceneObjectSphere(
			makeSphere(makeVector(-.4, -1+br, -.4), br),
			*materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialPhong(makeMaterialPhong(csMul(makeColor(1, .9, .1), scene->standardReflectivity), 0.2, 10000, 0)))

		))
	);

	// Boxes
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectTransform(makeSceneObjectTransform(
		mMul(makeMatrixAxisAngle(makeVector(0, 1, 0), .3), makeMatrixTranslation(makeVector(-.3, -.3, .3))),
		(SceneObject *) allocateSceneObjectBox(makeSceneObjectBox(
			makeVector(.3, .7, .3),
			whiteMaterial
		))
	)));
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectTransform(makeSceneObjectTransform(
		mMul(makeMatrixAxisAngle(makeVector(0, 1, 0), -.3), makeMatrixTranslation(makeVector(.3, -.75, -.3))),
		(SceneObject *) allocateSceneObjectBox(makeSceneObjectBox(
			makeVector(.3, .3, .3),
			whiteMaterial
		))
	)));

	
	// Walls
	
	// Floor
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(0, 1, 0), -1+vEpsilon),
		whiteMaterial
	)));
	// Ceiling
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(0, -1, 0), -1+vEpsilon),
		whiteMaterial
	)));
	// Middle
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(0, 0, -1), -1+vEpsilon),
		whiteMaterial
	)));
	// Left
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(1, 0, 0), -1+vEpsilon),
		*materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialDiffuse(makeMaterialDiffuse(csMul(makeColor(1, 0.5, 0.1), scene->standardReflectivity))))
	)));
	// Right
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(-1, 0, 0), -1+vEpsilon),
		*materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialDiffuse(makeMaterialDiffuse(csMul(makeColor(0.1, 0.3, 0.5), scene->standardReflectivity))))
	)));
 
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



void buildSpherePhotonSpawnTest(Scene *scene) {
	
	// Camera
	scene->cameraOrientation = makeMatrixTranslation(makeVector(0, 0, 3.8));
	
	
	
	Material *blackMaterial = *materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialLamp(makeMaterialLamp(makeColorLightness(0))));
	Material *whiteMaterial  = *materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialLamp(makeMaterialLamp(makeColorLightness(1))));
	
	
	// Lights
//	scene->skyColor = makeColorLightness(0.5);
	SceneObjectSphere lamp = makeSceneObjectSphere(makeSphere(makeVectorOrigo(), 1), whiteMaterial);
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectSphere(lamp));
	

	int numPhotons = 300;
	PhotonContainer photons = makePhotonContainer(numPhotons);
	sceneObjectSphereEmitPhotons((SceneObject *) &lamp, numPhotons, &photons);
	
	containerForeach(Photon, photon, photons) {

		sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectSphere(makeSceneObjectSphere(
			makeSphere(photon->heading.origin, .02),
			blackMaterial
		)));
	}

	
}


void buildPaintBox(Scene *scene) {
	
	// Camera
	scene->cameraOrientation = makeMatrixTranslation(makeVector(0, 0.5, 1.8));
	
	
	
	Material *paintMaterial = *materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialPhong(makeMaterialPhong(csMul(makeColor(1, .9, .1), scene->standardReflectivity), 0.2, 10000, 0)));
	Material *whiteMaterial = *materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialDiffuse(makeMaterialDiffuse(makeColorLightness(scene->standardReflectivity))));
	Material *lampMaterial  = *materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialLamp(makeMaterialLamp(makeColorLightness(7))));
	
	
	// Lights
	scene->skyColor = makeColorBlack();

	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectTransform(makeSceneObjectTransform(
		mMul(makeMatrixTranslation(makeVector(0, 1-vEpsilon, 0)), makeMatrixScale(.5)),
		(SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
			makePlane(makeVector(0, -1, 0), 0),
			lampMaterial
		))
	)));

	
	// Ball
	float br = 0.5;
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectSphere(makeSceneObjectSphere(makeSphere(makeVector(0, -1+br, 0), br), paintMaterial)));

	
	// Walls
	
	// Floor
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(0, 1, 0), -1+vEpsilon),
		whiteMaterial
	)));
	// Ceiling
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(0, -1, 0), -1+vEpsilon),
		whiteMaterial
	)));
	// Back
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(0, 0, -1), -1+vEpsilon),
		whiteMaterial
	)));
	// Front
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(0, 0, 1), -1+vEpsilon),
		whiteMaterial
	)));
	// Left
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(1, 0, 0), -1+vEpsilon),
		*materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialDiffuse(makeMaterialDiffuse(csMul(makeColor(1, 0.5, 0.1), scene->standardReflectivity))))
	)));
	// Right
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(-1, 0, 0), -1+vEpsilon),
		*materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialDiffuse(makeMaterialDiffuse(csMul(makeColor(0.5, 1, 0.1), scene->standardReflectivity))))

	)));
	
}


 
void buildRefractionBox(Scene *scene) {
	
	// Camera
	scene->cameraOrientation = mMul(mMul(makeMatrixTranslation(makeVector(0, 0, 3)), makeMatrixAxisAngle(makeVector(1, 0, 0), -0.3)), makeMatrixAxisAngle(makeVector(0, 1, 0), -0.5));
	
	
	Material *whiteMaterial = *materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialDiffuse(makeMaterialDiffuse(makeColorLightness(scene->standardReflectivity))));
	Material *blackMaterial = *materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialDiffuse(makeMaterialDiffuse(makeColorLightness(scene->standardReflectivity * 0.1))));
	Material *checkeredMaterial = *materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialCheckered(makeMaterialCheckered(whiteMaterial, blackMaterial)));

	Material *glass = *materialPointerContainerAddValue(
		&scene->materials,
		(Material *) allocateMaterialTransparent(makeMaterialTransparent(
			makeColorLightness(scene->standardReflectivity*0), // reflectivity
			1, // specularity
			10000, // exponent
			0, // metallic
			1, // transparency
			1.5, // IOR
			makeColorLightness(0.5) // halfAbsorbDistance
		))
	);
	
	Material *lampMaterial = *materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialLamp(makeMaterialLamp(makeColorLightness(5))));
	
	
	// Lights
	scene->skyColor = makeColorLightness(0);
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectTransform(makeSceneObjectTransform(
																															   mMul(makeMatrixTranslation(makeVector(0, 1-vEpsilon, 0)), makeMatrixScale(.5)),
																															   (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(makePlane(makeVector(0, -1, 0), 0), lampMaterial))
																															   )));
	
	
	// Ball
	float br = 0.6;
	sceneObjectPointerContainerAddValue(
		&scene->objects,
		(SceneObject *) allocateSceneObjectSphere(makeSceneObjectSphere(
			makeSphere(makeVector(0, 0, 0), br),
			glass
		))
	);
	
	// Floor
	sceneObjectPointerContainerAddValue(
		&scene->objects,
		(SceneObject *) allocateSceneObjectPlane(makeSceneObjectPlane(
			makePlane(makeVector(0, 1, 0), -br),
			checkeredMaterial
		))
	);
}


void buildCornellCylinderBox(Scene *scene) {
	
	// Camera
	scene->cameraOrientation = makeMatrixTranslation(makeVector(0, 0, 3.74));
	
	Material *whiteMaterial = *materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialDiffuse(makeMaterialDiffuse(makeColorLightness(scene->standardReflectivity))));
	Material *lampMaterial  = *materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialLamp(makeMaterialLamp(makeColorLightness(8))));
	
	// Lights
	scene->skyColor = makeColorBlack();
	//	float lr = 0.2;
	//	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectSphere(makeSceneObjectSphere(makeSphere(makeVector(0, 1-lr - 0.05, 0), lr), lampMaterial)));
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectTransform(makeSceneObjectTransform(
		mMul(makeMatrixTranslation(makeVector(0, 1-vEpsilon, 0)), makeMatrixScale(.5)),
		(SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(makePlane(makeVector(0, -1, 0), 0), lampMaterial))
	)));
	
	
	// Cylinders
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectCylinder(makeSceneObjectCylinder(makeVector(-.3, .4, .3), makeVector(-.3, -1, .3), .3, whiteMaterial)));
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectCylinder(makeSceneObjectCylinder(makeVector(.3, -.4, -.3), makeVector(.3, -1, -.3), .3, whiteMaterial)));

	
	
	// Walls
	
	// Floor
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(0, 1, 0), -1+vEpsilon),
		whiteMaterial
	)));
	// Ceiling
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(0, -1, 0), -1+vEpsilon),
		whiteMaterial
	)));
	// Middle
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(0, 0, -1), -1+vEpsilon),
		whiteMaterial
	)));
	// Left
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(1, 0, 0), -1+vEpsilon),
		*materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialDiffuse(makeMaterialDiffuse(csMul(makeColor(1, 0.5, 0.1), scene->standardReflectivity))))
	)));
	// Right
	sceneObjectPointerContainerAddValue(&scene->objects, (SceneObject *) allocateSceneObjectUnitPlane(makeSceneObjectUnitPlane(
		makePlane(makeVector(-1, 0, 0), -1+vEpsilon),
		*materialPointerContainerAddValue(&scene->materials, (Material *) allocateMaterialDiffuse(makeMaterialDiffuse(csMul(makeColor(0.1, 0.3, 0.5), scene->standardReflectivity))))
	)));
	
}
