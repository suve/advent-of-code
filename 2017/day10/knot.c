#include <stdio.h>
#include <stdlib.h>

int *elem;

void alloc_array(const int size) {
	elem = malloc(size * sizeof(int));
	for(int i = 0; i < size; ++i) elem[i] = i;
}

void visualise(const int startPos, const int length, const int size) {
	const int endPos = (startPos + length-1) % size;
	
	for(int i = 0; i < size; ++i) {
		if(i == startPos)
			printf("([%d] ", elem[i]);
		else if(i == endPos)
			printf("%d) ", elem[i]);
		else
			printf("%d ", elem[i]);
	}
	printf("\n");
}

void reverse(const int startPos, const int length, const int size) {
	const int endPos = (startPos + length - 1) % size;
	
	for(int i = 0; i < length/2; ++i) {
		int a_p = (startPos+i) % size;
		
		int b_p = (endPos-i);
		if(b_p < 0) b_p += size;
		
		int a_v = elem[a_p];
		int b_v = elem[b_p];
		
		elem[a_p] = b_v;
		elem[b_p] = a_v;
	}
}

int main(void) {
	int size;
	scanf("%d ", &size);
	alloc_array(size);
	
	int pos = 0;
	int skip = 0;
	
	int length;
	while(scanf("%d ", &length) == 1) {
		visualise(pos, length, size);
		reverse(pos, length, size);
		
		pos = (pos + length + skip) % size;
		skip += 1;
	}
	visualise(pos, 0, size);
	
	printf("%d x %d = %d\n", elem[0], elem[1], elem[0]*elem[1]);
	
	return 0;
}
