#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CAVES 32
#define START_INDEX 0
#define END_INDEX 1

struct Map {
	int count;
	int is_big[MAX_CAVES];
	int *connections[MAX_CAVES];
	char *names[MAX_CAVES];
};

int find_or_add_cave(struct Map *m, const char *const cave) {
	for(int idx = 0; idx < m->count; ++idx) {
		if(strcmp(m->names[idx], cave) == 0) return idx;
	}

	const size_t len = strlen(cave) + 1;
	m->names[m->count] = malloc(len);
	memcpy(m->names[m->count], cave, len);

	m->count += 1;
	return (m->count - 1);
}

struct Map read_input(void) {
	struct Map m;

	m.count = 2;
	m.names[START_INDEX] = "start";
	m.names[END_INDEX] = "end";

	for(int i = 0; i < MAX_CAVES; ++i) {
		m.connections[i] = malloc(sizeof(int) * MAX_CAVES);
		memset(m.connections[i], 0, sizeof(int) * MAX_CAVES);
	}

	char buffer[32];
	while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
		const int buflen = strlen(buffer);
		buffer[buflen-1] = '\0'; // Remove the trailing newline

		char *const dash = strchr(buffer, '-');
		*dash = '\0';
		
		char *const name1 = buffer;
		char *const name2 = dash + 1;

		const int idx1 = find_or_add_cave(&m, name1);
		m.is_big[idx1] = (name1[0] >= 'A') && (name1[0] <= 'Z');

		const int idx2 = find_or_add_cave(&m, name2);
		m.is_big[idx2] = (name2[0] >= 'A') && (name2[0] <= 'Z');

		m.connections[idx1][idx2] = m.connections[idx2][idx1] = 1;
	}

	return m;
}

int count_paths_r(struct Map *m, int current, int *visited) {
	if(current == END_INDEX) return 1;
	
	int count = 0;
	for(int i = 0; i < m->count; ++i) {
		if((m->connections[current][i]) && (!visited[i])) {
			int visited_cpy[MAX_CAVES];
			memcpy(visited_cpy, visited, sizeof(visited_cpy));

			if(!m->is_big[current]) visited_cpy[current] = 1;
			count += count_paths_r(m, i, visited_cpy);
		}
	}
	return count;
}

int count_paths(struct Map *m) {
	int visited[MAX_CAVES];
	memset(visited, 0, sizeof(visited));
	return count_paths_r(m, START_INDEX, visited);
} 

int main(void) {
	struct Map m = read_input();
	int count = count_paths(&m);
	printf("Part1: %d\n", count);
	return 0;
}
