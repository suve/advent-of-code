use std::io;
use std::io::BufRead as _;
use std::vec::Vec;

enum Opcode {
	nop,
	acc,
	jmp,
}

struct Instruction {
	pub op: Opcode,
	pub no: isize,
}

impl Instruction {
	pub fn new(line: &str) -> Self {
		let mut split = line.split(' ');
		let opcode = match split.next().unwrap() {
			"nop" => Opcode::nop,
			"acc" => Opcode::acc,
			"jmp" => Opcode::jmp,
			_ => panic!("Unknown opcode"),
		};
		let arg: isize = split.next().unwrap().parse().unwrap();

		return Self {
			op: opcode,
			no: arg,
		};
	}
}

struct Console {
	instructions: Vec<Instruction>,
	iptr: isize,
	accu: isize,
}

impl Console {
	pub fn new<T>(reader: T) -> Self
		where T: io::Read
	{
		let mut instructions: Vec<Instruction> = vec![];

		for line in io::BufReader::new(reader).lines() {
			instructions.push(Instruction::new(&line.unwrap()));
		}

		return Console {
			instructions,
			iptr: 0,
			accu: 0,
		};
	}

	pub fn run(&mut self) {
		let max = (self.instructions.len() as isize)- 1;
		
		let mut executed: Vec<bool> = vec![];
		for i in 0..=max { executed.push(false); }

		while (self.iptr >= 0) && (self.iptr <= max) {
			if executed[self.iptr as usize] {
				println!("Visited instruction #{} again, accumulator is {}", self.iptr, self.accu);
				return;
			} else {
				executed[self.iptr as usize] = true;
			}

			let i = &self.instructions[self.iptr as usize];
			match i.op {
				Opcode::nop => self.op_nop(i.no),
				Opcode::acc => self.op_acc(i.no),
				Opcode::jmp => self.op_jmp(i.no),
			}
			self.iptr += 1;
		}
		println!("Program terminated (iptr: {}), accumulator is {}", self.iptr, self.accu); 
	}

	fn op_nop(&mut self, _arg: isize) { }

	fn op_acc(&mut self, arg: isize) {
		self.accu += arg;
	}

	fn op_jmp(&mut self, arg: isize) {
		self.iptr += arg - 1;
	}
}

fn main() {
	Console::new(io::stdin()).run();
}
