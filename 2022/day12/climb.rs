use std::collections::{BinaryHeap, HashMap};
use std::io;
use std::io::BufRead as _;

#[derive(Copy, Clone, Eq, Hash, PartialEq, PartialOrd, Ord)]
struct Coords {
	x: isize,
	y: isize,
}

struct Map {
	width: isize,
	height: isize,
	data: HashMap<Coords, u8>,
}

fn read_input() -> (Coords, Coords, Map, Vec<Coords>) {
	let mut w: isize = 0;
	let mut h: isize = 0;
	let mut data = HashMap::<Coords, u8>::new();

	let mut start: Option<Coords> = None;
	let mut end: Option<Coords> = None;
	let mut zeroes = vec![];
	for line in io::BufReader::new(io::stdin()).lines() {
		let line = line.unwrap();
		let trimmed = line.trim();

		if w != 0 {
			if (trimmed.len() as isize) != w {
				panic!("Error: line {} is {} characters long, expected {}", (h+1), trimmed.len(), w);
			}
		} else {
			w = (trimmed.len() as isize)
		}

		for (i, b) in trimmed.bytes().enumerate() {
			let position = Coords{ x: i as isize, y: h };
			let tile = match b {
				83 => {
					start = Some(position);
					0
				},
				69 => {
					end = Some(position);
					25
				},
				_ => (b - 97)
			};
			if tile == 0 {
				zeroes.push(position);
			}
			data.insert(position, tile);
		}
		h += 1;
	}

	if start.is_none() {
		panic!("Error: No starting point found in input");
	}
	if end.is_none() {
		panic!("Error: No ending point found in input");
	}

	let map = Map {
		data: data,
		width: w,
		height: h,
	};
	return (start.unwrap(), end.unwrap(), map, zeroes);
}

fn find_path(map: &Map, start: Coords, end: Coords) -> Option<usize> {
	use std::cmp::Reverse;

	#[derive(Eq, Ord, PartialEq, PartialOrd)]
	struct Candidate {
		cost: usize,
		coords: Coords,
		parent: Coords,
	}
	let mut queue = BinaryHeap::<Reverse::<Candidate>>::new();
	queue.push(Reverse(Candidate {
		cost: 0,
		coords: start,
		parent: start,
	}));

	let mut parent = HashMap::<Coords, Coords>::new();
	loop {
		let current = match queue.pop() {
			Some(value) => value.0,
			None => return None,
		};

		if current.coords == end {
			return Some(current.cost);
		}
		if parent.contains_key(&current.coords) {
			continue
		}
		parent.insert(current.coords, current.parent);

		let current_value = *(map.data.get(&current.coords).unwrap()) as usize;

		let possibilities = vec![
			Coords { x: current.coords.x + 1, y: current.coords.y },
			Coords { x: current.coords.x, y: current.coords.y + 1 },
			Coords { x: current.coords.x - 1, y: current.coords.y },
			Coords { x: current.coords.x, y: current.coords.y - 1 },
		];
		for next in possibilities {
			if next.x < 0 || next.x >= map.width || next.y < 0 || next.y >= map.height {
				continue
			}

			let next_value = *(map.data.get(&next).unwrap()) as usize;
			if next_value > (current_value + 1) {
				continue
			}

			queue.push(Reverse(Candidate {
				cost: current.cost + 1,
				coords: next,
				parent: current.coords
			}));
		}
	}
}

fn main() {
	let (start, end, map, zeroes) = read_input();
	println!("Part 1: {}", find_path(&map, start, end).unwrap());
	println!("Part 2: {}", zeroes.iter()
		.map(|zero| { find_path(&map, *zero, end) })
		.filter(|i| { i.is_some() })
		.map(|i| { i.unwrap() })
		.reduce(|a, i| { if i < a { i } else { a }})
		.unwrap()
	);
}

