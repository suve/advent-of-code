#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 256u

typedef unsigned int uint;

void read_input(char *data, uint *rows, uint *cols) {
	*rows = 0;
	*cols = 0;

	char buffer[1024];
	while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
		uint len = strlen(buffer);
		if(buffer[len-1] == '\n') {
			buffer[--len] = '\0';
		}

		// Ignore empty lines
		if(len == 0) continue;

		if(len > MAX_SIZE) {
			fprintf(stderr, "Error: Map row too long (max %u columns allowed)\n", MAX_SIZE);
			exit(EXIT_FAILURE);
		}

		if(*cols != 0) {
			if(*cols != len) {
				fprintf(stderr, "Error: Map rows are of uneven length (%u vs %u)\n", *cols, len);
				exit(EXIT_FAILURE);
			}
		} else {
			*cols = len;
		}

		if(*rows == MAX_SIZE) {
			fprintf(stderr, "Error: Too many map rows (max %u rows allowed)\n", MAX_SIZE);
			exit(EXIT_FAILURE);
		}
		*rows += 1;

		memcpy(data, buffer, len);
		data += len;
	}
}

int main(void) {
	uint rows, cols;
	char data[MAX_SIZE * MAX_SIZE];
	read_input(data, &rows, &cols);

	int visible[rows * cols];
	memset(visible, 0, sizeof(visible));

	for(uint y = 0; y < rows; ++y) {
		// Check visibility from the left
		{
			char highest = '\0'; // cheeky
			for(uint x = 0; x < cols; ++x) {
				uint offset = (y*cols) + x;
				if(data[offset] > highest) {
					visible[offset] = 1;
					highest = data[offset];
				}
			}
		}

		// Check visibility from the right
		{
			char highest = '\0'; // cheeky
			for(uint x = cols; x > 0; --x) {
				uint offset = (y*cols) + (x - 1);
				if(data[offset] > highest) {
					visible[offset] = 1;
					highest = data[offset];
				}
			}
		}
	}

	for(uint x = 0; x < cols; ++x) {
		// Check visibility from the top
		{
			char highest = '\0'; // cheeky
			for(uint y = 0; y < rows; ++y) {
				uint offset = (y*cols) + x;
				if(data[offset] > highest) {
					visible[offset] = 1;
					highest = data[offset];
				}
			}
		}

		// Check visibility from the bottom
		{
			char highest = '\0'; // cheeky
			for(uint y = rows; y > 0; --y) {
				uint offset = ((y-1)*cols) + x;
				if(data[offset] > highest) {
					visible[offset] = 1;
					highest = data[offset];
				}
			}
		}
	}

	uint count = 0;
	for(uint i = 0; i < rows*cols; ++i) {
		if(visible[i]) ++count;
	}

	printf("Part 1: %u\n", count);
}
