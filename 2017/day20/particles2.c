#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Vector3 {
	int x, y, z;
};

struct Vector3 add_vectors(const struct Vector3 a, const struct Vector3 b) {
	return (struct Vector3){
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	};
}

int vectors_equal(const struct Vector3 a, const struct Vector3 b) {
	return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

struct Particle {
	int id;
	struct Vector3 pos, vel, acc;
};

int lines_intersect(
	const int a1x, const int a1y, const int a2x, const int a2y,
	const int b1x, const int b1y, const int b2x, const int b2y
) {
	return (a1x - a2x)*(b1y - b2y) - (a1y - a2y)*(b1x - b2x);
}

int particles_may_collide(const struct Particle a, const struct Particle b) {
	const struct Vector3 a1 = a.pos;
	const struct Vector3 a2 = add_vectors(a.pos, a.vel);
	
	const struct Vector3 b1 = b.pos;
	const struct Vector3 b2 = add_vectors(b.pos, b.vel);
	
	if(lines_intersect(a1.x, a1.y, a2.x, a2.y, b1.x, b1.y, b2.x, b2.y)) {
		if(lines_intersect(a1.y, a1.z, a2.y, a2.z, b1.y, b1.z, b2.y, b2.z)) {
			return 1;
		}
	}
	
	return 0;
}


#define MAX_PARTICLES 1024
struct Particle part[MAX_PARTICLES];
int pnum = 0;


int any_two_particles_may_collide(void) {
	for(int p1 = 0; p1 < pnum; ++p1) {
		for(int p2 = p1+1; p2 < pnum; ++p2) {
			if(particles_may_collide(part[p1], part[p2])) {
				return 1;
			}
		} 
	}
	return 0;
}

void simulation_step(const int step_no) {
	for(int p = 0; p < pnum; ++p) {
		part[p].vel = add_vectors(part[p].vel, part[p].acc);
		part[p].pos = add_vectors(part[p].pos, part[p].vel);
	}
	
	uint8_t destroy[MAX_PARTICLES];
	memset(destroy, 0, sizeof(destroy));
	for(int p1 = 0; p1 < pnum; ++p1) {
		for(int p2 = p1+1; p2 < pnum; ++p2) {
			if(vectors_equal(part[p1].pos, part[p2].pos)) {
				destroy[p1] = destroy[p2] = 1;
			}
		} 
	}
	
	int p = 0;
	while(p < pnum) {
		if(destroy[p]) {
			printf("> step %d: particle %d destroyed\n", step_no, part[p].id);
			
			--pnum;
			part[p] = part[pnum];
			destroy[p] = destroy[pnum];
		} else {
			++p;
		}
	}
}


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
	
	int steps = 0;
	int steps_without_collisions = 0;
	while(steps_without_collisions < 1000) {
		int old_pnum = pnum;
		simulation_step(++steps);
		if(pnum == old_pnum) ++steps_without_collisions;
	}
	
	printf("particles left: %d\n", pnum);
	return 0;
}
