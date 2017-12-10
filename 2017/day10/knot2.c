#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ROUNDS 64

#define SIZE 256
int elem[SIZE];

#define EXTRA_LENGHTS 5
const int extra[EXTRA_LENGHTS] = { 17, 31, 73, 47, 23 };


void fill_array(void) {
	for(int i = 0; i < SIZE; ++i) elem[i] = i;
}

void visualise(const int startPos, const int length) {
	const int endPos = (startPos + length-1) % SIZE;
	
	for(int i = 0; i < SIZE; ++i) {
		if(i == startPos)
			printf("([%d] ", elem[i]);
		else if(i == endPos)
			printf("%d) ", elem[i]);
		else
			printf("%d ", elem[i]);
	}
	printf("\n");
}

void reverse(const int startPos, const int length) {
	const int endPos = (startPos + length - 1) % SIZE;
	
	for(int i = 0; i < length/2; ++i) {
		int a_p = (startPos+i) % SIZE;
		
		int b_p = (endPos-i);
		if(b_p < 0) b_p += SIZE;
		
		int a_v = elem[a_p];
		int b_v = elem[b_p];
		
		elem[a_p] = b_v;
		elem[b_p] = a_v;
	}
}

void print_hash(void) {
	for(int group = 0; group < 16; ++group) {
		uint8_t hash = 0;
		for(int member = 0; member < 16; ++member) {
			hash ^= elem[group*16 + member];
		}
		
		printf("%02x", hash);
	}
	putc('\n', stdout);
}

int main(void) {
	fill_array();
	
	char buffer[128];
	int buflen;
	
	if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
		buffer[0] = '\0';
		buflen = 0;
	} else {
		buflen = strlen(buffer);
		if(buffer[buflen-1] == '\n') buffer[--buflen] = '\0';
	}
	
	printf("buffer: \"%s\" (%d)\n", buffer, buflen);
	
	int pos = 0;
	int skip = 0;
	for(int round = 0; round < ROUNDS; ++round) {
		for(int b = 0; b < buflen; ++b) {
			int length = buffer[b];
			
			// visualise(pos, length);
			reverse(pos, length);
			
			pos = (pos + length + skip) % SIZE;
			skip += 1;
		}
		for(int e = 0; e < EXTRA_LENGHTS; ++e) {
			int length = extra[e];
			
			// visualise(pos, length);
			reverse(pos, length);
			
			pos = (pos + length + skip) % SIZE;
			skip += 1;
		}
	}
	print_hash();
	
	return 0;
}
