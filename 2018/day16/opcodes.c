#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define UNUSED(x) ((void)(x))

void opcode_addr(const int argA, const int argB, const int argC, int *const reg) {
	reg[argC] = reg[argA] + reg[argB];
}

void opcode_addi(const int argA, const int argB, const int argC, int *const reg) {
	reg[argC] = reg[argA] + argB;
}

void opcode_mulr(const int argA, const int argB, const int argC, int *const reg) {
	reg[argC] = reg[argA] * reg[argB];
}

void opcode_muli(const int argA, const int argB, const int argC, int *const reg) {
	reg[argC] = reg[argA] * argB;
}

void opcode_banr(const int argA, const int argB, const int argC, int *const reg) {
	reg[argC] = reg[argA] & reg[argB];
}

void opcode_bani(const int argA, const int argB, const int argC, int *const reg) {
	reg[argC] = reg[argA] & argB;
}

void opcode_borr(const int argA, const int argB, const int argC, int *const reg) {
	reg[argC] = reg[argA] | reg[argB];
}

void opcode_bori(const int argA, const int argB, const int argC, int *const reg) {
	reg[argC] = reg[argA] | argB;
}

void opcode_setr(const int argA, const int argB, const int argC, int *const reg) {
	UNUSED(argB);
	reg[argC] = reg[argA];
}

void opcode_seti(const int argA, const int argB, const int argC, int *const reg) {
	UNUSED(argB);
	reg[argC] = argA;
}

void opcode_gtir(const int argA, const int argB, const int argC, int *const reg) {
	reg[argC] = argA > reg[argB];
}

void opcode_gtri(const int argA, const int argB, const int argC, int *const reg) {
	reg[argC] = reg[argA] > argB;
}

void opcode_gtrr(const int argA, const int argB, const int argC, int *const reg) {
	reg[argC] = reg[argA] > reg[argB];
}

void opcode_eqir(const int argA, const int argB, const int argC, int *const reg) {
	reg[argC] = argA == reg[argB];
}

void opcode_eqri(const int argA, const int argB, const int argC, int *const reg) {
	reg[argC] = reg[argA] == argB;
}

void opcode_eqrr(const int argA, const int argB, const int argC, int *const reg) {
	reg[argC] = reg[argA] == reg[argB];
}

typedef void(*OpcodeFunc)(const int, const int, const int, int *const);
struct OpcodeInfo {
	OpcodeFunc func;
	const char *name;
} Opcode[] = {
	#define OPINFO(op) { .func = &opcode_ ## op, .name = #op }
	OPINFO(addr),
	OPINFO(addi),
	OPINFO(mulr),
	OPINFO(muli),
	OPINFO(banr),
	OPINFO(bani),
	OPINFO(borr),
	OPINFO(bori),
	OPINFO(setr),
	OPINFO(seti),
	OPINFO(gtir),
	OPINFO(gtri),
	OPINFO(gtrr),
	OPINFO(eqir),
	OPINFO(eqri),
	OPINFO(eqrr),
};
#define OPCODE_COUNT (int)(sizeof(Opcode) / sizeof(struct OpcodeInfo))


void process_samples(int *opPossible) {
	int sampleNo = 0;
	int threeMatches = 0;
	
	int emptyLines = 0;
	char line[256];
	while(fgets(line, sizeof(line), stdin) != NULL) {
		if(strncmp(line, "Before: ", 8) != 0) {
			if(++emptyLines > 1) 
				break;
			else
				continue;
		}
		emptyLines = 0;

		int before[4];
		sscanf(line, "Before: [%d, %d, %d, %d]", before+0, before+1, before+2, before+3);

		fgets(line, sizeof(line), stdin);
		int opcodeNo, argA, argB, argC;
		sscanf(line, "%d %d %d %d", &opcodeNo, &argA, &argB, &argC);

		fgets(line, sizeof(line), stdin);
		int after[4];
		sscanf(line, "After: [%d, %d, %d, %d]", after+0, after+1, after+2, after+3);

		int matches = 0;
		int match[OPCODE_COUNT];
		for(int i = 0; i < OPCODE_COUNT; ++i) {
			int registers[4];
			memcpy(registers, before, sizeof(registers));

			Opcode[i].func(argA, argB, argC, registers);
			
			int afterOk = 0;
			for(int r = 0; r < 4; ++r) afterOk += registers[r] == after[r];
			
			if(afterOk == 4) {
				match[i] = 1;
				matches += 1;
			} else {
				match[i] = 0;
				opPossible[opcodeNo * OPCODE_COUNT + i] = 0;
			}
		}

		printf("Sample %d behaves like %d opcodes: ", sampleNo, matches);
		for(int i = 0; i < OPCODE_COUNT; ++i) {
			if(match[i]) printf("%s ", Opcode[i].name);
		}
		puts("");

		++sampleNo;
		if(matches >= 3) ++threeMatches;
	}

	printf("--> part1: %d\n", threeMatches);
}


void decode_opcodes(int *opPossible, int *opMap) {
	for(int o = 0; o < OPCODE_COUNT; ++o) {
		printf("opcode %d may be: ", o);
		for(int i = 0; i < OPCODE_COUNT; ++i) {
			if(opPossible[o * OPCODE_COUNT + i]) printf("%s ", Opcode[i].name);
		}
		puts("");
	}	

	int singles;
	do {
		int howmany[OPCODE_COUNT];
		for(int op = 0; op < OPCODE_COUNT; ++op) {
			howmany[op] = 0;
			for(int i = 0; i < OPCODE_COUNT; ++i) {
				if(opPossible[op * OPCODE_COUNT + i]) {
					howmany[op] += 1;
					opMap[op] = i;
				}
			}
		}

		singles = 0;
		for(int op = 0; op < OPCODE_COUNT; ++op) {
			if(howmany[op] != 1) continue;
			++singles;

			for(int other = 0; other < OPCODE_COUNT; ++other) {
				if(other == op) continue;

				int offset = other * OPCODE_COUNT + opMap[op];
				if(opPossible[offset]) {
					opPossible[offset] = 0;
				}
			}
		}

	} while(singles != OPCODE_COUNT);

	printf("--> opcodes decoded\n");
	for(int op = 0; op < OPCODE_COUNT; ++op) {
		printf("opcode %d is %s\n", op, Opcode[opMap[op]].name);
	}
}

void run_program(const int *opMap) {
	int reg[4];
	memset(reg, 0, sizeof(reg));

	char line[256];
	while(fgets(line, sizeof(line), stdin) != NULL) {
		int opcodeNo, argA, argB, argC;
		if(sscanf(line, "%d %d %d %d", &opcodeNo, &argA, &argB, &argC) != 4) continue;

		Opcode[opMap[opcodeNo]].func(argA, argB, argC, reg);
	}

	printf("--> part2: %d %d %d %d\n", reg[0], reg[1], reg[2], reg[3]);
}

int main(void) {
	int opMap[OPCODE_COUNT];
	memset(opMap, 0, sizeof(opMap));

	int opPossible[OPCODE_COUNT * OPCODE_COUNT];
	memset(opPossible, 1, sizeof(opPossible));

	process_samples(opPossible);
	decode_opcodes(opPossible, opMap);
	run_program(opMap);
}
