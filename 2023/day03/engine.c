#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLS 200
#define MAX_ROWS 200
#define OFFSET(x, y) (((y) * MAX_COLS) + (x))

void read_input(char **out_data, int *out_rows) {
	char *data = malloc(MAX_COLS * MAX_ROWS);
	if(data == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		exit(EXIT_FAILURE);
	}
	memset(data, '.', MAX_COLS * MAX_ROWS);

	int rows = 0;
	while(1) {
		char buffer[256];
		if(fgets(buffer, sizeof(buffer), stdin) == NULL) break;

		size_t len = strlen(buffer);
		if(len == 0) continue;

		if(buffer[len-1] == '\n') {
			buffer[len-1] = '\0';
			len -= 1;
		}
		if(len == 0) continue;

		memcpy(data + OFFSET(0, rows), buffer, len);
		++rows;
	}

	*out_data = data;
	*out_rows = rows;
}

int adjacent_to_symbol(const int startX, const int endX, const int numY, const char *data, const int rows) {
	// fprintf(stderr, "Scanning at %d-%d:%d\n", startX, endX, numY);
	const int minY = (numY == 0) ? numY : (numY - 1);
	const int maxY = (numY+1 == rows) ? numY : (numY + 1);
	const int minX = (startX == 0) ? startX : (startX - 1);
	const int maxX = (endX+1 == MAX_COLS) ? endX : (endX + 1);

	for(int y = minY; y <= maxY; ++y) {
		for(int x = minX; x <= maxX; ++x) {
			char c = data[OFFSET(x, y)];
			if((c > '9') || ((c < '0') && (c != ' ') && (c != '.'))) {
				// fprintf(stderr, "Found part at %d-%d:%d (symbol: '%c')\n", startX, endX, numY, c);
				return 1;
			}
		}
	}
	return 0;
}

int main(void) {
	char *data;
	int rows;
	read_input(&data, &rows);

	unsigned int sum = 0;
	for(int y = 0; y < rows; ++y) {
		int startIndex = -1;
		int endIndex = -1;
		unsigned int value = 0;
		
		for(int x = 0; x < MAX_COLS; ++x) {
			char c = data[OFFSET(x, y)];
			if(startIndex < 0) {
				if((c >= '0') && (c <= '9')) {
					startIndex = endIndex = x;
					value = c - '0';
				}
			} else {
				if((c >= '0') && (c <= '9')) {
					value = (value * 10) + (c - '0');
					endIndex = x;
				} else {
					if(adjacent_to_symbol(startIndex, endIndex, y, data, rows)) {
						sum += value;
					}
					startIndex = -1;
				}
			}
		}

		// Don't forget about numbers at end of the line
		if((startIndex > 0) && (adjacent_to_symbol(startIndex, endIndex, y, data, rows))) {
			sum += value;
		}
	}
	printf("Part1: %u\n", sum);
}
