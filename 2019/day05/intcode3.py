#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys


def decode_opcode(value):
	op = value % 100
	value = value // 100
	
	argmode = []
	for i in range(3):
		argmode.append(value % 10)
		value = value // 10

	return op, argmode


def get_val(ints, position, mode):
	if mode == 0:
		return ints[position]
	elif mode == 1:
		return position
	else:
		print("What the fuck", file=sys.stderr)
		exit(1)


def run(ints, input):
	pos = 0
	while True:
		op, argmode = decode_opcode(ints[pos])

		if op == 1:
			arg1 = ints[pos+1]
			arg2 = ints[pos+2]
			arg3 = ints[pos+3]
			pos += 4
			
			ints[arg3] = get_val(ints, arg1, argmode[0]) + get_val(ints, arg2, argmode[1])
		elif op == 2:
			arg1 = ints[pos+1]
			arg2 = ints[pos+2]
			arg3 = ints[pos+3]
			pos += 4
			
			ints[arg3] = get_val(ints, arg1, argmode[0]) * get_val(ints, arg2, argmode[1])
		elif op == 3:
			arg = ints[pos+1]
			pos += 2

			ints[arg] = input.pop(0)
		elif op == 4:
			arg = ints[pos+1]
			pos += 2

			print("Opcode 4: {}".format(get_val(ints, arg, argmode[0])))
		elif (op == 5) or (op == 6):
			arg1 = ints[pos+1]
			arg2 = ints[pos+2]
			
			val = get_val(ints, arg1, argmode[0])
			if ((op == 5) and (val != 0)) or ((op == 6) and (val == 0)):
				pos = get_val(ints, arg2, argmode[1])
			else:
				pos += 3
		elif (op == 7) or (op == 8):
			arg1 = ints[pos+1]
			arg2 = ints[pos+2]
			arg3 = ints[pos+3]
			pos += 4

			val1 = get_val(ints, arg1, argmode[0])
			val2 = get_val(ints, arg2, argmode[1])

			condition = ((op == 7) and (val1 < val2)) or ((op == 8) and (val1 == val2))
			ints[arg3] = 1 if condition else 0
		elif op == 99:
			break
		else:
			print("Unknown opcode \"{}\"".format(ints[pos]), file=sys.stderr)
			exit(1)
	
	return ints


if __name__ == "__main__":
	for line in sys.stdin:
		ints = list(map(lambda x: int(x), line.split(',')))
		run(ints, [5])

