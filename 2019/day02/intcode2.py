#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys

def run(input, noun, verb):
	ints = input.copy()
	
	ints[1] = noun
	ints[2] = verb
	
	pos = 0
	while True:
		if ints[pos] == 1:
			arg1 = ints[pos+1]
			arg2 = ints[pos+2]
			arg3 = ints[pos+3]
			pos += 4
			
			ints[arg3] = ints[arg1] + ints[arg2]
		elif ints[pos] == 2:
			arg1 = ints[pos+1]
			arg2 = ints[pos+2]
			arg3 = ints[pos+3]
			pos += 4
			
			ints[arg3] = ints[arg1] * ints[arg2]
		elif ints[pos] == 99:
			break
		else:
			print("Unknown opcode \"{}\"".format(ints[pos]), file=sys.stderr)
			exit(1)
	
	return ints


if __name__ == "__main__":
	input = sys.stdin.read().split(',')
	input = list(map(lambda x: int(x), input))

	for noun in range(100):
		for verb in range(100):
			output = run(input, noun, verb)
			if output[0] == 19690720:
				print("noun: {}; verb: {}; checksum: {}".format(noun, verb, 100 * noun + verb))
				exit(0)
	
	print("NOT FOUND  ¯\_ツ_/¯")
	exit(1)

