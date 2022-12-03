#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ITEM_TYPES 52

struct Rucksack {
	int left[ITEM_TYPES];
	int right[ITEM_TYPES];
	int duplicate;
};

void parse_line(char *buffer, struct Rucksack *sack) {
	memset(sack, 0, sizeof(struct Rucksack));
	sack->duplicate = -1;

	size_t len = strlen(buffer);
	if(buffer[len-1] == '\n') {
		buffer[--len] = '\0';
	}
	if(len % 2) {
		fprintf(stderr, "Invalid input: line length is not divisible by 2\n");
		exit(EXIT_FAILURE);
	}

	int *dest = sack->left;
	for(int i = 0; i < len; ++i) {
		if(i == (len / 2)) {
			dest = sack->right;
		}

		char c = buffer[i];
		if(c == '\0') {
			break;
		} else if((c >= 'a') && (c <= 'z')) {
			dest[c - 'a'] += 1;
		} else if((c >= 'A') && (c <= 'Z')) {
			dest[c - 'A' + (ITEM_TYPES / 2)] += 1;
		} else {
			fprintf(stderr, "Invalid input: unexpected character \"%c\" (0x%x)\n", c, (int)((unsigned char)c));
			exit(EXIT_FAILURE);
		}
	}

	for(int i = 0; i < ITEM_TYPES; ++i) {
		if((sack->left[i] > 0) && (sack->right[i] > 0)) {
			sack->duplicate = i;
			break;
		}
	}
}

#define RUCKSACKS_MAX 512

int main(void) {
	unsigned int count = 0;
	struct Rucksack rucksack[RUCKSACKS_MAX];

	char buffer[1024];
	while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
		parse_line(buffer, rucksack + count);
		++count;
	}

	unsigned int part1 = 0;
	for(int i = 0; i < count; ++i) {
		if(rucksack[i].duplicate >= 0) {
			part1 += rucksack[i].duplicate + 1;
		}
	}
	printf("Part 1: %u\n", part1);

	unsigned int part2 = 0;
	for(int group = 0; group < (count / 3); ++group) {
		int present[ITEM_TYPES];
		memset(present, 0, sizeof(present));

		for(int g = 0; g < 3; ++g) {
			struct Rucksack *sack = &rucksack[(group * 3) + g];
			for(int i = 0; i < ITEM_TYPES; ++i) {
				if((sack->left[i] > 0) || (sack->right[i] > 0)) {
					present[i] += 1;
				}
			}
		}

		for(int i = 0; i < ITEM_TYPES; ++i) {
			if(present[i] == 3) {
				part2 += (i + 1);
				break;
			}
		}
	}
	printf("Part 2: %u\n", part2);

	return 0;
}
