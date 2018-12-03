#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FABRIC_SIZE 1000L
#define FABRIC_MEMSIZE (FABRIC_SIZE * FABRIC_SIZE * sizeof(int))

#define OFFSET(x, y) ((x) + ((y)*FABRIC_SIZE))

int main(void) {
	int *fabric = malloc(FABRIC_MEMSIZE);
	memset(fabric, 0, FABRIC_MEMSIZE);

	int overlaps[2000]; // should be enough
	memset(overlaps, 0, sizeof(overlaps));

	char line[256];
	while(fgets(line, sizeof(line), stdin)) {
		int cid, cx, cy, cw, ch;
		sscanf(line, "#%d @ %d,%d: %dx%d\n", &cid, &cx, &cy, &cw, &ch);
	
		overlaps[cid] = 1;
		for(int y = cy; y < cy+ch; ++y) {
			for(int x = cx; x < cx+cw; ++x) {
				long int offset = OFFSET(x, y);
				if(fabric[offset] > 0) {
					overlaps[fabric[offset]] = 2;
					overlaps[cid] = 2;
					fabric[offset] = -1;
				} else if(fabric[offset] == 0)  {
					fabric[offset] = cid;
				}
			}
		}
	}

	long int dupl = 0;
	long int offset = 0;
	for(int y = 0; y < FABRIC_SIZE; ++y) {
		for(int x = 0; x < FABRIC_SIZE; ++x) {
			if(fabric[offset++] == -1) ++dupl;	
		}
	}
	printf("-> part1: %ld\n", dupl);

	for(int i = 0; i < 2000; ++i) {
		if(overlaps[i] == 1) printf("--> part2: %d\n", i);
	}

	return 0;
}
