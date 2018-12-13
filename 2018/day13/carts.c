#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Direction {
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT
};

void turn_left(enum Direction *d) {
	if(*d == DIR_UP)
		*d = DIR_LEFT;
	else
		*d -= 1;
}

void turn_right(enum Direction *d) {
	*d = (*d + 1) % 4;
}

struct Cart {
	int x, y;
	enum Direction dir;
	int turns;
};

int compare_carts(const void* A, const void *B) {
	const struct Cart *ca = (const struct Cart*)A;
	const struct Cart *cb = (const struct Cart*)B;
	
	if(ca->y < cb->y) return -1;
	if(ca->y > cb->y) return +1;
	
	if(ca->x < cb->x) return -1;
	if(ca->x > cb->x) return +1;
	
	return 0;
}

#define MAX_W 200
#define MAX_H 200
#define OFFSET(x, y)    ( ((y) * MAX_W) + (x) )

char track[MAX_W * MAX_H];
int w, h;

#define MAX_CARTS 64
struct Cart cart[MAX_CARTS];
int cart_num;

void detect_carts(char *line, const size_t len) {
	for(int x = 0; x < len; ++x) {
		enum Direction d;
		switch(line[x]) {
			case '^': d = DIR_UP;    break;
			case '>': d = DIR_RIGHT; break;
			case 'v': d = DIR_DOWN;  break;
			case '<': d = DIR_LEFT;  break;
			default: goto loop_end;
		}
		
		if(cart_num == MAX_CARTS) {
			fprintf(stderr, "Maximum number of carts (%d) exceeded!\n", MAX_CARTS);
			exit(1);
		}
		
		cart[cart_num].x = x;
		cart[cart_num].y = h;
		cart[cart_num].dir = d;
		cart[cart_num].turns = 0;
		++cart_num;
		
		printf("--> cart #%d at %d,%d\n", cart_num, x, h);
		
		line[x] = ((d == DIR_UP) || (d == DIR_DOWN)) ? '|' : '-';
		
		
		loop_end: /* empty statement */;
	}
}

void read_input(void) {
	w = h = cart_num = 0;
	
	while(!feof(stdin)) {
		char line[256];
		fgets(line, sizeof(line), stdin);
		
		size_t len = strlen(line);
		if(len == 0) continue;
		
		while(line[len-1] == '\n') line[--len] = '\0';
		
		detect_carts(line, len);
		memcpy(&track[OFFSET(0, h)], line, len);
		
		if(len > w) w = len;
		++h;
	}
}

void tick(const int tick_no) {
	qsort(cart, cart_num, sizeof(struct Cart), &compare_carts);
	
	int crashes = 0;
	for(int i = 0; i < cart_num; ++i) {
		if((cart[i].x < 0) || (cart[i].y < 0)) continue;
		
		switch(cart[i].dir) {
			case DIR_UP:    --cart[i].y; break;
			case DIR_RIGHT: ++cart[i].x; break;
			case DIR_DOWN:  ++cart[i].y; break;
			case DIR_LEFT:  --cart[i].x; break;
		}
		
		int offset = OFFSET(cart[i].x, cart[i].y);
		switch(track[offset]) {
			case  '/':
				cart[i].dir =
					(cart[i].dir == DIR_UP)    ? DIR_RIGHT :
					(cart[i].dir == DIR_RIGHT) ? DIR_UP    :
					(cart[i].dir == DIR_DOWN)  ? DIR_LEFT  :
					(cart[i].dir == DIR_LEFT)  ? DIR_DOWN  : -1
				;
			break;
			
			case '\\':
				cart[i].dir =
					(cart[i].dir == DIR_UP)    ? DIR_LEFT  :
					(cart[i].dir == DIR_RIGHT) ? DIR_DOWN  :
					(cart[i].dir == DIR_DOWN)  ? DIR_RIGHT :
					(cart[i].dir == DIR_LEFT)  ? DIR_UP    : -1
				;
			break;
			
			case  '+':
				if(cart[i].turns == 0)
					turn_left(&cart[i].dir);
				else if(cart[i].turns == 2)
					turn_right(&cart[i].dir);
				
				cart[i].turns = (cart[i].turns + 1) % 3;
			break;
		}
		
		for(int j = 0; j < cart_num; ++j) {
			if(i == j) continue;
			
			if(cart[i].x != cart[j].x) continue;
			if(cart[i].y != cart[j].y) continue;
			
			printf("crash at %d,%d on tick %d\n", cart[i].x, cart[i].y, tick_no);
			
			crashes += 2;
			cart[i].x = cart[j].x = cart[i].y = cart[j].y = -1;
			break;
		}
	}
	
	int i = 0;
	while(crashes > 0) {
		if((cart[i].x >= 0) || (cart[i].y >= 0)) { ++i; continue; }
		
		for(int j = i+1; j < cart_num; ++j) cart[j-1] = cart[j];
		--cart_num;
		--crashes;
	}
}

int main(void) {
	read_input();
	
	int tick_no = 0;
	while(1) {
		tick(++tick_no);
		if(cart_num < 2) break;
	}
	
	if(cart_num == 1)
		printf("last cart at %d,%d\n", cart[0].x, cart[0].y);
	else
		printf("no carts left\n");
}
