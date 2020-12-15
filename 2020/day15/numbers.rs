#![feature(new_uninit)]

use std::boxed::Box;
use std::env::args;
use std::io;
use std::io::Read as _;
use std::vec::Vec;

const MEMSIZE: usize = 32 * 1024*1024;

fn parse_args() -> usize {
	args()
		.nth(1)
		.expect("You must provide the number of iterations")
		.parse::<usize>()
		.expect("Failed to parse number")
}

fn parse_input() -> Vec<usize> {
	let mut text = String::from("");
	io::stdin().lock().read_to_string(&mut text).expect("Failed to parse stdin");
	return text.trim().split(",").map(|x| x.parse::<usize>().unwrap()).collect();
}

fn main() {
	let limit = parse_args();
	let start_nums = parse_input();
	
	let mem = Box::<[usize; MEMSIZE]>::new_zeroed();
	let mut mem = unsafe { mem.assume_init() };

	for (i, n) in start_nums.iter().enumerate() {
		mem[*n] = i+1;
	}

	// lil' bit of cheating: the numbers don't repeat in the input data,
	// so the first number after the starting numbers will always be a 0
	let mut next: usize = 0;
	
	let mut turn_no = start_nums.len() + 1;
	while turn_no < limit {
		let current = next;
		if mem[current] == 0 {
			next = 0;
		} else {
			next = turn_no - mem[current];
		}
		mem[current] = turn_no;
		turn_no += 1;
	}

	println!("{}", next);
}
