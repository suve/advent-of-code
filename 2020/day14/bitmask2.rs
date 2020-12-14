use std::collections::HashMap;
use std::io;
use std::io::BufRead as _;
use std::vec::Vec;

const NUM_BITS: usize = 36;

struct Mask {
	ones: u64,
	x: Vec<usize>,
}

impl Mask {
	fn new(text: &str) -> Self {
		if text.len() != 36 {
			panic!("Invalid string length");
		}

		let ones = u64::from_str_radix(&text.replace("X", "0"), 2).unwrap();
		
		let mut x: Vec<usize> = vec![];
		let mut chars = text.chars();
		for i in (0..NUM_BITS).rev() {
			if chars.next().unwrap() == 'X' {
				x.push(i);
			}
		}

		return Self { ones, x };
	}
	
	fn apply(&self, number: u64) -> Vec<u64> {
		let mut result: Vec<u64> = vec![];
		result.push(number | self.ones);

		for i in &self.x {
			let mut new_result: Vec<u64> = vec![];
			for n in result {
				let bitmask = (1 as u64) << i;
				new_result.push(n | bitmask);
				new_result.push(n & !bitmask);
			}
			result = new_result;
		}

		return result;
	}
}

fn main() {
	let mut mem = HashMap::<u64,u64>::new();
	let mut mask = Mask::new("000000000000000000000000000000000000");

	for line in io::BufReader::new(io::stdin()).lines() {
		let line = line.unwrap();
		if line.starts_with("mask = ") {
			mask = Mask::new(line.split("mask = ").nth(1).unwrap());
		} else if line.starts_with("mem[") {
			let mut split = line.strip_prefix("mem[").unwrap().split("] = ");
			let address = split.next().unwrap().parse::<u64>().unwrap();
			let value = split.next().unwrap().parse::<u64>().unwrap();

			for a in mask.apply(address) {
				mem.insert(a, value);
			}
		}
	}

	let mut sum: u64 = 0;
	for (_address, value) in mem {
		sum += value;
	}
	println!("{}", sum);
}
