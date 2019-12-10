#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Opt_verbose = 0;

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

		if((Opt_verbose) && (vis)) {
			printf(
				"Asteroid #%d (%d:%d) has line of sight to asteroid #%d (%d:%d)\n",
				fromIndex, asteroid[fromIndex].x, asteroid[fromIndex].y,
				i, asteroid[i].x, asteroid[i].y
			);
		}
	}
	return count;
}

static int findAndPrintBest(void) {
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
	return bestIndex;
}

static void vaporize(const int idx) {
	asteroidCount -= 1;
	if(asteroidCount > idx) {
		memmove(&asteroid[idx], &asteroid[idx+1], sizeof(struct Point) * (asteroidCount - idx));
	}
}

static void calculateDistanceAndAngle(const int fromIndex, const int toIndex, unsigned int *const distance, unsigned int *const angle) {
	const int xDiff = asteroid[toIndex].x - asteroid[fromIndex].x;
	const int yDiff = asteroid[toIndex].y - asteroid[fromIndex].y;
	
	const double dist = hypot(xDiff, yDiff);
	const double sin = yDiff / dist;
	const double cos = xDiff / dist;

	// The arguments here quite funky, aren't they?
	// This flips the logic a bit so that angle 0 is "up", not "right"
	double rads = atan2(cos, -sin);
	if(rads < 0) rads += (2.0 * M_PI);

	*distance = dist * (UINT_MAX / 1000.0);
	*angle = trunc(rads * (UINT_MAX / 2) / (2.0 * M_PI));
}

struct Target {
	unsigned int angle, distance;
	int index;
};

int target_compare(const void *a, const void *b) {
	const struct Target *at = a;
	const struct Target *bt = b;

	if(at->angle > bt->angle) return +1;
	if(at->angle < bt->angle) return -1;

	if(at->distance > bt->distance) return +1;
	if(at->distance < bt->distance) return -1;

	return 0;
}

#define BETS_ON 200

static void fireTehLazor(int fromIndex) {
	struct Target target[asteroidCount-1];
	for(int i = 0, t = 0; i < asteroidCount; ++i) {
		if(i == fromIndex) continue;

		unsigned int d, a;
		calculateDistanceAndAngle(fromIndex, i, &d, &a);
		
		target[t].angle = a;
		target[t].distance = d;
		target[t].index = i;
		++t;
	}

	qsort(target, asteroidCount-1, sizeof(struct Target), &target_compare);

	int betIndex = -1;
	unsigned int lastAngle = UINT_MAX;
	for(int t = 0, blasted = 0; blasted < (asteroidCount-1); t = (t == asteroidCount-2) ? 0 : t+1, lastAngle = (t == 0) ? UINT_MAX : lastAngle) {
		int idx = target[t].index;
		if(idx < 0) continue;

		if(target[t].angle == lastAngle) continue;

		++blasted;
		if(blasted == BETS_ON) {
			betIndex = idx;
		}

		if(Opt_verbose) {
			printf(
				"pew pew! Asteroid no.%d to be vaporized was #%d (%d:%d)\n",
				blasted, idx, asteroid[idx].x, asteroid[idx].y
			);
		}

		target[t].index = -1;
		lastAngle = target[t].angle;
	}
	
	if(asteroidCount-1 >= BETS_ON) {
		const int bx = asteroid[betIndex].x;
		const int by = asteroid[betIndex].y;
		printf(
			"Asteroid %d to be blasted was #%d (%d:%d, checksum: %d)\n",
			BETS_ON, betIndex, bx, by, ((bx * 100) + by)
		);
	}
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

int main(int argc, char **argv) {
	for(int a = 1; a < argc; ++a) {
		if(strcmp(argv[a], "--verbose") == 0) Opt_verbose = 1;
	}

	readInput();
	fireTehLazor(findAndPrintBest());
}
