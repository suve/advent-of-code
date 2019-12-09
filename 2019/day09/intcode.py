#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys


def str_to_ints(string):
	return list(map(lambda x: int(x), string.split(',')))


def ints_to_str(ints):
	return ",".join(list(map(lambda x: str(x), ints)))


class IntcodeRunner:
	def _decode_opcode(self, value):
		op = value % 100
		value = value // 100
		
		argmode = []
		for i in range(3):
			argmode.append(value % 10)
			value = value // 10

		return op, argmode


	def _get_addr(self, position, mode):
		if mode == 0:
			return position
		elif mode == 2:
			return self.relbase + position
		else:
			print("What in tarnation", file=sys.stderr)
			exit(1)


	def _get_val(self, position, mode):
		if mode == 2:
			position += self.relbase
			mode = 0

		if mode == 0:
			maxpos = len(self.ints) - 1
			if position > maxpos:
				return 0
			else:
				return self.ints[position]
		elif mode == 1:
			return position
		else:
			print("What the fuck", file=sys.stderr)
			exit(1)


	def _assign_val(self, position, value):
		maxpos = len(self.ints) - 1
		if position > maxpos:
			diff = position - maxpos
			self.ints += diff * [0]

		self.ints[position] = value


	def run(self, input):
		output = []
		while True:
			op, argmode = self._decode_opcode(self.ints[self.pos])

			if op == 1:
				arg1 = self.ints[self.pos+1]
				arg2 = self.ints[self.pos+2]
				arg3 = self.ints[self.pos+3]
				self.pos += 4
				
				self._assign_val(self._get_addr(arg3, argmode[2]), self._get_val(arg1, argmode[0]) + self._get_val(arg2, argmode[1]))
			elif op == 2:
				arg1 = self.ints[self.pos+1]
				arg2 = self.ints[self.pos+2]
				arg3 = self.ints[self.pos+3]
				self.pos += 4
				
				self._assign_val(self._get_addr(arg3, argmode[2]), self._get_val(arg1, argmode[0]) * self._get_val(arg2, argmode[1]))
			elif op == 3:
				arg0 = self.ints[self.pos+1]
				self.pos += 2

				self._assign_val(self._get_addr(arg0, argmode[0]), input.pop(0))
			elif op == 4:
				arg0 = self.ints[self.pos+1]
				self.pos += 2

				output.append(self._get_val(arg0, argmode[0]))
			elif (op == 5) or (op == 6):
				arg1 = self.ints[self.pos+1]
				arg2 = self.ints[self.pos+2]
				
				val = self._get_val(arg1, argmode[0])
				if ((op == 5) and (val != 0)) or ((op == 6) and (val == 0)):
					self.pos = self._get_val(arg2, argmode[1])
				else:
					self.pos += 3
			elif (op == 7) or (op == 8):
				arg1 = self.ints[self.pos+1]
				arg2 = self.ints[self.pos+2]
				arg3 = self.ints[self.pos+3]
				self.pos += 4

				val1 = self._get_val(arg1, argmode[0])
				val2 = self._get_val(arg2, argmode[1])

				condition = ((op == 7) and (val1 < val2)) or ((op == 8) and (val1 == val2))
				self._assign_val(self._get_addr(arg3, argmode[2]), 1 if condition else 0)
			elif op == 9:
				arg1 = self.ints[self.pos+1]
				self.pos += 2

				self.relbase += self._get_val(arg1, argmode[0])
			elif op == 99:
				break
			else:
				print("Unknown opcode \"{}\"".format(self.ints[self.pos]), file=sys.stderr)
				exit(1)
		
		return output


	def __init__(self, program):
		self.ints = program.copy()
		self.relbase = 0
		self.pos = 0


if __name__ == "__main__":
	if (len(sys.argv) < 3) or (sys.argv[1] != "--input"):
		print("You must provide --input", file=sys.stderr)
		exit(1)
	
	input = str_to_ints(sys.argv[2])

	for line in sys.stdin:
		ir = IntcodeRunner(str_to_ints(line))
		output = ir.run(input.copy())
		print(ints_to_str(output))
