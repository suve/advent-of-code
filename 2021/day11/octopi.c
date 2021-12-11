#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10
#define MIN_STEPS 100

void read_input(char *energy) {
	int lines = 0;

	char buffer[16];
	while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
		if(lines > SIZE) {
			fprintf(stderr, "Output too large: over %d lines\n", SIZE);
			exit(EXIT_FAILURE);
		}

		const int len = strlen(buffer);
		if(len != SIZE + 1) { // fgets() returns a trailing newline
			fprintf(
				stderr, 
				"Invalid line length: expected %d characters, got %d on line %d\n",	
				SIZE,
				len - 1,
				lines
			);
			exit(EXIT_FAILURE);
		}

		for(int i = 0; i < SIZE; ++i) {
			if((buffer[i] < '0') || (buffer[i] > '9')) {
				fprintf(stderr, "Illegal character found ('%c') in input on line %d\n", buffer[i], lines);
				exit(EXIT_FAILURE);
			}
		}
		memcpy(energy + (lines * SIZE), buffer, SIZE);
		++lines;
	}
}

#define STACK_SIZE 256

struct Stack {
	int data[STACK_SIZE];
	int count;
};

void push(struct Stack *stack, const int value) {
	if(stack->count == STACK_SIZE) {
		fprintf(stderr, "Stack size exceeded (%d)\n", stack->count);
		exit(EXIT_FAILURE);
	}

	stack->data[stack->count] = value;
	stack->count += 1;
}

int pop(struct Stack *stack) {
	if(stack->count == 0) {
		fprintf(stderr, "Cannot pop from empty stack\n");
		exit(EXIT_FAILURE);
	}

	stack->count -= 1;
	return stack->data[stack->count];
}

int main(void) {
	char energy[SIZE*SIZE];
	read_input(energy);

	char flashed[SIZE*SIZE];
	struct Stack stack;

	int flash_count = 0;
	int all_flash_on = 0;
	for(int step = 1; (step <= MIN_STEPS) || (all_flash_on == 0); ++step) {
		memset(flashed, 0, sizeof(flashed));
		stack.count = 0;

		for(int i = 0; i < SIZE * SIZE; ++i) {
			if(++energy[i] > '9') push(&stack, i);				
		}
		
		while(stack.count > 0) {
			const int position = pop(&stack);
			if(flashed[position]) continue;

			flashed[position] = 1;
			if(step <= MIN_STEPS) {
				// For Part1, we want to count flashes only in first MIN_STEPS
				++flash_count;
			}

			const int y = position / SIZE;
			const int x = position % SIZE;
			for(int dy = -1; dy <= +1; ++dy) {
				const int ny = y + dy;
				if((ny < 0) || (ny >= SIZE)) continue;			

				for(int dx = -1; dx <= +1; ++dx) {
					if((dy == 0) && (dx == 0)) continue;

					const int nx = x + dx;
					if((nx < 0) || (nx >= SIZE)) continue;
					
					const int offset = (ny * SIZE) + nx;
					if(!flashed[offset]) {
						if(++energy[offset] > '9') {
							push(&stack, offset);
						}
					}
				}
			}
		}

		int all_flashed = 1;
		for(int i = 0; i < SIZE * SIZE; ++i) {
			if(flashed[i])
				energy[i] = '0';
			else
				all_flashed = 0;
		}

		if((all_flashed) && (all_flash_on == 0)) {
			all_flash_on = step;
		}
	}
	printf("Part1: %d\n", flash_count);
	printf("Part2: %d\n", all_flash_on);
}
