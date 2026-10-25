#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> //gives access to uint32_t type

#define WIDTH 320
#define HEIGHT 200
#define PITCH WIDTH*sizeof(uint32_t) //pitch = width of texture * size of 1px (in B) (here, 320 * 4B)
#define WSTRETCHED WIDTH*4
#define HSTRETCHED HEIGHT*4
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF

uint32_t framebuffer[WIDTH*HEIGHT]; //unsigned 32b(=4B) int (2^32 instead of 2^31 if signed)

uint32_t frame = 0x000000;

/*
	Error handling: 
	SDL functions return 0 if everything went well, an error code otherwise

	SDL allocation:
	SDL_init is kinda like a malloc, so it needs SDL_Quit() kinda like free().
	Same goes for texture, renderer and window
*/
int errorHandling(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture){
	if(window == NULL || renderer == NULL || texture == NULL){
		if(window!=NULL) SDL_DestroyTexture(texture);
		if(renderer!=NULL) SDL_DestroyRenderer(renderer);
		if(texture!=NULL) SDL_DestroyWindow(window);
	
		fprintf(stderr, "SDL error: component(s) declaration failed:");
		SDL_Quit(); //free the sdl init
		return EXIT_FAILURE;
	}
}

// framebuffer-related functions
/* putPixel
	puts a pixel with the color of the specified HEX code at the specified pixel
	@returns EXIT_FAILURE if coordinates are out of the framebuffer boundaries
*/
int putPixel(int x, int y, uint32_t color){
		if(x<=0 || x>WIDTH || y<=0 || y>HEIGHT){
			fprintf(stderr, "Pixel coordinates are out of bound, current resolution is: %dx%d\n", WIDTH, HEIGHT);
			printf("x: %d, y: %d", x, y);
			return EXIT_FAILURE;
		}
		x-=1;
		y-=1;
		framebuffer[y*WIDTH + x] = color;
}

void clearFramebuffer(){
	for(int i = 0; i<WIDTH*HEIGHT; i++){
		framebuffer[i] = 0x000000;
	}
}

int main(void){
	if (!SDL_Init(SDL_INIT_VIDEO)){ //initialize SDL
		fprintf(stderr, "SDL_Init failed: %s \n", SDL_GetError());
		return EXIT_FAILURE;
	} 
	SDL_Window *window; //Create a window
	SDL_Renderer *renderer; //visible renderer to draw inside the window
	SDL_Texture *texture; //what to display
	SDL_Event event; //event (from user)

	window = SDL_CreateWindow("SDL Framebuffer", WSTRETCHED, HSTRETCHED, 0); //0 => no flags
	renderer = SDL_CreateRenderer(window, NULL);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	/* texture:
	The pixel format (2nd arg) specifies the color format to be used for the pixels. Here: RGB with 8 bits for each color
	The 3rd arg specifies if the texture can be updated (streaming=yes)
	*/
	errorHandling(window, renderer, texture); //checking if there was any error
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST); //stretch window
	

	bool is_running = true;
	//Display the window
	while(is_running){

		uint64_t start = SDL_GetPerformanceCounter(); //measuring the start with micro/nano s precision

		//Get the events from the user
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_EVENT_QUIT){
				is_running = false;
			}
		}

		//Update pixels in framebuffer

		



		//some error handling would be nice. But i want to keep this clean as this program is for learning purposes
		//update and display everything
		SDL_UpdateTexture(texture, NULL, framebuffer, PITCH); //2nd arg = dimensions to update, NULL = update everything, last arg = pitch (see macros)
		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, texture, NULL, NULL); //3rd arg = dimensions to update 4th = dimensions to display 
		SDL_RenderPresent(renderer); //actually displays the renderer

		uint64_t end = SDL_GetPerformanceCounter(); //measuring the end with micro/nano s precision
		double tickTime = (double)(end-start) / (double)(SDL_GetPerformanceFrequency()); //getPerformanceFrequency measures the frequency/s of which getPerformanceCounter is adjusted

		if(tickTime<0.016){ //more than 60fps
			SDL_Delay((0.016 - tickTime)*1000.0); //delay in ms
		}
		frame++;
	}

	//Free everything from memory
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
