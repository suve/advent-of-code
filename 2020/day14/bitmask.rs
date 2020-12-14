use std::collections::HashMap;
use std::io;
use std::io::BufRead as _;

struct Mask {
	pub and: u64,
	pub or: u64,
}

impl Mask {
	fn new(text: &str) -> Self {
		let and = u64::from_str_radix(&text.replace("X", "1"), 2).unwrap();
		let or = u64::from_str_radix(&text.replace("X", "0"), 2).unwrap();
		return Self { and, or };
	}

	fn apply(&self, number: u64) -> u64 {
		(number & self.and) | self.or
	}
}

fn main() {
	let mut mem = HashMap::<u64,u64>::new();
	let mut mask = Mask::new("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");

	for line in io::BufReader::new(io::stdin()).lines() {
		let line = line.unwrap();
		if line.starts_with("mask = ") {
			mask = Mask::new(line.split("mask = ").nth(1).unwrap());
		} else if line.starts_with("mem[") {
			let mut split = line.strip_prefix("mem[").unwrap().split("] = ");
			let address = split.next().unwrap().parse::<u64>().unwrap();
			let value = split.next().unwrap().parse::<u64>().unwrap();

			mem.insert(address, mask.apply(value));
		}
	}

	let mut sum: u64 = 0;
	for (_address, value) in mem {
		sum += value;
	}
	println!("{}", sum);
}
