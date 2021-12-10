use std::io;
use std::io::BufRead as _;


fn check_match(x: char, stack: &Vec<char>) -> bool {
	let y = match stack.last() {
		Some(value) => *value,
		None => return false
	};
	match x {
		')' => return y == '(',
		']' => return y == '[',
		'}' => return y == '{',
		'>' => return y == '<',
		_ => panic!("ass!"),
	}
}

fn main() {
	let mut p1 = 0;
	let mut p2vec = vec![];
	for line in io::BufReader::new(io::stdin()).lines() {
		let mut stack: Vec<char> = vec![];
		let mut line_corrupted = false;
		for c in line.unwrap().chars() {
			match c {
				'(' | '[' | '{' | '<' => { stack.push(c); },
				')' | ']' | '}' | '>' => {
					if check_match(c, &stack) {
						stack.pop();
					} else {
						p1 += match c {
							')' => 3,
							']' => 57,
							'}' => 1197,
							'>' => 25137,
							_ => panic!("crap!")
						};
						line_corrupted = true;
						break;	
					}
				},
				_ => panic!("shite!"),
			}
		}

		// A line can be either corrupted or incomplete, but not both
		if line_corrupted { 
			continue;
		}

		let mut p2: usize = 0;
		while let Some(c) = stack.pop() {
			p2 *= 5;
			p2 += match c {
				'(' => 1,
				'[' => 2,
				'{' => 3,
				'<' => 4,
				_ => panic!("shitnugget!"),
			}
		}
		p2vec.push(p2);
	}

	println!("Part1: {}", p1);

	p2vec.sort();
	println!("Part2: {}", p2vec[p2vec.len() / 2]);
}
