use std::convert::TryInto;
use std::io;
use std::io::BufRead as _;
use std::vec::Vec;

const SIZE: usize = 5;

struct Board {
	numbers: [usize; SIZE*SIZE],
	marked:  [bool; SIZE*SIZE],
}

impl Board {
	fn new(num: &[usize; SIZE*SIZE]) -> Self {
		return Self {
			numbers: *num,
			marked: [false; SIZE*SIZE],
		};
	}

	fn row_completed(&self, row: usize) -> bool {
		let mut count = 0;
		for x in 0..SIZE {
			if self.marked[row * SIZE + x] {
				count += 1;
			}
		}
		return count == SIZE;
	}

	fn col_completed(&self, col: usize) -> bool {
		let mut count = 0;
		for y in 0..SIZE {
			if self.marked[y * SIZE + col] {
				count += 1;
			}
		}
		return count == SIZE;
	}

	fn mark_number(&mut self, num: usize) -> bool {
		let mut complete = false;

		for i in 0..SIZE*SIZE {
			if self.numbers[i] != num {
				continue;
			}
			self.marked[i] = true;

			if complete {
				continue;
			}
			let row = i / SIZE;
			if self.row_completed(row) {
				complete = true;
			} else {
				let col = i % SIZE;
				complete = self.col_completed(col);
			}
		}
		return complete;
	}

	fn get_score(&self, multiplier: usize) -> usize {
		return multiplier * self.numbers.iter()
			.enumerate()
			.filter(|(i, _)| !self.marked[*i])
			.fold(0, |acc, (_, val)| acc + val);
	}
}

fn str_to_vec(s: &str, delim: &str) -> Vec<usize> {
	s.split(delim)
		.filter(|x| x.len() > 0)
		.map(|x| x.parse::<usize>().unwrap())
		.collect::<Vec<usize>>()
}

fn main() {
	let mut lines = io::BufReader::new(io::stdin()).lines();
	let draw = str_to_vec(&lines.next().unwrap().unwrap(), ",");

	let mut boards = vec![];
	loop {
		// Skip empty line before each board
		if lines.next().is_none() {
			break;
		}

		let mut numbers = vec![];
		for _ in 0..SIZE {
			let mut nums = str_to_vec(&lines.next().unwrap().unwrap(), " ");
			if nums.len() != SIZE {
				panic!("Wrong number of numbers in row: expected {}, got {}", SIZE, nums.len());
			}
			numbers.append(&mut nums);
		}
		boards.push(Board::new(&numbers[0..SIZE*SIZE].try_into().unwrap()));
	}

	let mut completed = vec![false; boards.len()];
	let mut compl_count = 0;
	for n in draw {
		eprintln!("Marking '{}'...", n);
		for (i, b) in boards.iter_mut().enumerate() {
			if !completed[i] && b.mark_number(n) {
				eprintln!("Board #{} is complete! Board score: {}", (i+1), b.get_score(n));
				completed[i] = true;
				compl_count += 1;
			}
		}
		if compl_count == boards.len() {
			break;
		}
	}
}
