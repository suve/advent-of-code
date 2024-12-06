#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Map {
	char *data;
	size_t capacity;
	int width;
	int height;
};

void read_input(struct Map *map) {
	map->width = 0;

	int line_no = 0;
	size_t offset = 0;
	char buffer[1024];
	while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
		size_t line_len = strlen(buffer);
		if(line_len == 0) continue;

		++line_no;
		if(map->width == 0) {
			map->width = line_len;
		} else if(map->width != line_len) {
			fprintf(
				stderr,
				"Error on line #%d: expected line to have %d chars, got %d\n",
				line_no, (int)(line_len), (int)(map->width)
			);
			exit(EXIT_FAILURE);
		}

		if(offset + line_len >= map->capacity) {
			fprintf(stderr, "Error: map capacity too small\n");
			exit(EXIT_FAILURE);
		}

		memcpy(map->data + offset, buffer, line_len);
		offset += line_len;
	}
	map->height = line_no;
}

enum Facing {
	FACE_UP,
	FACE_RIGHT,
	FACE_DOWN,
	FACE_LEFT
};

struct Guard {
	int x;
	int y;
	enum Facing facing;
};

struct Guard find_guard(struct Map *map) {
	struct Guard guard = (struct Guard){
		.x = -1,
		.y = -1,
		.facing = FACE_UP,
	};

	for(int y = 0; y < map->height; ++y) {
		for(int x = 0; x < map->width; ++x) {
			size_t offset = (y * map->width) + x;
			if(map->data[offset] == '^') {
				guard.x = x;
				guard.y = y;
				return guard;
			}
		}
	}
	fprintf(stderr, "Error: failed to find the guard!\n");
}

int simulate_path(struct Map *map, struct Guard *guard) {
	int path_len = 1;

	int distinct = 1;
	map->data[(guard->y * map->width) + guard->x] = 'x';

	while(1) {
		int new_x = guard->x;
		int new_y = guard->y;
		switch(guard->facing) {
			case FACE_UP: new_y -= 1; break;
			case FACE_RIGHT: new_x += 1; break;
			case FACE_DOWN: new_y += 1; break;
			case FACE_LEFT: new_x -= 1; break;
		}

		if((new_x < 0) || (new_x >= map->width)) return distinct;
		if((new_y < 0) || (new_y >= map->height)) return distinct;

		size_t offset = (new_y * map->width) + new_x;
		if(map->data[offset] == '#') {
			guard->facing = (guard->facing + 1) % 4;
		} else {
			guard->x = new_x;
			guard->y = new_y;
			++path_len;
			
			if(map->data[offset] != 'x') {
				++distinct;
				map->data[offset] = 'x';
			}
		}
	}
}

int main(void) {
	struct Map map;
	map.capacity = 1024 * 1024;
	map.data = malloc(map.capacity);

	read_input(&map);
	struct Guard guard = find_guard(&map);
	printf("Part1: %d\n", simulate_path(&map, &guard));
}
