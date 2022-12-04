#include <stdio.h>

int contains(unsigned int outer_start, unsigned int outer_end, unsigned int inner_start, unsigned int inner_end) {
	return (inner_start >= outer_start) && (inner_end <= outer_end);
}

int overlaps(unsigned int a_start, unsigned int a_end, unsigned int b_start, unsigned int b_end) {
	return ((a_start >= b_start) && (a_start <= b_end)) || ((b_start >= a_start) && (b_start <= a_end));
}

int main(void) {
	unsigned int p1 = 0;
	unsigned int p2 = 0;

	while(1) {
		unsigned a_start, a_end, b_start, b_end;
		if(scanf("%u-%u,%u-%u", &a_start, &a_end, &b_start, &b_end) != 4) break;

		if(contains(a_start, a_end, b_start, b_end) || (contains(b_start, b_end, a_start, a_end))) {
			++p1;
			++p2;
		} else if(overlaps(a_start, a_end, b_start, b_end)) {
			++p2;
		}
	}

	printf("Part 1: %u\nPart 2: %u\n", p1, p2);
	return 0;
}
