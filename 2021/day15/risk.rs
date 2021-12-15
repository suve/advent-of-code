use std::collections::{BinaryHeap, HashMap};
use std::io;
use std::io::BufRead as _;

#[derive(Copy, Clone, Eq, Hash, PartialEq, PartialOrd, Ord)]
struct Coords {
	x: isize,
	y: isize,
}

struct Map {
	size: isize,
	data: HashMap<Coords, u8>,
}

fn read_input() -> Map {
	let mut w: usize = 0;
	let mut h: usize = 0;
	let mut data = HashMap::<Coords, u8>::new();

	for line in io::BufReader::new(io::stdin()).lines() {
		let line = line.unwrap();
		let trimmed = line.trim();
	
		if w != 0 {
			if trimmed.len() != w {
				panic!("Error: line {} is {} characters long, expected {}", (h+1), trimmed.len(), w);
			}
		} else {
			w = trimmed.len();
		}

		for (i, b) in trimmed.bytes().enumerate() {
			data.insert(Coords{ x: i as isize, y: h as isize }, b - 48);
		}
		h += 1;
	}

	if h != w {
		panic!("Error: input map should be a square, but got {} rows vs {} columns\n", h, w);
	}

	return Map {
		data: data,
		size: w as isize,
	};
}

fn find_route(map: &Map) -> usize {
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
		coords: Coords { x: 0, y: 0 },
		parent: Coords { x: 0, y: 0 },
	}));

	let mut parent = HashMap::<Coords, Coords>::new();
	loop {
		let current = match queue.pop() {
			Some(value) => value.0,
			None => panic!("Ran out of coordinates to check"),
		};

		if current.coords.x == (map.size - 1) && current.coords.y == (map.size - 1) {
			return current.cost;
		}
		if parent.contains_key(&current.coords) {
			continue
		}
		parent.insert(current.coords, current.parent);

		let possibilities = vec![
			Coords { x: current.coords.x + 1, y: current.coords.y },
			Coords { x: current.coords.x, y: current.coords.y + 1 },
			Coords { x: current.coords.x - 1, y: current.coords.y },
			Coords { x: current.coords.x, y: current.coords.y - 1 },
		];
		for next in possibilities {
			let value = match map.data.get(&next) {
				Some(valref) => *valref,
				None => continue,
			};

			queue.push(Reverse(Candidate {
				cost: current.cost + (value as usize),
				coords: next,
				parent: current.coords
			}));
		}
	}
}

fn main() {
	let map = read_input();
	println!("Part1: {}", find_route(&map));
}

