use std::cmp::{Ord, Ordering};
use std::collections::HashMap;
use std::io;
use std::io::BufRead as _;
use std::vec::Vec;

#[derive(Clone,Copy,PartialEq)]
enum Colour {
	White,
	Black
}

impl Colour {
	fn flip(&self) -> Self {
		match self {
			Colour::White => Colour::Black,
			Colour::Black => Colour::White,
		}
	}
}

#[derive(Clone,Eq,Hash,PartialEq)]
struct Coords {
	pub x: isize,
	pub y: isize,
}

impl Ord for Coords {
	fn cmp(&self, other: &Self) -> Ordering {
		if self.y < other.y {
			return Ordering::Less;
		}
		if self.y > other.y {
			return Ordering::Greater;
		}

		if self.x < other.x {
			return Ordering::Less;
		}
		if self.x > other.x {
			return Ordering::Greater;
		}

		return Ordering::Equal;
	}
}

impl PartialOrd for Coords {
	fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
		Some(self.cmp(other))
	}
}

impl Coords {
	fn get_nw(&self) -> Self {
		return Self {
			x: self.x - (self.y % 2).abs(),
			y: self.y - 1,
		}
	}
	
	fn get_ne(&self) -> Self {
		return Self {
			x: self.x + 1 - (self.y % 2).abs(),
			y: self.y - 1,
		}
	}

	fn get_w(&self) -> Self {
		return Self {
			x: self.x - 1,
			y: self.y,
		}
	}

	fn get_e(&self) -> Self {
		return Self {
			x: self.x + 1,
			y: self.y, 
		}
	}

	fn get_sw(&self) -> Self {
		return Self {
			x: self.x - (self.y % 2).abs(),
			y: self.y + 1,
		}
	}
	
	fn get_se(&self) -> Self {
		return Self {
			x: self.x + 1 - (self.y % 2).abs(),
			y: self.y + 1,
		}
	}

	fn get_dir(&self, dir: &Direction) -> Self {
		match dir {
			Direction::NW => self.get_nw(),
			Direction::NE => self.get_ne(),
			Direction::W => self.get_w(),
			Direction::E => self.get_e(),
			Direction::SW => self.get_sw(),
			Direction::SE => self.get_se(),
		}
	}
}

enum Direction {
	NW,
	NE,
	W,
	E,
	SW,
	SE,
}

fn tokenize(line: &str) -> Vec<Direction> {
	let mut result = Vec::<Direction>::with_capacity(line.len());

	let mut chars = line.chars();
	loop {
		let dir = match chars.next() {
			Some(value) => match value {
				'n' => match chars.next().unwrap() {
					'w' => Direction::NW,
					'e' => Direction::NE,
					_ => panic!("Unrecognized character"),
				},
				's' => match chars.next().unwrap() {
					'w' => Direction::SW,
					'e' => Direction::SE,
					_ => panic!("Unrecognized character"),
				},
				'w' => Direction::W,
				'e' => Direction::E,
				_ => panic!("Unrecognized character"),
			},
			None => return result,
		};
		result.push(dir);
	}
}

type TileMap = HashMap<Coords, Colour>;

fn count_black_tiles(tiles: &TileMap) -> usize { 
	tiles.iter().filter(|(_coords, colour)| **colour == Colour::Black).count()
}

fn count_black_neighbours(tiles: &TileMap, position: &Coords) -> usize {
	let mut count = 0;

	let coords_vec: Vec<Coords> = vec![
		position.get_nw(),
		position.get_ne(),
		position.get_w(),
		position.get_e(),
		position.get_sw(),
		position.get_se(),
	];
	for coords in coords_vec {
		count += match tiles.get(&coords) {
			Some(value) => match value {
				Colour::Black => 1,
				Colour::White => 0,
			},
			None => 0,
		};
	}
	return count;
}

fn daily_flip(tiles: &TileMap) -> TileMap {
	// This is a very wasteful approach, but eh, whatever
	let mut coords_vec = Vec::<Coords>::with_capacity(tiles.len() * 7);
	for (coords, _colour) in tiles {
		coords_vec.push(coords.clone());
		coords_vec.push(coords.get_nw());
		coords_vec.push(coords.get_ne());
		coords_vec.push(coords.get_w());
		coords_vec.push(coords.get_e());
		coords_vec.push(coords.get_sw());
		coords_vec.push(coords.get_se());
	}
	coords_vec.sort();
	coords_vec.dedup();

	let mut result = TileMap::new();
	for coords in coords_vec {
		let (present, oldColour) = match tiles.get(&coords) {
			Some(value) => (true, *value),
			None => (false, Colour::White),
		};

		let neighbours = count_black_neighbours(tiles, &coords);
		let newColour = match oldColour {
			Colour::White => match neighbours {
				2 => Colour::Black,
				_ => Colour::White,
			},
			Colour::Black => match neighbours{
				1 | 2 => Colour::Black,
				_ => Colour::White,
			},
		};

		if newColour == Colour::Black {
			result.insert(coords, Colour::Black);
		}
	}
	return result;
}

fn main() {
	let mut tiles = TileMap::new();
	for line in io::BufReader::new(io::stdin()).lines() {
		let mut pos = Coords { x: 0, y: 0 };
		for dir in tokenize(&line.unwrap()) {
			pos = pos.get_dir(&dir);
		}

		let newColour = match tiles.get(&pos) {
			Some(colour) => colour.flip(),
			None => Colour::Black,		
		};
		eprintln!("--> {:+3} : {:+3} - {}", pos.x, pos.y, if newColour == Colour::White { "white" } else { "black" });
		tiles.insert(pos, newColour);
	}

	println!("Part1: {}", count_black_tiles(&tiles));

	for i in 1..=99 {
		tiles = daily_flip(&tiles);
		if (i < 10) || (i % 10 == 0) {
			println!("--> Day {}: {}", i, count_black_tiles(&tiles));
		}
	}

	tiles = daily_flip(&tiles);
	println!("Part2: {}", count_black_tiles(&tiles));
}
