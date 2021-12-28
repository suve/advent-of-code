#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 160

struct Map {
	unsigned int width;
	unsigned int height;
	char data[MAX_SIZE * MAX_SIZE];
};

struct Map *read_input(void) {
	struct Map *map = malloc(sizeof(struct Map));
	memset(map, 0, sizeof(struct Map));
	
	char buffer[1024];
	while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
		unsigned int buflen = strlen(buffer) - 1; // Subtract trailing newline
		if(map->width > 0) {
			if(map->width != buflen) {
				fprintf(
					stderr,
					"Inconsistent input row length: expected %u bytes, got %u\n",
					map->width, buflen
				);
				exit(EXIT_FAILURE);
			}
		} else {
			if(buflen > MAX_SIZE) {
				fprintf(stderr, "Max width (%d) exceeded\n", MAX_SIZE);
				exit(EXIT_FAILURE);
			}
			map->width = buflen;
		}

		if(map->height == MAX_SIZE) {
			fprintf(stderr, "Max width (%d) exceeded\n", MAX_SIZE);
			exit(EXIT_FAILURE);
		}

		memcpy(map->data + (map->height * map->width), buffer, map->width);
		map->height += 1;
	}

	return map;
}

unsigned int calc_step(struct Map *map) {
	unsigned int count = 0;
	unsigned int datalen = map->width * map->height;
	
	for(unsigned int y = 0; y < map->height; ++y) {
		for(unsigned int x = 0; x < map->width; ++x) {
			const unsigned int offset = (y * map->width) + x;
			if(map->data[offset] != '>') continue;
			
			const unsigned int new_off = (y * map->width) + ((x + 1) % map->width);
			if(map->data[new_off] == '.') {
				map->data[offset] = 'x';
				map->data[new_off] = '!';
				++count;
			}
		}
	}
	for(unsigned int i = 0; i < datalen; ++i) {
		if(map->data[i] == '!')
			map->data[i] = '>';
		else if(map->data[i] == 'x')
			map->data[i] = '.';
	}

	for(unsigned int y = 0; y < map->height; ++y) {
		for(unsigned int x = 0; x < map->width; ++x) {
			const unsigned int offset = (y * map->width) + x;
			if(map->data[offset] != 'v') continue;
			
			const unsigned int new_off = (((y + 1) % map->height) * map->width) + x;
			if(map->data[new_off] == '.') {
				map->data[offset] = 'x';
				map->data[new_off] = '!';
				++count;
			}
		}
	}
	for(unsigned int i = 0; i < datalen; ++i) {
		if(map->data[i] == '!')
			map->data[i] = 'v';
		else if(map->data[i] == 'x')
			map->data[i] = '.';
	}

	return count;
}

int main(void) {
	struct Map *map = read_input();

	unsigned int steps;
	for(steps = 1; calc_step(map) != 0; ++steps);
	print(map);
	printf("Result: %d\n", steps);

	free(map);
	return 0;
}
