#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define UNUSED(x) ((void)(x))

#define REGISTERS_NUM 6
int reg[REGISTERS_NUM];


void opcode_addr(const int argA, const int argB, const int argC) {
	reg[argC] = reg[argA] + reg[argB];
}

void opcode_addi(const int argA, const int argB, const int argC) {
	reg[argC] = reg[argA] + argB;
}

void opcode_mulr(const int argA, const int argB, const int argC) {
	reg[argC] = reg[argA] * reg[argB];
}

void opcode_muli(const int argA, const int argB, const int argC) {
	reg[argC] = reg[argA] * argB;
}

void opcode_banr(const int argA, const int argB, const int argC) {
	reg[argC] = reg[argA] & reg[argB];
}

void opcode_bani(const int argA, const int argB, const int argC) {
	reg[argC] = reg[argA] & argB;
}

void opcode_borr(const int argA, const int argB, const int argC) {
	reg[argC] = reg[argA] | reg[argB];
}

void opcode_bori(const int argA, const int argB, const int argC) {
	reg[argC] = reg[argA] | argB;
}

void opcode_setr(const int argA, const int argB, const int argC) {
	UNUSED(argB);
	reg[argC] = reg[argA];
}

void opcode_seti(const int argA, const int argB, const int argC) {
	UNUSED(argB);
	reg[argC] = argA;
}

void opcode_gtir(const int argA, const int argB, const int argC) {
	reg[argC] = argA > reg[argB];
}

void opcode_gtri(const int argA, const int argB, const int argC) {
	reg[argC] = reg[argA] > argB;
}

void opcode_gtrr(const int argA, const int argB, const int argC) {
	reg[argC] = reg[argA] > reg[argB];
}

void opcode_eqir(const int argA, const int argB, const int argC) {
	reg[argC] = argA == reg[argB];
}

void opcode_eqri(const int argA, const int argB, const int argC) {
	reg[argC] = reg[argA] == argB;
}

void opcode_eqrr(const int argA, const int argB, const int argC) {
	reg[argC] = reg[argA] == reg[argB];
}


enum Opcode {
	OP_ADDR,
	OP_ADDI,
	OP_MULR,
	OP_MULI,
	OP_BANR,
	OP_BANI,
	OP_BORR,
	OP_BORI,
	OP_SETR,
	OP_SETI,
	OP_GTIR,
	OP_GTRI,
	OP_GTRR,
	OP_EQIR,
	OP_EQRI,
	OP_EQRR,
	_OP_COUNT_,
};

typedef void (*OpFunc)(const int argA, const int argB, const int argC);
OpFunc opfunc[_OP_COUNT_] = {
	&opcode_addr,
	&opcode_addi,
	&opcode_mulr,
	&opcode_muli,
	&opcode_banr,
	&opcode_bani,
	&opcode_borr,
	&opcode_setr,
	&opcode_seti,
	&opcode_gtir,
	&opcode_gtri,
	&opcode_gtrr,
	&opcode_eqir,
	&opcode_eqri,
	&opcode_eqrr,
};

const char* opname[_OP_COUNT_] = {
	"addr",
	"addi",
	"mulr",
	"muli",
	"banr",
	"bani",
	"borr",
	"setr",
	"seti",
	"gtir",
	"gtri",
	"gtrr",
	"eqir",
	"eqri",
	"eqrr",
};


struct Instruction {
	enum Opcode op;
	int a, b, c;
};

#define INSTRUCTIONS_MAX 64
int instructionCount = 0;

struct Instruction instr[INSTRUCTIONS_MAX];

struct {
	int value;
	int alias;
} iptr;


void parse_input(void) {
	char buffer[128];
	while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
		if(buffer[0] == '#') {
			sscanf(buffer+3, "%d", &iptr.alias);
			fprintf(stderr, "--> iptr alias: %d\n", iptr.alias);
			continue;
		}

		buffer[4] = '\0';
		
		int op;
		for(op = 0; op < _OP_COUNT_; ++op) {
			if(strcmp(buffer, opname[op]) == 0) break;
		}
		if(op == _OP_COUNT_) {
			fprintf(stderr, "Unrecognized instruction: %c%c%c%c\n", buffer[0], buffer[1], buffer[2], buffer[3]);
			exit(1);
		}

		int argA, argB, argC;
		sscanf(buffer+5, "%d %d %d", &argA, &argB, &argC);

		instr[instructionCount++] = (struct Instruction){
			.op = op,
			.a = argA,
			.b = argB,
			.c = argC
		};
	}
	fprintf(stderr, "--> instruction count: %d\n", instructionCount);
}

int execute(void) {
	while((iptr.value >= 0) && (iptr.value < instructionCount)) {
		struct Instruction *i = &instr[iptr.value];

		reg[iptr.alias] = iptr.value;
		opfunc[i->op](i->a, i->b, i->c);
		iptr.value = reg[iptr.alias];
		iptr.value += 1;
	}
}

int reset(void) {
	iptr.value = iptr.alias = 0;
	for(int r = 0; r < REGISTERS_NUM; ++r) {
		reg[r] = 0;
	}
}

int main(int argc, char **argv) {
	reset();
	parse_input();
	execute();

	printf("ip: %d; reg: [%d, %d, %d, %d, %d, %d]\n", iptr.value, reg[0], reg[1], reg[2], reg[3], reg[4], reg[5]);
}
