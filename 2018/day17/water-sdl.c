#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#define SIZE 2000

#define SPRING_X 500
#define SPRING_Y 0

enum Tile {
	TILE_EMPTY,
	TILE_CLAY,
	TILE_WATER,
	TILE_FLOWING,
};

int** alloc_map(void) {
	const int map_size = SIZE * SIZE * sizeof(int);
	int *map = malloc(map_size);
	if(map == NULL) {
		perror("Failed to alloc map");
		exit(EXIT_FAILURE);
	}

	int **rows = malloc(SIZE * sizeof(int*));
	if(rows == NULL) {
		perror("Failed to alloc rows");
		exit(EXIT_FAILURE);
	}

	memset(map, 0, map_size);
	for(int i = 0; i < SIZE; ++i) {
		rows[i] = map + (i*SIZE);
	}

	return rows;
}

struct MapSize {
	int x_min, x_max;
	int y_min, y_max;
};

struct MapSize read_input(int **map) {
	struct MapSize ms = {
		.x_min = SIZE,
		.x_max = 0,
		.y_min = SIZE,
		.y_max = 0,
	};

	char axis, unused;
	int pos, start, end;
	while(scanf("%c=%d, %c=%d..%d\n", &axis, &pos, &unused, &start, &end) == 5) {
		if(axis == 'x') {
			for(int y = start; y <= end; ++y) {
				map[y][pos] = TILE_CLAY;
			}
			if(pos < ms.x_min) ms.x_min = pos;
			if(pos > ms.x_max) ms.x_max = pos;
			if(start < ms.y_min) ms.y_min = start;
			if(end > ms.y_max) ms.y_max = end;
		} else if(axis == 'y') {
			for(int x = start; x <= end; ++x) {
				map[pos][x] = TILE_CLAY;
			}
			if(start < ms.x_min) ms.x_min = start;
			if(end > ms.x_max) ms.x_max = end;
			if(pos < ms.y_min) ms.y_min = pos;
			if(pos > ms.y_max) ms.y_max = pos;
		} else {
			fprintf(stderr,
				"Failed to parse line: %c=%d, %c=%d..%d\n",
				axis, pos, unused, start, end
			);
		}
	}
	return ms;
}

void open_window(const struct MapSize *ms, const int scale, SDL_Window **win, SDL_Renderer **ren) {
	// +2 so we can see one tile to the left and one to the right
	const int window_w = ms->x_max - ms->x_min + 1 + 2;
	const int window_h = (ms->y_max + 1 < 1500) ? (ms->y_max + 1) : 1500;

	if(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO)) {
		fprintf(stderr, "SDL2 init failed: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	if(SDL_CreateWindowAndRenderer(window_w * scale, window_h * scale, 0, win, ren)) {
		fprintf(stderr, "Failed to open SDL2 window: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_RenderSetLogicalSize(*ren, window_w, window_h);
}

void drawFrame(SDL_Renderer *ren, int **map, const struct MapSize *ms, const int cameraY) {
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderClear(ren);

	for(int y = (cameraY > 0) ? cameraY : 0; y <= ms->y_max; ++y) {
		for(int x = ms->x_min - 1; x <= ms->x_max + 1; ++x) {
			switch(map[y][x]) {
				case TILE_CLAY:
					SDL_SetRenderDrawColor(ren, 127, 127, 127, 255);
				break;
				case TILE_FLOWING:
					SDL_SetRenderDrawColor(ren, 0, 195, 255, 255);
				break;
				case TILE_WATER:
					SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
				break;
				case TILE_EMPTY:
					goto skip;
			}

			int pixel_x = x - (ms->x_min - 1);
			SDL_RenderDrawPoint(ren, pixel_x, y - cameraY);

			skip: ;
		}
	}

	SDL_RenderPresent(ren);
}

void process_events(int *quit, int *pause, int *cameraY) {
	SDL_Event ev;
	while(SDL_PollEvent(&ev)) {
		if(ev.type == SDL_QUIT)
			*quit = 1;
		else if(ev.type == SDL_KEYDOWN) {
			if(ev.key.keysym.sym == SDLK_ESCAPE)
				*quit = 1;
			else if(ev.key.keysym.sym == SDLK_SPACE)
				*pause = !*pause;
		} else if(ev.type == SDL_MOUSEWHEEL) {
			*cameraY -= ev.wheel.y;
		}
	}
}

void parse_options(int argc, char **argv, int *scale, int *timestep) {
	*scale = 1;
	*timestep = 50;

	int i = 1;
	while(i < argc) {
		if(strcmp(argv[i], "--scale") == 0) {
			if(i + 1 == argc) {
				fprintf(stderr, "Error: the --scale option requires an argument\n");
				exit(EXIT_FAILURE);
			}
			*scale = atoi(argv[i+1]);
			i += 2;
		} else if(strcmp(argv[i], "--timestep") == 0) {
			if(i + 1 == argc) {
				fprintf(stderr, "Error: the --scale option requires an argument\n");
				exit(EXIT_FAILURE);
			}
			*timestep = atoi(argv[i+1]);
			i += 2;
		} else {
			fprintf(stderr, "Error: unexpected command-line argument \"%s\"\n", argv[i]);
			exit(EXIT_FAILURE);
		}
	}
}

struct FlowGroup {
	int active;
	int x_min, x_max, y;
	struct FlowGroup *child_left, *child_right;
};

struct FlowGroup* flowGroup_new(const int x, const int y) {
	struct FlowGroup *fg = malloc(sizeof(struct FlowGroup));
	*fg = (struct FlowGroup) {
		.active = 1,
		.x_min = x,
		.x_max = x,
		.y = y,
		.child_left = NULL,
		.child_right = NULL,
	};
	return fg;
}

int flowGroup_process(struct FlowGroup *fg, int **map, const struct MapSize *ms) {
	// groups below the max_y are active indefinitely
	if(fg->y > ms->y_max) return 0;

	int changes = 0;
	int childChanges = 0;

	// Check current child groups
	if(fg->child_left) {
		childChanges += flowGroup_process(fg->child_left, map, ms);
		if(!fg->child_left->active) {
			free(fg->child_left);
			fg->child_left = NULL;
		}
	}

	if(fg->child_right) {
		childChanges += flowGroup_process(fg->child_right, map, ms);
		if(!fg->child_right->active) {
			free(fg->child_right);
			fg->child_right = NULL;
		}
	}

	// Try to create a new child groups, or expand left/right
	while(!fg->child_left) {
		const enum Tile under_min = map[fg->y + 1][fg->x_min];
		if((under_min == TILE_CLAY) || (under_min == TILE_WATER)) {
			const enum Tile left = map[fg->y][fg->x_min - 1];
			if((left == TILE_EMPTY) || (left == TILE_FLOWING)) {
				// printf("Expanded from %d-%d:%d to %d-%d:%d\n", fg->x_min, fg->x_max, fg->y, fg->x_min-1, fg->x_max, fg->y);
				map[fg->y][fg->x_min - 1] = TILE_FLOWING;
				fg->x_min -= 1;
				changes = 1;
			} else {
				break;
			}
		} else if(under_min == TILE_EMPTY) {
			fg->child_left = flowGroup_new(fg->x_min, fg->y+1);
			map[fg->y+1][fg->x_min] = TILE_FLOWING;
			changes = 1;
		} else {
			break;
		}
	}

	while(!fg->child_right) {
		const enum Tile under_max = map[fg->y + 1][fg->x_max];
		if((under_max == TILE_CLAY) || (under_max == TILE_WATER)) {
			const enum Tile right = map[fg->y][fg->x_max + 1];
			if((right == TILE_EMPTY) || (right == TILE_FLOWING)) {
				// printf("Expanded from %d-%d:%d to %d-%d:%d\n", fg->x_min, fg->x_max, fg->y, fg->x_min, fg->x_max+1, fg->y);
				map[fg->y][fg->x_max + 1] = TILE_FLOWING;
				fg->x_max += 1;
				changes = 1;
			} else {
				break;
			}
		} else if((under_max == TILE_EMPTY) && (fg->x_min != fg->x_max)) {
			fg->child_right = flowGroup_new(fg->x_max, fg->y + 1);
			map[fg->y+1][fg->x_max] = TILE_FLOWING;
			changes = 1;
		} else {
			break;
		}
	}

	if(!fg->child_left && !fg->child_right) {
		fg->active = 0;
		int under_flowing = 0;
		for(int x = fg->x_min; x <= fg->x_max; ++x) {
			const enum Tile under = map[fg->y + 1][x];
			if(under == TILE_FLOWING) {
				under_flowing = 1;
				break;
			}
		}
		if(!under_flowing) {
			for(int x = fg->x_min; x <= fg->x_max; ++x) {
				map[fg->y][x] = TILE_WATER;
			}
		}
		changes = 1;
	}

	return changes + childChanges;
}

int main(int argc, char **argv) {
	int scale, timestep;
	parse_options(argc, argv, &scale, &timestep);

	int **map = alloc_map();
	struct MapSize ms = read_input(map);

	SDL_Window *win;
	SDL_Renderer *ren;
	open_window(&ms, scale, &win, &ren);
	drawFrame(ren, map, &ms, 0);

	struct FlowGroup *spring = flowGroup_new(SPRING_X, SPRING_Y);
	map[SPRING_Y][SPRING_X] = TILE_FLOWING;

	int quit = 0;
	int pause = 0;
	int camera_y = 0;
	while(!quit) {
		SDL_Delay(timestep);

		process_events(&quit, &pause, &camera_y);
		if(pause) continue;

		int changes = flowGroup_process(spring, map, &ms);
		if(changes == 0) {
			printf("No more changes - stopping\n");
			SDL_Delay(5000);
			break;
		}

		drawFrame(ren, map, &ms, camera_y);
	}

	int flowing_tiles = 0;
	int still_tiles = 0;
	for(int y = ms.y_min; y <= ms.y_max; ++y) {
		for(int x = ms.x_min - 1; x <= ms.x_max + 1; ++x) {
			switch(map[y][x]) {
				case TILE_FLOWING:
					flowing_tiles += 1;
				break;
				case TILE_WATER:
					still_tiles += 1;
				break;
			}
		}
	}

	printf("%d water tiles total - %d still tiles and %d flowing tiles\n", still_tiles + flowing_tiles, still_tiles, flowing_tiles);
}
