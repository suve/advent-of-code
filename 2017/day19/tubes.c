#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_W 256
#define MAX_H 256

char map[MAX_W * MAX_H];
#define MAP(x, y)   map[((y) * MAX_W) + (x)]

int w = 0;
int h = 0;

void read_input(void) {
	memset(map, 0, sizeof(map));
	char buffer[MAX_W+2];
	
	while(1) {
		if(fgets(buffer, sizeof(buffer), stdin) == NULL) break;
		
		int len = strlen(buffer);
		if(buffer[len-1] == '\n') buffer[--len] = '\0';
		if(len == 0) break;
		
		if(w == 0) {
			printf("> width detected as %d\n", len);
			w = len;
		} else if(w != len) {
			printf("> line %d width (%d) doesn't match %d\n", h, len, w);
			exit(EXIT_FAILURE); 
		}
		
		memcpy(map + h * MAX_W, buffer, len);
		h += 1;
	} 
	printf("> height is %d\n", h);
}

#define IS_LETTER(m)  ( ((m >= 'A') && (m <= 'Z')) || ((m >= 'a') && (m <= 'z')) ) 
#define IS_PIPE(m)    ( (m == '|') || (m == '-') || (m == '+') )

int find_start_pos(void) {
	int x = 0;
	while(!IS_PIPE(map[x])) ++x;
	
	return x;
}

enum Direction {
	DIR_UP,
	DIR_RI,
	DIR_DO,
	DIR_LE
};

void calc_movement(int *const x, int *const y, const enum Direction dir) {
	if(dir == DIR_UP)
		*y -= 1;
	else if(dir == DIR_RI)
		*x += 1;
	else if(dir == DIR_DO)
		*y += 1;
	else if(dir == DIR_LE)
		*x -= 1;
}

int go_forward(int *const x, int *const y, enum Direction *const dir) {
	int nx = *x, ny = *y;
	calc_movement(&nx, &ny, *dir);
	
	char nm = MAP(nx, ny);
	if(IS_LETTER(nm) || IS_PIPE(nm)) {
		*x = nx;
		*y = ny;
		return 1;
	}
	
	
	int right_x = *x, right_y = *y;
	enum Direction right_dir = (*dir+1) % 4;
	calc_movement(&right_x, &right_y, right_dir);
	
	char right_m = MAP(right_x, right_y);
	if(IS_LETTER(right_m) || IS_PIPE(right_m)) {
		*x = right_x;
		*y = right_y;
		*dir = right_dir;
		return 1;
	}
	
	
	int left_x = *x, left_y = *y;
	enum Direction left_dir = (*dir == 0) ? 3 : *dir-1;
	calc_movement(&left_x, &left_y, left_dir);
	
	char left_m = MAP(left_x, left_y);
	if(IS_LETTER(left_m) || IS_PIPE(left_m)) {
		*x = left_x;
		*y = left_y;
		*dir = left_dir;
		return 1;
	}
	
	return 0;
}

int main(void) {
	read_input();
	
	int x = find_start_pos();
	int y = 0; 
	enum Direction dir = DIR_DO;
	
	char letters[64];
	memset(letters, 0, sizeof(letters));
	int let = 0;
	
	int steps = 1;
	while(go_forward(&x, &y, &dir)) {
		++steps;
		
		char m = MAP(x, y);
		if(IS_LETTER(m)) {
			letters[let++] = m;
		}
	}
	printf("path: %s\nsteps: %d\n", letters, steps);
	
	return 0;
}
