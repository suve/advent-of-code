#include <stdio.h>

int current_group[256];
int current_person;
int current_size;

void reset(void) {
	for(int i = 'a'; i <= 'z'; ++i) current_group[i] = 0;
	current_person = 0;
	current_size = 0;
}

int count_any = 0;
int count_all = 0;

void count(void) {
	for(int i = 'a'; i <= 'z'; ++i) {
		if(current_group[i] > 0) {
			count_any += 1;
			if(current_group[i] == current_size) {
				count_all += 1;
			}
		}
	}
}

int main(void) {
	reset();

	int c;
	while((c = fgetc(stdin)) != EOF) {
		if(c != '\n') {
			current_group[c] += 1;
			current_person += 1;
		} else {
			if(current_person > 0) {
				current_size += 1;
				current_person = 0;
			} else {
				count();
				reset();
			}
		}
	}
	count();

	printf("Any: %d; All: %d\n", count_any, count_all);
}
