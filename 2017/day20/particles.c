#include <stdio.h>
#include <stdlib.h>

struct Vector3 {
	int x, y, z;
};

struct Particle {
	int id;
	struct Vector3 pos, vel, acc;
};

int manhattan(const struct Vector3 vec) {
	return abs(vec.x) + abs(vec.y) + abs(vec.z);
}

int relation(const int a, const int b) {
	if(a > b) return +1;
	if(a < b) return -1;
	return 0;
}

int CompareParticles(const void *A_void, const void *B_void) {
	const struct Particle *A = A_void;
	const struct Particle *B = B_void;
	
	int a = manhattan(A->acc);
	int b = manhattan(B->acc);
	int cmp = relation(a, b);
	if(cmp != 0) return cmp;
	
	a = manhattan(A->vel);
	b = manhattan(B->vel);
	cmp = relation(a, b);
	if(cmp != 0) return cmp;
	
	a = manhattan(A->pos);
	b = manhattan(B->pos);
	return relation(a, b);
}

#define MAX_PARTICLES 1024
struct Particle part[MAX_PARTICLES];
int pnum = 0;


int readline(void) {
	int matches = scanf(
		"p=<%d,%d,%d>, v=<%d,%d,%d>, a=<%d,%d,%d> ",
		&part[pnum].pos.x, &part[pnum].pos.y, &part[pnum].pos.z,
		&part[pnum].vel.x, &part[pnum].vel.y, &part[pnum].vel.z,
		&part[pnum].acc.x, &part[pnum].acc.y, &part[pnum].acc.z
	);
	
	return matches == 9;
}

int main(void) {
	while(readline()) {
		part[pnum].id = pnum;
		++pnum;
	}
	printf("> %d particles read\n", pnum);
	
	qsort(part, pnum, sizeof(struct Particle), &CompareParticles);
	
	printf("particle %d\n", part[0].id);
	return 0;
}
