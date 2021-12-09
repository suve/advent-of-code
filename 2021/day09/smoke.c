#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 128
#define MAX_BASINS 512

void read_input(char *map, int *width, int *height) {	
	char buffer[1024];
	fgets(buffer, sizeof(buffer), stdin);

	*width = strlen(buffer) - 1; // fgets returns the trailing newline as well
	memcpy(map, buffer, *width);
	map += *width;

	*height = 1;
	while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
		const int buflen = strlen(buffer) - 1;
		if(buflen != *width) {
			fprintf(stderr, "Inconsistent width: %d vs %d chars\n", buflen, *width);
			exit(EXIT_FAILURE);
		}
		
		memcpy(map, buffer, *width);
		map += *width;

		*height += 1;
	}
}

int measure_basin_r(int x, int y, char *map, int width, int height) {
	const int offset = (y * width) + x;
	const int value = map[offset];

	int count = 1;
	map[offset] = 'x';

	#define IS_OK(neighbour_value) ((neighbour_value != 'x') && (neighbour_value != '9') && (value < neighbour_value))
	
	if((y > 0) && (IS_OK(map[offset - width]))) {
		count += measure_basin_r(x, y-1, map, width, height);
	}
	if((y < height - 1) && (IS_OK(map[offset + width]))) {
		count += measure_basin_r(x, y+1, map, width, height);
	}
	if((x > 0) && (IS_OK(map[offset - 1]))) {
		count += measure_basin_r(x-1, y, map, width, height);
	}
	if((x < width - 1) && (IS_OK(map[offset + 1]))) {
		count += measure_basin_r(x+1, y, map, width, height);
	}
	return count;
}

int measure_basin(int x, int y, char *map, int width, int height) {
	char mapcpy[width * height];
	memcpy(mapcpy, map, width * height);

	return measure_basin_r(x, y, mapcpy, width, height);
}

int main(void) {
	int width, height;
	char map[MAX_SIZE * MAX_SIZE];
	read_input(map, &width, &height);

	int basin_offset[MAX_BASINS];
	int basin_count = 0;
	
	int risk = 0;
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			const int offset = (y * width) + x;
			const char value = map[offset];

			// For a point to count as a low point, it needs to be lower
			// than all of its neighbours. Hence, if we find even that it's
			// higher (or equal) to even a single neighbour, we can skip it.
			if((y > 0) && (value >= map[offset - width])) continue;
			if((y < height - 1) && (value >= map[offset + width])) continue;
			if((x > 0) && (value >= map[offset - 1])) continue;
			if((x < width - 1) && (value >= map[offset + 1])) continue;

			risk += 1 + (value - '0');
			
			if(basin_count == MAX_BASINS) {
				fprintf(stderr, "Max number of basins reached (%d)\n", basin_count);
				exit(EXIT_FAILURE);
			}
			basin_offset[basin_count++] = offset;
		}
	}
	printf("Part 1: %d (from %d points)\n", risk, basin_count);

	int largest_basin[3] = {0, 0, 0};
	for(int i = 0; i < basin_count; ++i) {
		const int x = basin_offset[i] % width;
		const int y = basin_offset[i] / width;
		const int basin_size = measure_basin(x, y, map, width, height);

		if(basin_size < largest_basin[2]) continue;
		largest_basin[2] = basin_size;
		
		if(basin_size < largest_basin[1]) continue;
		largest_basin[2] = largest_basin[1];
		largest_basin[1] = basin_size;

		if(basin_size < largest_basin[0]) continue;
		largest_basin[1] = largest_basin[0];
		largest_basin[0] = basin_size;
	}

	printf(
		"Part 2: %d * %d * %d = %d\n",
		largest_basin[0],
		largest_basin[1],
		largest_basin[2],
		largest_basin[0] * largest_basin[1] * largest_basin[2]
	);
}
