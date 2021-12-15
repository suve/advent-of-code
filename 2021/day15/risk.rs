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

fn calc_cost(map: &Map, scale: usize) -> usize {
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
	let dest = (map.size * (scale as isize)) - 1;
	loop {
		let current = match queue.pop() {
			Some(value) => value.0,
			None => panic!("Ran out of coordinates to check"),
		};

		if current.coords.x == dest && current.coords.y == dest {
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
			if next.x < 0 || next.x > dest || next.y < 0 || next.y > dest { 
				continue
			}

			let next_scaled = Coords {
				x: next.x % map.size,
				y: next.y % map.size,
			};
			let mut next_value = *(map.data.get(&next_scaled).unwrap()) as usize;
			next_value += (next.x / map.size) as usize;
			next_value += (next.y / map.size) as usize;
			while next_value >= 10 {
				next_value -= 9 // 10 is supposed to wrap back to 1, not 0
			}

			queue.push(Reverse(Candidate {
				cost: current.cost + next_value,
				coords: next,
				parent: current.coords
			}));
		}
	}
}

fn main() {
	let map = read_input();
	println!("Part1: {}", calc_cost(&map, 1));
	println!("Part2: {}", calc_cost(&map, 5));
}

