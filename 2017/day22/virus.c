#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID_HALF 10000
#define GRID_SIZE (2 * GRID_HALF + 1)
#define OFFSET(x, y)  ((y) * GRID_SIZE + (x))

char grid[GRID_SIZE * GRID_SIZE];
long int bursts_total;


void print_grid(const int size) {
	int x = GRID_HALF - (size / 2);
	int y = GRID_HALF - (size / 2);
	
	char buffer[size+1];
	for(int i = 0; i < size; ++i) {
		long int offset = OFFSET(x, y+i);
		
		memcpy(buffer, grid + offset, size);
		buffer[size] = '\0';
		
		puts(buffer);
	}
}


void process_args(int argc, char **argv) {
	if(argc < 3) {
		fprintf(stderr, "You must provide the number of bursts to process: \"--bursts X\"\n");
		exit(EXIT_FAILURE);
	}
	
	if(strcmp(argv[1], "--bursts") == 0) {
		bursts_total = atoi(argv[2]);
		if(bursts_total < 1) {
			fprintf(stderr, "The number of bursts must be at least 1\n");
			exit(EXIT_FAILURE);
		}
	} else {
		fprintf(stderr, "Unknown option \"%s\"\n", argv[1]);
		exit(EXIT_FAILURE);
	}
}

void init_buffer(void) {
	memset(grid, '.', GRID_SIZE * GRID_SIZE);
}

void process_input(void) {
	int lineNo = 0;
	
	char buffer[128];
	while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
		int buflen = strlen(buffer);
		if(buffer[buflen-1] == '\n') buffer[--buflen] = '\0';
		
		int x = GRID_HALF - (buflen / 2);
		int y = GRID_HALF - (buflen / 2) + lineNo;
		long int offset = OFFSET(x, y);
		memcpy(grid + offset, buffer, buflen);
		
		++lineNo;
	}
}


enum Direction {
	DIR_UP,
	DIR_RI,
	DIR_DO,
	DIR_LE
};
void turn_left(enum Direction *const dir) {
	*dir = (*dir == DIR_UP) ? DIR_LE : (*dir - 1); 
}
void turn_right(enum Direction *const dir) {
	*dir = (*dir + 1) % 4;
}

void go_forward(int *const x, int *const y, const enum Direction *const dir) {
	if(*dir == DIR_UP)
		*y -= 1;
	if(*dir == DIR_RI)
		*x += 1;
	if(*dir == DIR_DO)
		*y += 1;
	if(*dir == DIR_LE)
		*x -= 1;
}

int main(int argc, char **argv) {
	process_args(argc, argv);
	
	init_buffer();
	process_input();
	
	int vx = GRID_HALF;
	int vy = GRID_HALF;
	enum Direction vd = DIR_UP;
	
	long int bursts_infected = 0;
	for(long int b = 0; b < bursts_total; ++b) {
		long int offset = OFFSET(vx, vy);
		
		if(grid[offset] == '#') {
			turn_right(&vd);
			grid[offset] = '.';
		} else {
			turn_left(&vd);
			grid[offset] = '#';
			++bursts_infected;
		}
		
		go_forward(&vx, &vy, &vd);
	}
	
	print_grid(9);
	printf("> %ld new infections\n", bursts_infected);
}
