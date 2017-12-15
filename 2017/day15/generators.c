#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define GEN1_FACTOR 16807ull
#define GEN2_FACTOR 48271ull
#define MODULUS 2147483647ull

#define GEN1_MOD 4ull
#define GEN2_MOD 8ull

int main(int argc, char** argv) {
	int mode = 0;
	int verbose = 0;
	
	for(int a = 1; a < argc; ++a) {
		if(strcmp(argv[a], "--part1") == 0)
			mode = 1;
		else if(strcmp(argv[a], "--part2") == 0)
			mode = 2;
		else if(strcmp(argv[a], "--verbose") == 0)
			verbose = 1;
		else printf("Unknown option \"%s\"\n", argv[a]), exit(EXIT_FAILURE);
	}
	if(mode == 0) {
		printf("You must provide a mode of operation: \"--part1\" or \"--part2\"\n");
		exit(EXIT_FAILURE);
	}
	
	
	uint64_t gen1, gen2, rounds;
	scanf("%lu %lu %lu", &gen1, &gen2, &rounds);
	
	uint64_t score = 0;
	while(rounds --> 0) {
		if(mode == 1) {
			gen1 = (gen1 * GEN1_FACTOR) % MODULUS;
			gen2 = (gen2 * GEN2_FACTOR) % MODULUS;
		} else {
			do { gen1 = (gen1 * GEN1_FACTOR) % MODULUS; } while(gen1 % GEN1_MOD);
			do { gen2 = (gen2 * GEN2_FACTOR) % MODULUS; } while(gen2 % GEN2_MOD);
		}
		
		uint16_t low1 = gen1;
		uint16_t low2 = gen2;
		if(low1 == low2) ++score;
		
		if(verbose) printf("> %20lu:%20lu -> %2x:%2x\n", gen1, gen2, low1, low2);
	}
	printf("score: %lu\n", score);
}
