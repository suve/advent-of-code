#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RULES 128
#define MAX_SIZE 65536

#define STEPS 10

struct Polymer {
	size_t length;
	char data[MAX_SIZE];
};

struct Rule {
	char from[2];
	char to;
};

int rule_cmp(const void *va, const void *vb) {
	const struct Rule *ra = (struct Rule*)va;
	const struct Rule *rb = (struct Rule*)vb;

	if(ra->from[0] < rb->from[0]) return -1;
	if(ra->from[0] > rb->from[0]) return +1;

	if(ra->from[1] < rb->from[1]) return -1;
	if(ra->from[1] > rb->from[1]) return +1;

	return 0;
}

void read_input(struct Polymer *polymer, struct Rule *rules, int *rule_count) {
	char buffer[256];
	if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
		fprintf(stderr, "Failed to read starting polymer\n");
		exit(EXIT_FAILURE);
	}

	size_t buflen = strlen(buffer);
	if(buflen < 3) {
		fprintf(stderr, "Initial polymer is too short: expected at least 2 characters\n");
		exit(EXIT_FAILURE);
	}
	polymer->length = buflen - 1; // Trim trailing newline
	memcpy(polymer->data, buffer, polymer->length);
	polymer->data[polymer->length] = '\0'; // In case we want to print it

	fgets(buffer, sizeof(buffer), stdin); // Skip line

	*rule_count = 0;
	while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
		buflen = strlen(buffer);
		buffer[buflen-1] = '\0'; // Trim trailing newline		

		if(strlen(buffer) != 7) goto invalid;
		if((buffer[0] < 'A') || (buffer[0] > 'Z')) goto invalid;
		if((buffer[1] < 'A') || (buffer[1] > 'Z')) goto invalid;
		if(buffer[2] != ' ') goto invalid;
		if(buffer[3] != '-') goto invalid;
		if(buffer[4] != '>') goto invalid;
		if(buffer[5] != ' ') goto invalid;
		if((buffer[6] < 'A') || (buffer[6] > 'Z')) goto invalid;

		rules[*rule_count].from[0] = buffer[0];
		rules[*rule_count].from[1] = buffer[1];
		rules[*rule_count].to = buffer[6];
		*rule_count += 1;
		continue;
		
		invalid: {	
			fprintf(stderr, "Invalid insertion pair: \"%s\"\n", buffer);
			exit(EXIT_FAILURE);
		}
	}
}

struct Rule *match_rule(const char first, const char second, const struct Rule *rules, const int rule_count) {
	struct Rule fakerule = (struct Rule){
		.from = {first, second},
		.to = '\0'
	};
	
	return bsearch(&fakerule, rules, rule_count, sizeof(struct Rule), &rule_cmp);
}

struct Polymer *apply_rules(const struct Polymer *old, const struct Rule *rules, const int rule_count) {
	struct Polymer *new = malloc(sizeof(struct Polymer));
	new->length = 0;

	for(size_t i = 0; i < (old->length - 1); ++i) {
		new->data[new->length] = old->data[i];
		new->length += 1;
		
		struct Rule *change = match_rule(old->data[i], old->data[i+1], rules, rule_count);
		if(change != NULL) {
			new->data[new->length] = change->to;
			new->length += 1;
		}
	}

	// Don't forget to copy the last char
	new->data[new->length] = old->data[old->length - 1];
	new->length += 1;
	// Add a NUL terminator in case we want to print this
	new->data[new->length] = '\0';

	return new;
}

#define LETTER_COUNT ('Z' - 'A' + 1)
void count_letters(struct Polymer *polymer, int *count) {
	memset(count, 0, sizeof(int) * LETTER_COUNT);
	for(size_t i = 0; i < polymer->length; ++i) {
		const int offset = polymer->data[i] - 'A';
		count[offset] += 1;
	}
}

void find_minmax(const int *values, const int count, int *min_idx, int *max_idx) {
	int index = 0;
	while(values[index] == 0) ++index;

	int min = index;
	int max = index;
	for(int i = index + 1; i < count; ++i) {
		if((values[i] > 0) && (values[i] < values[min])) min = i;
		if(values[i] > values[max]) max = i;
	}

	*min_idx = min;
	*max_idx = max;
}

int main(void) {
	struct Polymer *polymer = malloc(sizeof(struct Polymer));
	struct Rule rules[MAX_RULES];
	int rule_count = 0;

	read_input(polymer, rules, &rule_count);
	qsort(rules, rule_count, sizeof(struct Rule), &rule_cmp);

	for(int i = 0; i < STEPS; ++i) {
		struct Polymer *new = apply_rules(polymer, rules, rule_count);
		free(polymer);
		polymer = new;
	}
	
	int count[LETTER_COUNT];
	count_letters(polymer, count);
	
	int min, max;
	find_minmax(count, LETTER_COUNT, &min, &max);

	printf(
		"Part1: %d (<%d x %c> - <%d x %c>)\n",
		count[max] - count[min],
		count[max], 'A' + max,
		count[min], 'A' + min
	);
}
