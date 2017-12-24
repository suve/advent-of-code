#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Part {
	int left, right;
};

#define PARTS_MAX 100
struct Part part[PARTS_MAX];
int part_count;

int mode = 0;


void construction_step(
	const int size_now, int *const best_size, 
	const int length_now, int *const best_length, 
	uint8_t *const used, const int looking_for
) {
	int matched = 0;
	for(int p = 0; p < part_count; ++p) {
		if(used[p]) continue;
		
		if(part[p].left == looking_for) {
			used[p] = 1;
			construction_step(
				size_now + part[p].left + part[p].right, best_size,
				length_now + 1, best_length, 
				used, part[p].right
			);
			used[p] = 0;
			
			matched = 1;
		} else if(part[p].right == looking_for) {
			used[p] = 1;
			construction_step(
				size_now + part[p].left + part[p].right, best_size, 
				length_now + 1, best_length,
				used, part[p].left
			);
			used[p] = 0;
			
			matched = 1;
		} 
	}
	
	if(!matched) {
		printf("> dead-end: %d\n", size_now);
		
		if(mode == 1) {
			if(size_now > *best_size) *best_size = size_now;
		} else {
			if(length_now >= *best_length) {
				*best_length = length_now;
				if(size_now > *best_size) *best_size = size_now;
			}
		}
	}
}

int construct_bridge(void) {
	uint8_t used[part_count];
	memset(used, 0, sizeof(used));
	
	int size = 0, length = 0;
	construction_step(0, &size, 0, &length, used, 0);
	return size;
}

void parse_args(int argc, char **argv) {
	if(argc < 2) {
		fprintf(stderr, "You must specify mode of operation: either \"--part1\" or \"--part2\"\n");
		exit(EXIT_FAILURE);
	}
	
	if(strcmp(argv[1], "--part1") == 0) {
		mode = 1;
	} else if(strcmp(argv[1], "--part2") == 0) {
		mode = 2;
	} else {
		fprintf(stderr, "Unrecognized argument: \"%s\" (%lu)\n", argv[1], strlen(argv[1]));
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv) {
	parse_args(argc, argv);
	
	part_count = 0;
	while(scanf("%d/%d ", &(part[part_count].left), &(part[part_count].right)) == 2) ++part_count;
	
	printf("> elements: %d\n", part_count);
	printf("bridge: %d\n", construct_bridge());
	
	return 0;
}
