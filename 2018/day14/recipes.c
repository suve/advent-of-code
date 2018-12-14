#define _GNU_SOURCE
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECIPES 256000000L

long int rawprint(char *buffer, long int value) {
	long int copy = value;
	int digits = 0;
	while(copy > 0) copy /= 10, ++digits;
	
	if(digits == 0) digits = 1;
	for(int d = digits-1; d >= 0; --d) {
		buffer[d] = value % 10;
		value /= 10;
	}
	
	return digits;
}

int main(void) {
	char *recipe = malloc(MAX_RECIPES);
	
	long int input = 0;
	while(scanf("%ld\n", &input) == 1) {
		memset(recipe, 0, MAX_RECIPES);
		
		recipe[0] = 3;
		recipe[1] = 7;
		long int recipe_num = 2;
		
		int pos1 = 0;
		int pos2 = 1;
		
		char input_str[16];
		long int ilen = rawprint(input_str, input);
		int located = 0;
		
		while((recipe_num < input+10) || (!located)) {
			int sum = recipe[pos1] + recipe[pos2];
			if(sum >= 10) {
				recipe[recipe_num++] = sum / 10;
				recipe[recipe_num++] = sum % 10;
			} else {
				recipe[recipe_num++] = sum;
			}
			
			pos1 = (pos1 + (recipe[pos1] + 1)) % recipe_num;
			pos2 = (pos2 + (recipe[pos2] + 1)) % recipe_num;
			
			// -- part 2
			if(located) continue;
			
			for(int i = 0; i < 2; ++i) {
				long int offset = recipe_num - ilen - i;
				if(offset < 0) continue;
				
				if(memcmp(recipe + offset, input_str, ilen) == 0) { 
					printf("--> part2: \"%ld\" located at position %ld\n", input, offset);
					located = 1;
					break;
				} 
			}
		}
		
		printf(
			"--> part1: %d%d%d%d%d%d%d%d%d%d\n",
			(int)recipe[input+0], (int)recipe[input+1],
			(int)recipe[input+2], (int)recipe[input+3],
			(int)recipe[input+4], (int)recipe[input+5],
			(int)recipe[input+6], (int)recipe[input+7],
			(int)recipe[input+8], (int)recipe[input+9]
		);
	}
	
	free(recipe);
	return 0;
}
