#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 128

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

int offset(int x, int y, int width) {
	return x + (y * width);
}

int main(void) {
	int width, height;
	char map[MAX_SIZE * MAX_SIZE];
	read_input(map, &width, &height);

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
		}
	}
	printf("Part 1: %d\n", risk);
}
