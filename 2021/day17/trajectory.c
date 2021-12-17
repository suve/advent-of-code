#include <math.h>
#include <stdio.h>

void calc_x_velocity(int tx1, int tx2, int *min, int *max) {
	int i = 0, v = 0;
	do {
		i += 1;	
		v += i;
	} while(v < tx1);
	
	*min = *max = i;

	while(1) {
		i += 1;	
		v += i;
		if(v > tx2) return;
		*max = i;
	}
}

int main(void) {
	int tx1, tx2, ty1, ty2;
	while(scanf("target area: x=%d..%d, y=%d..%d\n", &tx1, &tx2, &ty1, &ty2) == 4) {
		int min_x, max_x;
		calc_x_velocity(tx1, tx2, &min_x, &max_x);

		max_x = tx2; // lol, need this for part2

		int ok_count = 0;
		int best_x = 0, best_y = 0, best_height = 0;
		for(int x = min_x; x <= max_x; ++x) {
			for(int y = ty1; y < 500; ++y) { // max value pulled out of my ass
				int pos_x = 0;
				int pos_y = 0;
				int vel_x = x;
				int vel_y = y;
				int height = 0;

				int ok;
				while(1) {
					pos_x += vel_x;
					pos_y += vel_y;

					if(vel_x > 0) vel_x -= 1;
					vel_y -= 1;

					if(pos_y > height) height = pos_y;

					if(pos_x > tx2) {
						ok = 0;
						break;
					}
					if(pos_y < ty1) {
						ok = 0;
						break;
					}

					if((pos_x >= tx1) && (pos_y >= ty1) && (pos_y <= ty2)) {
						ok = 1;
						break;
					}
				}

				if(ok) {
					ok_count += 1;
					if(height > best_height) {
						best_x = x;
						best_y = y;
						best_height = height;
					}
				}
			}
		}
		printf("Best velocity: %d,%d (height of %d)\n", best_x, best_y, best_height);
		printf("Good velocities: %d\n", ok_count);
		puts("");
	}

	return 0;
}
