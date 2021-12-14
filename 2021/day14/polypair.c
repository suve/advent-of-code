#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RULES 128

#define LETTER_COUNT ('Z' - 'A' + 1)
#define PAIR_COUNT (LETTER_COUNT * LETTER_COUNT)

#define STEPS_PART_ONE 10
#define STEPS_PART_TWO 40

/*
 * Instead of storing the polymer as a long-ass string,
 * store only the count of letter pairs.
 *
 * Consider the polymer "ABCD":
 * - one "AB" pair
 * - one "BC" pair
 * - one "CD" pair
 *
 * There's one caveat, though: by doing this, we count "B" and "C" twice.
 * Since insertion rules work by adding an extra letter in the middle
 * of a pair, the first and last letter of the polymer never change.
 * So let's just store these as extra data.
 */
struct Polymer {
	unsigned long int count[PAIR_COUNT];
	int first, last;
};

struct Rule {
	int from;
	int to[2];
};

int rule_cmp(const void *va, const void *vb) {
	const struct Rule *ra = (struct Rule*)va;
	const struct Rule *rb = (struct Rule*)vb;

	if(ra->from < rb->from) return -1;
	if(ra->from > rb->from) return +1;

	return 0;
}

int letters_to_pair(const char first, const char second) {
	return ((first - 'A') * LETTER_COUNT) + (second - 'A');
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
	buflen -= 1; // Trim trailing newline

	memset(polymer, 0, sizeof(struct Polymer));
	for(size_t i = 0; i < (buflen - 1); ++i) {
		const int pair = letters_to_pair(buffer[i], buffer[i+1]);
		polymer->count[pair] += 1;
	}
	polymer->first = buffer[0] - 'A';
	polymer->last = buffer[buflen - 1] - 'A';

	// Skip empty line between polymer and rules
	fgets(buffer, sizeof(buffer), stdin);

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

		// Suppose AB -> C
		rules[*rule_count].from = letters_to_pair(buffer[0], buffer[1]);  // From AB
		rules[*rule_count].to[0] = letters_to_pair(buffer[0], buffer[6]); // To AC
		rules[*rule_count].to[1] = letters_to_pair(buffer[6], buffer[1]); // To CB
		*rule_count += 1;
		continue;
		
		invalid: {	
			fprintf(stderr, "Invalid insertion pair: \"%s\"\n", buffer);
			exit(EXIT_FAILURE);
		}
	}
}

struct Rule *match_rule(const int pair, const struct Rule *rules, const int rule_count) {
	struct Rule fakerule = (struct Rule){
		.from = pair,
		.to = {-1, -1}
	};
	
	return bsearch(&fakerule, rules, rule_count, sizeof(struct Rule), &rule_cmp);
}

struct Polymer *apply_rules(const struct Polymer *old, const struct Rule *rules, const int rule_count) {
	struct Polymer *new = malloc(sizeof(struct Polymer));
	memset(new, 0, sizeof(struct Polymer));

	for(int pair = 0; pair < PAIR_COUNT; ++pair) {
		if(old->count[pair] == 0) continue;

		struct Rule *rule = match_rule(pair, rules, rule_count);
		if(rule == NULL) {
			new->count[pair] += old->count[pair];
			continue;
		}
		
		new->count[rule->to[0]] += old->count[pair];
		new->count[rule->to[1]] += old->count[pair];
	}

	new->first = old->first;
	new->last = old->last;
	return new;
}

void count_letters(const struct Polymer *polymer, unsigned long int *count) {
	memset(count, 0, sizeof(unsigned long int) * LETTER_COUNT);
	for(int pair = 0; pair < PAIR_COUNT; ++pair) {
		if(polymer->count[pair] == 0) continue;
		
		const int first = pair / LETTER_COUNT;
		const int second = pair % LETTER_COUNT;
		count[first] += polymer->count[pair];
		count[second] += polymer->count[pair];
	}

	count[polymer->first] += 1;
	count[polymer->last] += 1;

	// Remember, each letter is stored twice
	for(int l = 0; l < LETTER_COUNT; ++l) {
		count[l] /= 2;
	}
}

void find_minmax(const unsigned long int *values, const int count, int *min_idx, int *max_idx) {
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

void print_answer(const char *header, const struct Polymer *polymer) {
	unsigned long int count[LETTER_COUNT];
	count_letters(polymer, count);
	
	int min, max;
	find_minmax(count, LETTER_COUNT, &min, &max);

	printf(
		"%s: %lu (<%lu x %c> - <%lu x %c>)\n",
		header,
		count[max] - count[min],
		count[max], 'A' + max,
		count[min], 'A' + min
	);
}

int main(void) {
	struct Polymer *polymer = malloc(sizeof(struct Polymer));
	struct Rule rules[MAX_RULES];
	int rule_count = 0;

	read_input(polymer, rules, &rule_count);
	qsort(rules, rule_count, sizeof(struct Rule), &rule_cmp);

	for(int i = 0; i < STEPS_PART_ONE; ++i) {
		struct Polymer *new = apply_rules(polymer, rules, rule_count);
		free(polymer);
		polymer = new;
	}
	print_answer("Part1", polymer);	
	
	for(int i = STEPS_PART_ONE; i < STEPS_PART_TWO; ++i) {
		struct Polymer *new = apply_rules(polymer, rules, rule_count);
		free(polymer);
		polymer = new;
	}
	print_answer("Part2", polymer);	

	free(polymer);
	return 0;
}
