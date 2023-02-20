#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint;

#define MAX_NUMBERS (5*1024)

struct Number {
	int value;
	uint seq;
};

void read_input(struct Number *num, uint *seq, uint *count) {
	uint c = 0;

	int value;
	while(scanf("%d\n", &value) == 1) {
		if(c == MAX_NUMBERS) {
			fprintf(stderr, "Error: maximum number of input values reached (%d)\n", MAX_NUMBERS);
			exit(EXIT_FAILURE);
		}

		num[c].value = value;
		num[c].seq = seq[c] = c;
		++c;
	}
	*count = c;
}

void swap(const int first, const uint second, struct Number *num, uint *seq) {
	const struct Number temp_num = num[first];
	num[first] = num[second];
	num[second] = temp_num;

	seq[num[first].seq] = first;
	seq[num[second].seq] = second;
}

void mix(struct Number *num, uint *seq, const uint count) {
	for(uint s = 0; s < count; ++s) {
		uint this = seq[s];

		int move = num[this].value;
		if(move == 0) continue;

		int dir = 1;
		if(move < 0) {
			dir = -1;
			move = -move;
		}
		do {
			uint other;
			if(dir > 0) {
				other = (this + 1) % count;
			} else {
				other = (this > 0) ? (this - 1) : (count - 1);
			}
			swap(this, other, num, seq);

			this = other;
			--move;
		} while(move > 0);
	}
}

uint find_zero(const struct Number *num, const uint count) {
	for(uint idx = 0; idx < count; ++idx) {
		if(num[idx].value == 0) return idx;
	}
	fprintf(stderr, "Error: unable to find zero value\n");
	exit(EXIT_FAILURE);
}

int main(void) {
	struct Number num[MAX_NUMBERS];
	uint seq[MAX_NUMBERS];
	uint count;
	read_input(num, seq, &count);

	mix(num, seq, count);

	{
		const uint zero = find_zero(num, count);
		const int first = num[(zero + 1000) % count].value;
		const int second = num[(zero + 2000) % count].value;
		const int third = num[(zero + 3000) % count].value;
		printf("Part1: (%d) + (%d) + (%d) = %d\n", first, second, third, first + second + third);
	}
}
