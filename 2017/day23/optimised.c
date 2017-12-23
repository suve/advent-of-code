#include <math.h>
#include <stdio.h>

#define NON_DEBUG_RUN 1

int is_prime(const int val) {
	int val_sqrt = sqrt(val);
	
	for(int x1 = 2; x1 <= val_sqrt; ++x1) {
		if((val % x1) == 0) return 0;
	}
	
	printf(">> %d is prime\n", val);
	return 1;
}

int main(void) {
	int a = NON_DEBUG_RUN;
	int b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;
	
	b = c = 93;
	if(a) {
		b *= 100;
		b += 100000;
		c = b;
		c += 17000;
	}
	
	while(1) {
		f = 1;
		d = e = b;
		
		if(!is_prime(b)) f = 0;
		

		
		
		
		
		if(f == 0) h += 1;
		
		//g = b - c;
		//if(g == 0)
		if(b == c) {
			printf("> h: %d\n", h);
			return 0;
		}
		
		b += 17;
	}
}
