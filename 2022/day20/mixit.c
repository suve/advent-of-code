#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;

#define MAX_NUMBERS (5*1024)

#define PART2_MIX_COUNT 10
#define PART2_DECRYPTION_KEY 811589153L

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

void mix(struct Number *num, uint *seq, const uint count, const long int multiplier) {
	for(uint s = 0; s < count; ++s) {
		uint this = seq[s];

		long int move = num[this].value;
		if(move == 0) continue;

		int dir = 1;
		if(move < 0) {
			dir = -1;
			move = -move;
		}
		if(multiplier > 1) move *= multiplier;

		move %= (count - 1);
		if(move == 0) continue;

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

void copy_data(const struct Number *from_num, const uint *from_seq, const uint count, struct Number **to_num, uint **to_seq) {
	const size_t size_num = count * sizeof(struct Number);
	*to_num = malloc(size_num);
	memcpy(*to_num, from_num, size_num);

	const size_t size_seq = count * sizeof(uint);
	*to_seq = malloc(size_seq);
	memcpy(*to_seq, from_seq, size_seq);
}

void part1(const struct Number *orig_num, const uint *orig_seq, const uint count) {
	struct Number *num;
	uint *seq;
	copy_data(orig_num, orig_seq, count, &num, &seq);

	mix(num, seq, count, 1);

	const uint zero = find_zero(num, count);
	const int first = num[(zero + 1000) % count].value;
	const int second = num[(zero + 2000) % count].value;
	const int third = num[(zero + 3000) % count].value;
	printf("Part1: (%d) + (%d) + (%d) = %d\n", first, second, third, first + second + third);

	free(num);
	free(seq);
}

void part2(struct Number *num, uint *seq, const uint count) {
	// Could make a copy of the data like in part1, but we're not gonna need it afterwards,
	// so let's skip all the malloc() and memcpy() calls and just operate on the original data.

	for(int i = 0; i < PART2_MIX_COUNT; ++i) {
		mix(num, seq, count, PART2_DECRYPTION_KEY);
	}

	const uint zero = find_zero(num, count);
	const long int first = num[(zero + 1000) % count].value * PART2_DECRYPTION_KEY;
	const long int second = num[(zero + 2000) % count].value * PART2_DECRYPTION_KEY;
	const long int third = num[(zero + 3000) % count].value * PART2_DECRYPTION_KEY;
	printf("Part2: (%ld) + (%ld) + (%ld) = %ld\n", first, second, third, first + second + third);
}

int main(void) {
	struct Number num[MAX_NUMBERS];
	uint seq[MAX_NUMBERS];
	uint count;
	read_input(num, seq, &count);

	part1(num, seq, count);
	part2(num, seq, count);
	return 0;
}
