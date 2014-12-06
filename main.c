#include <stdlib.h>
#include <SDL.h>

#include "Scene.h"




const int WIDTH = 256;
const int HEIGHT = 256;


int main ( int argc, char** argv )
{
	// initialize SDL video
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "Unable to init SDL: %s\n", SDL_GetError() );
		return 1;
	}
	
	// make sure SDL cleans up before exit
	atexit(SDL_Quit);
	
	// create a new window
	SDL_Surface* screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32,
										   SDL_HWSURFACE|SDL_DOUBLEBUF);
	if ( !screen )
	{
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}



	Color* pixels = malloc(sizeof(Color)*WIDTH*HEIGHT);
	for(int y=0; y<HEIGHT; ++y){
		for(int x=0; x<WIDTH; ++x){
			pixels[x + y*WIDTH] = makeColorBlack();
		}
	}

	Scene scene = makeScene();
	buildRefractionBox(&scene);

	

	// program main loop
	bool done = false;
	int numPasses = 0;
	for (;;) {

		++numPasses;
		
		sceneGeneratePhotons(&scene, 3, 10);
		printf("Generated %i photons.\n", scene.photons.numValues);
		
		for(int y=0; y<HEIGHT; ++y){
			for(int x=0; x<WIDTH; ++x){
				int currentPixel = x + y*WIDTH;
				
				pixels[currentPixel] = cAdd(pixels[currentPixel], sceneTraceRayAtPixel(&scene, x + y*WIDTH, WIDTH, HEIGHT, 3));
				
				unsigned int *ptr = (unsigned int*)screen->pixels;
				int lineoffset = y * (screen->pitch / 4);
				Color color = csMul(pixels[currentPixel], 1.0/numPasses);
				
				ptr[lineoffset + x] = SDL_MapRGB(screen->format,
												 linearFloatToGammaEncodedUint8(color.red  , 1.8),
												 linearFloatToGammaEncodedUint8(color.green, 1.8),
												 linearFloatToGammaEncodedUint8(color.blue , 1.8));
			}
			

			
			SDL_Flip(screen);

			
			// Message processing loop
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				// Check for messages
				switch (event.type)
				{
					// Exit if the window is closed
					case SDL_QUIT: {
						
						done = true;
						break;
					}
						
					// Check for keypresses
					case SDL_KEYDOWN: {
						
						// Exit if ESCAPE is pressed
						if (event.key.keysym.sym == SDLK_ESCAPE)
							done = true;
						break;
					}
				}
			}
			
			// Stop rendering more lines.
			if (done) {
				break;
			}
		}

		// Stop rendering more passes.
		if (done) {
			break;
		}
	}
	
	
	// all is well ;)
	printf("Exited cleanly\n");
	return 0;
}
