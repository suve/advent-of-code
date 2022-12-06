#include <stdio.h>

#define NOT_FOUND (-1)
#define END_OF_FILE (-2)

int parse_line(void) {
	int c[4];
	for(int i = 0; i < 3; ++i) {
		c[i] = fgetc(stdin);
		if(c[i] == '\n') return NOT_FOUND;
		if(c[i] == EOF) return END_OF_FILE;
	}

	int pos = 3;
	while(1) {
		int r = pos % 4;
		c[r] = fgetc(stdin);
		if(c[r] == '\n') return NOT_FOUND;
		if(c[r] == EOF) return NOT_FOUND;

		if((c[0] != c[1]) && (c[0] != c[2]) && (c[0] != c[3]) && (c[1] != c[2]) && (c[1] != c[3]) && (c[2] != c[3])) {
			break;
		}
		++pos;
	}

	// Read until end of line
	int lf = 0;
	do {
		lf = fgetc(stdin);
	} while((lf != '\n') && (lf != EOF));

	return pos+1;
}

int main(void) {
	while(1) {
		int r = parse_line();
		if(r >= 0) {
			printf("%d\n", r);
		} else if(r == NOT_FOUND) {
			printf("ERROR\n", r);
		} else {
			break;
		}
	}
	return 0;
}
