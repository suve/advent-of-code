#include <stdio.h>
#include <stdlib.h>

#define NANOBOTS_MAX 1024

struct Nanobot {
	long int x, y, z;
	long int radius;
};

struct Nanobot nanobot[NANOBOTS_MAX];
int botCount = 0;


int main(void) {
	char line[256];
	while(fgets(line, sizeof(line), stdin) != NULL) {
		long int x, y, z, r;
		sscanf(line, "pos=<%ld,%ld,%ld>, r=%ld", &x, &y, &z, &r);

		nanobot[botCount] = (struct Nanobot){
			.x = x,
			.y = y,
			.z = z,
			.radius = r
		};
		botCount += 1;
	}

	int strongest = 0;
	for(int i = 1; i < botCount; ++i) {
		if(nanobot[i].radius > nanobot[strongest].radius) strongest = i;
	}

	int inRange = 0;
	for(int i = 0; i < botCount; ++i) {
		long int xDist = labs(nanobot[strongest].x - nanobot[i].x);
		long int yDist = labs(nanobot[strongest].y - nanobot[i].y);
		long int zDist = labs(nanobot[strongest].z - nanobot[i].z);

		if(xDist + yDist + zDist <= nanobot[strongest].radius) ++inRange;
	}

	printf(
		"part1: nanobot #%d has %d other bots in range (%ld units)\n",
		strongest, inRange, nanobot[strongest].radius
	);
	return 0;
}
