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

void calc(const char *data, const uint primary_max, const uint secondary_max, uint(*const offset_func)(uint, uint, uint, uint), int *visible, uint *score) {
	for(uint pri = 0; pri < primary_max; ++pri) {
		char highest = '\0';

		uint pos_by_height['9' + 1];
		memset(pos_by_height, 0, sizeof(pos_by_height));

		for(uint sec = 0; sec < secondary_max; ++sec) {
			const uint offset = offset_func(pri, primary_max, sec, secondary_max);

			if(data[offset] > highest) {
				visible[offset] = 1;
				highest = data[offset];
			}

			score[offset] *= sec - pos_by_height[data[offset]];
			for(char c = '0'; c <= data[offset]; ++c) pos_by_height[c] = sec;
		}
	}
}

uint offset_left(uint y, uint rows, uint x, uint cols) {
	return (y * cols) + x;
}
uint offset_right(uint y, uint rows, uint x, uint cols) {
	return (y * cols) + (cols - x - 1);
}
uint offset_top(uint x, uint cols, uint y, uint rows) {
	return (y * cols) + x;
}
uint offset_bottom(uint x, uint cols, uint y, uint rows) {
	return ((rows - y - 1) * cols) + x;
}

int main(void) {
	uint rows, cols;
	char data[MAX_SIZE * MAX_SIZE];
	read_input(data, &rows, &cols);

	int visible[rows * cols];
	memset(visible, 0, sizeof(visible));

	uint score[rows * cols];
	for(uint i = 0; i < (rows * cols); ++i) score[i] = 1;

	calc(data, rows, cols, &offset_left, visible, score);
	calc(data, rows, cols, &offset_right, visible, score);
	calc(data, cols, rows, &offset_top, visible, score);
	calc(data, cols, rows, &offset_bottom, visible, score);

	uint count = 0;
	uint best = 0;
	for(uint i = 0; i < rows*cols; ++i) {
		if(visible[i]) ++count;
		if(score[i] > best) best = score[i];
	}

	printf("Part 1: %u\nPart 2: %u\n", count, best);
}
