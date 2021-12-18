#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Value {
	int is_pair;
	struct Value *parent;
	union {
		struct {
			struct Value *left, *right;
		};
		int value;
	};
};

void dump(struct Value *v) {
	if(v->is_pair) {
		putc('[', stdout);
		dump(v->left);
		putc(',', stdout);
		dump(v->right);
		putc(']', stdout);
	} else {
		printf("%d", v->value);
	}

	if(v->parent == NULL) putc('\n', stdout);
}

struct Value *parse_r(char **str) {
	struct Value *result = malloc(sizeof(struct Value));
	memset(result, 0, sizeof(struct Value));
	
	if(**str == '[') {
		*str += 1;
		struct Value *left = parse_r(str);
		*str += 1;
		struct Value *right = parse_r(str);
		*str += 1;

		left->parent = result;
		right->parent = result;

		result->is_pair = 1;
		result->left = left;
		result->right = right;
	} else {
		int value = 0;
		while((**str >= '0') && (**str <= '9')) {
			value = value * 10 + (**str - '0');
			*str += 1;
		}
		result->is_pair = 0;
		result->value = value;
	}
	return result;
}

struct Value *parse(char *str) {
	return parse_r(&str);
}

void free_value(struct Value *v) {
	if(v->is_pair) {
		free_value(v->left);
		free_value(v->right);
	}
	free(v);
}

void merge_left(struct Value *v, int value) {
	if(v->parent == NULL) return;
	if(v->parent->left == v) {
		merge_left(v->parent, value);
		return;
	}

	v = v->parent->left;
	while(v->is_pair) v = v->right;
	v->value += value;
}

void merge_right(struct Value *v, int value) {
	if(v->parent == NULL) return;
	if(v->parent->right == v) {
		merge_right(v->parent, value);
		return;
	}

	v = v->parent->right;
	while(v->is_pair) v = v->left;
	v->value += value;
}

int explode(struct Value *v, int depth) {
	if(v->is_pair) {
		if(depth == 4) {
			merge_left(v, v->left->value);
			merge_right(v, v->right->value);
			free(v->left);
			free(v->right);
			
			v->is_pair = 0;
			v->value = 0;
			return 1;
		}

		if(explode(v->left, depth + 1)) return 1;
		if(explode(v->right, depth + 1)) return 1;
	}
	return 0;
}

int split(struct Value *v) {
	if(v->is_pair) {
		if(split(v->left)) return 1;
		if(split(v->right)) return 1;
	} else {
		if(v->value >= 10) {
			int value = v->value;

			v->left = malloc(sizeof(struct Value));
			v->left->is_pair = 0;
			v->left->parent = v;
			v->left->value = value / 2;

			v->right = malloc(sizeof(struct Value));
			v->right->is_pair = 0;
			v->right->parent = v;
			v->right->value = (value / 2) + (value % 2);
			
			v->is_pair = 1;
			return 1;
		}
	}
	return 0;
}

void reduce(struct Value *v) {
	while(1) {
		if(explode(v, 0)) continue;
		if(split(v)) continue;
		return;
	}
}

int magnitude(struct Value *v) {
	if(v->is_pair) {
		return (3 * magnitude(v->left)) + (2 * magnitude(v->right));
	} else {
		return v->value;
	}
}

int main(void) {
	struct Value *current;
	
	char buffer[512];
	while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
		size_t buflen = strlen(buffer);
		if(buflen < 2) continue;
		buffer[buflen-1] = '\0'; // Get rid of trailing newline

		struct Value *prev = current;
		struct Value *next = parse(buffer);

		if(current == NULL) {
			current = next;
			continue;
		}

		current = malloc(sizeof(struct Value));
		current->parent = NULL;
		current->is_pair = 1;
		current->left = prev;
		current->right = next;

		dump(prev);
		puts("+");
		dump(next);
		puts("=");

		prev->parent = current;
		next->parent = current;
		
		reduce(current);
		dump(current);
		puts("");
	}
	printf("%d\n", magnitude(current));
	free_value(current);
}
