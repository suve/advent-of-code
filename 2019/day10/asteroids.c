#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Point {
	int x, y;
};

#define ASTEROIDS_MAX 4096
struct Point asteroid[ASTEROIDS_MAX];
int asteroidCount = 0;


static int sgn(const int x) {
	if(x > 0) return +1;
	if(x < 0) return -1;
	return 0;
}

static int gcd(int a, int b) {
	while(1) {
		if(a == b) return a;

		if(a > b)
			a = a - b;
		else
			b = b - a;
	}
}


static int checkLineOfSight(const int fromIndex, const int toIndex) {
	const int xDiff = asteroid[toIndex].x - asteroid[fromIndex].x;
	const int yDiff = asteroid[toIndex].y - asteroid[fromIndex].y;
	
	const int xDiff_abs = abs(xDiff);
	const int yDiff_abs = abs(yDiff);

	for(int i = 0; i < asteroidCount; ++i) {
		if((i == fromIndex) || (i == toIndex)) continue;

		const int xd = asteroid[i].x - asteroid[fromIndex].x;
		if(sgn(xd) != sgn(xDiff)) continue;

		const int xda = abs(xd);
		if(xda > xDiff_abs) continue;

		const int yd = asteroid[i].y - asteroid[fromIndex].y;
		if(sgn(yd) != sgn(yDiff)) continue;

		const int yda = abs(yd);
		if(yda > yDiff_abs) continue;

		// If it's a horizontal or a vertical line, just assume the answer is "yes"
		// as in "yes, asteroid #i blocks line of sight to asteroid #toIndex"
		if((xda == 0) || (yda == 0)) return 0;

		const int xfactor = xda * yDiff_abs;
		const int yfactor = yda * xDiff_abs;
		if(xfactor != yfactor) continue;

		return 0;
	}

	return 1;
}

static int countVisible(const int fromIndex) {
	int count = 0;
	for(int i = 0; i < asteroidCount; ++i) {
		if(i == fromIndex) continue;

		int vis = checkLineOfSight(fromIndex, i);
		count += vis;

		continue;
		if(vis) {
			printf(
				"Asteroid #%d (%d:%d) has line of sight to asteroid #%d (%d:%d)\n",
				fromIndex, asteroid[fromIndex].x, asteroid[fromIndex].y,
				i, asteroid[i].x, asteroid[i].y
			);
		}
	}
	return count;
}

static void findAndPrintBest(void) {
	int bestIndex = 0;
	int bestCount = countVisible(0);
	for(int i = 1; i < asteroidCount; ++i) {
		int count = countVisible(i);
		if(count > bestCount) {
			bestIndex = i;
			bestCount = count;
		}
	}

	printf(
		"Best asteroid is #%d (%d:%d) with %d other asteroids visible\n",
		bestIndex, asteroid[bestIndex].x, asteroid[bestIndex].y, bestCount
	);
}

static void readInput(void) {
	char buffer[1024];
	for(int y = 0; fgets(buffer, sizeof(buffer), stdin) != NULL; ++y) {
		size_t len = strlen(buffer);
		for(int x = 0; x < len; ++x) {
			if(buffer[x] != '#') continue;

			asteroid[asteroidCount].x = x;
			asteroid[asteroidCount].y = y;
			++asteroidCount;
		}
	}
}

int main(void) {
	readInput();
	findAndPrintBest();
}
