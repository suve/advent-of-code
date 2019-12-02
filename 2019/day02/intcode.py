#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys

mode = None
if sys.argv[1] == "--run":
	mode = 0
elif sys.argv[1] == "--restore":
	mode = 1
else:
	print("You must specify the execution mode (either --run or --restore)", file=sys.stderr)
	exit(1)


ints = sys.stdin.read().split(',')
ints = list(map(lambda x: int(x), ints))


if mode == 1:
	ints[1] = 12
	ints[2] = 2


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

print(",".join(map(lambda x: str(x), ints)))
