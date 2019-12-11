#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys


def str_to_ints(string):
	return list(map(lambda x: int(x), string.split(',')))


def ints_to_str(ints):
	return ",".join(list(map(lambda x: str(x), ints)))


class BoardRow:
	def get(self, x):
		if(x < self.start): return 0
		
		x -= self.start
		if(x >= len(self.data)): return 0

		return self.data[x]

	def set(self, x, value):
		if(x < self.start):
			expand = self.start - x
			self.data = (expand * [0]) + self.data
			self.start = x

		x -= self.start
		if(x >= len(self.data)):
			expand = (x - len(self.data)) + 1
			self.data = self.data + (expand * [0])

		self.data[x] = value

	def __str__(self):
		return "".join(map(lambda v: '#' if v == 1 else '.', self.data))

	def __init__(self):
		self.start = 0
		self.data = []

class Board:
	def get(self, x, y):
		if(y < self.start): return 0
		
		y -= self.start
		if(y >= len(self.data)): return 0

		return self.data[y].get(x)

	def set(self, x, y, value):
		if(y < self.start):
			expand = self.start - y
			for _ in range(expand):
				row = BoardRow()
				self.data = [row] + self.data
			self.start = y

		y -= self.start
		if(y >= len(self.data)):
			expand = (y - len(self.data)) + 1
			for _ in range(expand):
				row = BoardRow()
				self.data += [row]

		if(x < self.minX): self.minX = x
		elif(x > self.maxX): self.maxX = x

		self.data[y].set(x, value)

	def __str__(self):
		rows = []
		for i in range(len(self.data)):
			row = str(self.data[i])
			
			imin = self.data[i].start
			imax = imin + (len(row) - 1)

			if self.minX < imin:
				row = ((imin - self.minX) * ".") + row
			if self.maxX > imax:
				row = row + ((self.maxX - imax) * ".")
			rows.append(row)

		return "\n".join(rows)


	def __init__(self):
		self.minX = 0
		self.maxX = 0
		self.start = 0
		self.data = []


class BoardWalker:
	def turn_left(self):
		if self.dir == 0:
			self.dir = 3
		else:
			self.dir -= 1

	def turn_right(self):
		self.dir = (self.dir + 1) % 4

	def move(self):
		if self.dir == 0:
			self.y -= 1
		elif self.dir == 1:
			self.x += 1
		elif self.dir == 2:
			self.y += 1
		elif self.dir == 3:
			self.x -= 1

	def paint(self, colour):
		coords = "{}:{}".format(self.x, self.y)
		self.history.append(coords)

		self.board.set(self.x, self.y, colour)
	
	def peek(self):
		return self.board.get(self.x, self.y)

	def get_paints(self):
		return len(self.history)

	def get_unique_paints(self):
		return len(list(set(self.history)))

	def __init__(self, board):
		self.board = board
		self.x = 0
		self.y = 0
		self.dir = 0
		self.history = []


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


	def run(self):
		output = []
		moveWalker = False
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

				self._assign_val(self._get_addr(arg0, argmode[0]), self.walker.peek())
			elif op == 4:
				arg0 = self.ints[self.pos+1]
				self.pos += 2

				value = self._get_val(arg0, argmode[0])
				if moveWalker:
					if value == 0:
						self.walker.turn_left()
					else:
						self.walker.turn_right()
					self.walker.move()
				else:
					self.walker.paint(value)
				moveWalker = (moveWalker == False)
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
		
		return True


	def __init__(self, program, walker):
		self.ints = program
		self.walker = walker
		self.relbase = 0
		self.pos = 0


if __name__ == "__main__":
	mode = 0
	if(len(sys.argv) < 2):
		print("Please specify --part1 or --part2", file=sys.stderr)
		exit(1)
	
	if(sys.argv[1] == "--part1"):
		mode = 1
	elif(sys.argv[1] == "--part2"):
		mode = 2
	else:
		print("Please specify --part1 or --part2", file=sys.stderr)
		exit(1)

	for line in sys.stdin:
		board = Board()
		if mode == 2: board.set(0, 0, 1)

		walker = BoardWalker(board)

		ir = IntcodeRunner(str_to_ints(line), walker)
		ir.run()

		print("Total paints: {}; Unique paints: {}".format(walker.get_paints(), walker.get_unique_paints()))
		print(board)
