#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys


DEPTH_MAX = 5

PHASE_MIN = 5
PHASE_MAX = 9

BEST_RESULT = 0
BEST_CONFIG = []


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


def run_script(ints, start_pos, input):
	output = []
	finished = False

	pos = start_pos
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

			output.append(get_val(ints, arg, argmode[0]))
			# print("Opcode 4: {}".format(get_val(ints, arg, argmode[0])))
			break
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
			finished = True
			break
		else:
			print("Unknown opcode \"{}\"".format(ints[pos]), file=sys.stderr)
			exit(1)
	
	return pos, output, finished


def str_to_ints(string):
	return list(map(lambda x: int(x), string.split(',')))


def ints_to_str(ints):
	return ",".join(list(map(lambda x: str(x), ints)))


def run(config, script):
	global DEPTH_MAX, BEST_RESULT, BEST_CONFIG

	scripts = []
	script_pos = []
	for i in range(DEPTH_MAX):
		scripts.append(script.copy())
		script_pos.append(0)

	input = 0
	output = None
	
	idx = 0
	firstloop = True
	repeat = True
	while True:
		if firstloop:
			input = [config[idx], input]
		else:
			input = [input]

		# print("running script #{} from [{}] -- input: [{}]".format(idx, ints_to_str(config), ints_to_str(input)))

		endpos, output, finished = run_script(scripts[idx], script_pos[idx], input)
		script_pos[idx] = endpos

		# Short-circuit
		if len(output) == 0: 
			output = input[0]
			break
		
		output  = output[0]

		if finished: repeat = False

		idx += 1
		input = output
		if idx == DEPTH_MAX:
			if repeat:
				idx = 0
				firstloop = False
			else:
				break

	print("{} ---> {}".format(ints_to_str(config), output))

	if output > BEST_RESULT:
		BEST_RESULT = output
		BEST_CONFIG = config


def generate_config_r(conf, depth, max_depth):
	global PHASE_MIN, PHASE_MAX

	result = []
	for phase in range(PHASE_MIN, PHASE_MAX+1):
		if phase in conf: continue

		entry = conf.copy()
		entry.append(phase)

		if depth == max_depth:
			result.append(entry)
		else:
			result += generate_config_r(entry, depth + 1, max_depth)
	
	return result


def generate_configs():
	return generate_config_r([], 0, DEPTH_MAX-1)


if __name__ == "__main__":
	script = str_to_ints(sys.stdin.read())
	configs = generate_configs()

	for i in range(len(configs)):
		run(configs[i], script)

	print("!! {} ---> {}".format(ints_to_str(BEST_CONFIG), BEST_RESULT))

