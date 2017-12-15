#include <stdio.h>
#include <stdint.h>

#define GEN1_FACTOR 16807ull
#define GEN2_FACTOR 48271ull
#define MODULUS 2147483647ull

int main(int argc, char** argv) {
	uint64_t gen1, gen2, rounds;
	scanf("%lu %lu %lu", &gen1, &gen2, &rounds);
	
	uint64_t score = 0;
	while(rounds --> 0) {
		gen1 = (gen1 * GEN1_FACTOR) % MODULUS;
		gen2 = (gen2 * GEN2_FACTOR) % MODULUS;
		
		uint16_t low1 = gen1;
		uint16_t low2 = gen2;
		if(low1 == low2) ++score;
		
		// printf("> %20lu:%20lu -> %2x:%2x\n", gen1, gen2, low1, low2);
	}
	printf("score: %lu\n", score);
}
