#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.c"

struct Tile {
	int16_t owner;
	uint16_t distance;
};

#define PLANE_SIZE 2000L
#define PLANE_MEMSIZE (PLANE_SIZE * PLANE_SIZE * sizeof(struct Tile))

struct Tile *plane;
#define OFFSET(x, y) ((x) + ((y) * PLANE_SIZE))


struct Coord {
	uint16_t x, y;
	uint32_t space;
};

#define COORDS_MAX 256
struct Coord coords[COORDS_MAX];
int coord_count = 0;


struct Queue *q;

#define PART2_LIMIT 10000

void init_plane(void) {
	plane = malloc(PLANE_MEMSIZE);
	for(long int i = 0; i < PLANE_SIZE*PLANE_SIZE; ++i) {
		plane[i].owner = -1;
		plane[i].distance = 65535; // uint16_max
	}
}

void read_input(void) {
	int x, y;
	while(scanf("%d, %d\n", &x, &y) == 2) {
		coords[coord_count].x = x+500;
		coords[coord_count].y = y+500;
		coords[coord_count].space = 0;
		++coord_count;
	}
}

void flood(int coord_id, int x, int y, uint16_t dist) {
	struct Tile *tile = &(plane[OFFSET(x, y)]);
	
	// There's another coord closer - bail out
	if(dist > tile->distance) return;
	
	// Looks like we're tied with another coord.
	if(dist == tile->distance) {
		// We're tied with ourselves. Bail out.
		if(tile->owner == coord_id) return;
		
		// We're tied with multiple tiles. Fuck it, let's go home.
		if(tile->owner == -1) return;
		
		// Mark the tile as unowned
		tile->owner = -1;
	} else {
		// We are the closest coord - claim the tile
		tile->owner = coord_id;
		tile->distance = dist;
		printf("=> claimed tile %dx%d for #%d (distance: %d)\n", x, y, coord_id, dist);
	}
	
	if((x > 0) && (dist < plane[OFFSET(x-1,y)].distance))
		queue_push(q, coord_id, x-1, y, dist+1);
	
	if((x < PLANE_SIZE-1) && (dist < plane[OFFSET(x+1,y)].distance))
		queue_push(q, coord_id, x+1, y, dist+1);
	
	if((y > 0) && (dist < plane[OFFSET(x,y-1)].distance))
		queue_push(q, coord_id, x, y-1, dist+1);
	
	if((y < PLANE_SIZE-1) && (dist < plane[OFFSET(x,y+1)].distance))
		queue_push(q, coord_id, x, y+1, dist+1);
}

void count_tiles(void) {
	long int offset = 0;
	for(int y = 0; y < PLANE_SIZE; ++y) {
		for(int x = 0; x < PLANE_SIZE; ++x, ++offset) {
			if(plane[offset].owner < 0) continue;
			
			struct Coord *owner = &(coords[plane[offset].owner]);
			
			if((x == 0) || (x == PLANE_SIZE-1) || (y == 0) || (y == PLANE_SIZE-1)) {
				owner->space = UINT_MAX;
			} else {
				if(owner->space != UINT_MAX) owner->space += 1;
			}
		}
	}
}

int main(void) {
	init_plane();
	printf("--> plane init done\n");
	
	read_input();
	printf("--> input parsing finished\n");
	
	q = queue_init();
	for(int i = 0; i < coord_count; ++i) {
		queue_push(q, i, coords[i].x, coords[i].y, 0);
	}
	printf("--> performing flood fill...\n");
	while(q->count) {
		int16_t id, x, y;
		uint16_t dist;
		queue_pop(q, &id, &x, &y, &dist);
		
		if(dist > PLANE_SIZE*2) {
			fprintf(stderr, "!=!=> dist is %d, what the fuck?\n", dist);
			exit(1);
		}
		
		flood(id, x, y, dist);
		
		if(q->count % 1000 == 0) printf("--> %d tiles left in queue (last pop: %dx%d@%d)\n", q->count, x, y, dist);
	}
	queue_free(q);
	printf("--> flood fill done\n");
	
	count_tiles();
	printf("--> tile count done\n");
	
	int best_id = -1;
	uint32_t best_space = 0;
	for(int i = 0; i < coord_count; ++i) {
		if(coords[i].space != UINT_MAX) {
			printf("--> coord #%d: %d tiles\n", i, coords[i].space);
			if(coords[i].space > best_space) {
				best_space = coords[i].space;
				best_id = i;
			}
		} else {
			printf("--> coord #%d: INFINITE\n", i);
		}
	}
	
	if(best_id >= 0)
		printf("part1: coord #%d has %d tiles\n", best_id, best_space);
	else
		printf("part1: herp derp\n");
	
	uint32_t offset = 0;
	uint32_t region_size = 0;
	for(int y = 0; y < PLANE_SIZE; ++y) {
		for(int x = 0; x < PLANE_SIZE; ++x, ++offset) {
			uint32_t dist = 0;
			for(int c = 0; c < coord_count; ++c) dist += abs(x - coords[c].x) + abs(y - coords[c].y);
			if(dist < PART2_LIMIT) ++region_size;
		}
	}
	
	printf("part2: %ld\n", region_size);
}
