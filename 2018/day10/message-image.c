#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL2/SDL.h"

struct Particle {
	int x, y;
	int vx, vy;
};

#define PARTICLES_MAX 512
struct Particle particle[PARTICLES_MAX];
int particles = 0;

void read_input(int argc, char **argv) {
	if(argc < 2) {
		fprintf(stderr, "Missing required argument: inputfile\n");
		exit(1);
	}
	
	FILE *f = fopen(argv[1], "r");
	if(f == NULL) {
		perror("Failed to open input file");
		exit(1);
	}
	
	int pos_x, pos_y, vel_x, vel_y;
	while(fscanf(f, "position=<%d, %d> velocity=<%d, %d>\n", &pos_x, &pos_y, &vel_x, &vel_y) == 4) {
		particle[particles++] = (struct Particle){
			.x = pos_x,
			.y = pos_y,
			.vx = vel_x,
			.vy = vel_y
		};
	}
	
	fclose(f);
}

void advance(int count) {
	for(int p = 0; p < particles; ++p) {
		particle[p].x += particle[p].vx * count;
		particle[p].y += particle[p].vy * count;
	}
}

#define WINDOW_W 1880
#define WINDOW_H 1000

void openwindow(SDL_Window **Wind, SDL_Renderer **Rend) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	
	*Wind = SDL_CreateWindow(
		"advent-of-code-2018/day10",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_W, WINDOW_H,
		0
	);

	*Rend = SDL_CreateRenderer(
		*Wind,
		-1,
		0
	);
}

void draw(SDL_Renderer *Rend) {
	SDL_SetRenderDrawColor(Rend, 0, 0, 0, 255);
	SDL_RenderClear(Rend);
	
	SDL_SetRenderDrawColor(Rend, 0, 255, 0, 255);
	for(int p = 0; p < particles; ++p) {
		SDL_RenderDrawPoint(Rend, particle[p].x, particle[p].y);
	}
	
	SDL_RenderPresent(Rend);
}

int main(int argc, char **argv) {
	read_input(argc, argv);
	
	SDL_Window *Wind;
	SDL_Renderer *Rend;
	openwindow(&Wind, &Rend);
	
	long int steps = 0;
	
	int quit = 0;
	int autoplay = 1;
	while(!quit) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev) > 0) {
			if(ev.type == SDL_QUIT) {
				quit = 1;
				continue;
			}
			
			if(ev.type != SDL_KEYDOWN) continue;
			
			switch(ev.key.keysym.sym) {
				case SDLK_SPACE:
					autoplay = !autoplay;
					break;
				
				case SDLK_LEFT:
					advance(-1); --steps;
					break;
				
				case SDLK_RIGHT:
					advance(+1); ++steps;
					break;
				
				case SDLK_ESCAPE:
					quit = 1;
					break;
			}
		}
		
		draw(Rend);
		if(autoplay) advance(+1), ++steps;
	}
	
	printf("quit at %ld steps\n", steps);
	
	SDL_DestroyRenderer(Rend);
	SDL_DestroyWindow(Wind);
	SDL_Quit();
}
