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
		elif ints[pos] == 99:
			break
		else:
			print("Unknown opcode \"{}\"".format(ints[pos]), file=sys.stderr)
			exit(1)
	
	return ints


def read_input():
	input = sys.stdin.read().split(',')
	input = list(map(lambda x: int(x), input))
	return input


if __name__ == "__main__":
	run(read_input(), [1])

