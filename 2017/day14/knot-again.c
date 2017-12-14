#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ROUNDS 64
#define ROWS 128

#define SIZE 256
int elem[SIZE];

#define EXTRA_LENGHTS 5
const int extra[EXTRA_LENGHTS] = { 17, 31, 73, 47, 23 };

#define HASH_LEN 16
uint8_t hash[HASH_LEN];


void reset_array(void) {
	for(int i = 0; i < SIZE; ++i) elem[i] = i;
}

void reverse(const int startPos, const int length) {
	const int endPos = (startPos + length - 1) % SIZE;
	
	for(int i = 0; i < length/2; ++i) {
		int a_p = (startPos+i) % SIZE;
		
		int b_p = (endPos-i);
		if(b_p < 0) b_p += SIZE;
		
		int a_v = elem[a_p];
		int b_v = elem[b_p];
		
		elem[a_p] = b_v;
		elem[b_p] = a_v;
	}
}

void calc_hash(char *const buffer, const size_t buflen) {
	reset_array();
	
	int pos = 0;
	int skip = 0;
	for(int round = 0; round < ROUNDS; ++round) {
		for(int b = 0; b < buflen; ++b) {
			int length = buffer[b];
			
			// visualise(pos, length);
			reverse(pos, length);
			
			pos = (pos + length + skip) % SIZE;
			skip += 1;
		}
		for(int e = 0; e < EXTRA_LENGHTS; ++e) {
			int length = extra[e];
			
			// visualise(pos, length);
			reverse(pos, length);
			
			pos = (pos + length + skip) % SIZE;
			skip += 1;
		}
	}
	
	for(int group = 0; group < 16; ++group) {
		uint8_t grphash = 0;
		for(int member = 0; member < 16; ++member) {
			grphash ^= elem[group*16 + member];
		}
		hash[group] = grphash;
	}
}

void print_hash(void) {
	for(int group = 0; group < 16; ++group) {
		printf("%02x", hash[group]);
	}
	putc('\n', stdout);
}

int count_bits(void) {
	int count = 0;
	for(int h = 0; h < HASH_LEN; ++h) {
		for(int b = 0; b < 8; ++b) {
			if(hash[h] & (1 << b)) ++count;
		}
	}
	
	return count;
}

int main(void) {
	char input[128];
	if(fgets(input, sizeof(input), stdin) == NULL) {
		input[0] = '\0';
	} else {
		int in_last = strlen(input)-1;
		if(input[in_last] == '\n') input[in_last] = '\0';
	}
	
	printf("input: \"%s\" (%lu)\n", input, strlen(input));
	
	int total_bits = 0;
	for(int row = 0; row < 128; ++row) {
		char buffer[128];
		int buflen = snprintf(buffer, sizeof(buffer), "%s-%d", input, row);
		
		// printf("buffer: \"%s\"\n", buffer);
		
		calc_hash(buffer, buflen);
		
		// printf("> ");
		// print_hash();
		
		int bits_now = count_bits();
		printf("> row %3d: %d bits\n", row, bits_now);
		
		total_bits += bits_now;
	}
	printf("total bits: %d\n", total_bits);
	
	return 0;
}
