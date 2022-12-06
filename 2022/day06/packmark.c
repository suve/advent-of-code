#include <stdio.h>

#define SOP_COUNT 4
#define SOM_COUNT 14

int distinct(const int *const chars, const int count) {
	for(int i = 0; i < (count-1); ++i) {
		for(int j = (i+1); j < count; ++j) {
			if(chars[i] == chars[j]) return 0;
		}
	}
	return 1;
}

#define NOT_FOUND (-1)
#define END_OF_FILE (-2)

int parse_line(int *sop, int *som) {
	int hist_sop[SOP_COUNT];
	int hist_som[SOM_COUNT];

	*sop = 0;
	*som = 0;

	int pos = 0;
	while(1) {
		int r = fgetc(stdin);
		if(r == EOF) return (pos == 0) ? END_OF_FILE : NOT_FOUND;
		if(r == '\n') return NOT_FOUND;

		hist_sop[pos % SOP_COUNT] = r;
		hist_som[pos % SOM_COUNT] = r;
		++pos;

		if((*sop == 0) && (pos >= SOP_COUNT) && distinct(hist_sop, SOP_COUNT)) *sop = pos;
		if((pos >= SOM_COUNT) && distinct(hist_som, SOM_COUNT)) {
			*som = pos;
			break;
		}
	}

	// Read until end of line
	int lf = 0;
	do {
		lf = fgetc(stdin);
	} while((lf != '\n') && (lf != EOF));

	return 0;
}

int main(void) {
	while(1) {
		int sop, som;

		int ok = parse_line(&sop, &som);
		if(ok == 0) {
			printf("%d, %d\n", sop, som);
		} else if(ok == NOT_FOUND) {
			printf("ERROR\n");
		} else {
			break;
		}
	}
	return 0;
}
