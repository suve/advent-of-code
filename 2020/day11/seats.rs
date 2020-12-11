use std::io;
use std::io::BufRead as _;
use std::vec::Vec;

#[derive(Clone, Copy, Eq, PartialEq)]
enum Tile {
	Floor,
	Empty,
	Taken,
}

type Map = Vec<Vec<Tile>>;

fn parse_input<T>(reader: T) -> Map 
	where T: io::Read
{
	let mut map: Map = vec![];

	for line in io::BufReader::new(reader).lines() {
		let line = line.unwrap();
		let mut row: Vec<Tile> = Vec::with_capacity(line.len());
		for c in line.chars() {
			let tile = match c {
				'.' => Tile::Floor,
				'L' => Tile::Empty,
				'#' => Tile::Taken,
				_ => panic!("Unrecognized input (\"{}\")", c)
			};
			row.push(tile);
		}
		map.push(row);
	}

	let width = map[0].len();
	for i in 1..map.len() {
		let ilen = map[i].len();
		if ilen != width {
			panic!("Mismatch in row length: {} vs {}", ilen, width);
		}
	}
	return map;
}

fn get_tile(map: &Map, x: isize, y: isize) -> Tile {
	if y < 0 || x < 0 {
		return Tile::Floor;
	}

	let row = match map.get(y as usize) {
		Some(v) => v,
		None => return Tile::Floor,
	};
	let tile = match row.get(x as usize) {
		Some(v) => v,
		None => return Tile::Floor,
	};
	return *tile;
}

fn step(old: &Map, new: &mut Map) -> usize {
	let rows = new.len() as isize;
	let cols = new[0].len() as isize;

	let mut changes: usize = 0;
	for y in 0..rows {
		let uy = y as usize;
		for x in 0..cols {
			let ux = x as usize;

			let current = old[uy][ux];
			if current == Tile::Floor { continue }

			let mut neighbours = 0;
			neighbours += if get_tile(old, x-1, y-1) == Tile::Taken { 1 } else { 0 };
			neighbours += if get_tile(old, x  , y-1) == Tile::Taken { 1 } else { 0 };
			neighbours += if get_tile(old, x+1, y-1) == Tile::Taken { 1 } else { 0 };
			neighbours += if get_tile(old, x-1, y  ) == Tile::Taken { 1 } else { 0 };
			neighbours += if get_tile(old, x+1, y  ) == Tile::Taken { 1 } else { 0 };
			neighbours += if get_tile(old, x-1, y+1) == Tile::Taken { 1 } else { 0 };
			neighbours += if get_tile(old, x  , y+1) == Tile::Taken { 1 } else { 0 };
			neighbours += if get_tile(old, x+1, y+1) == Tile::Taken { 1 } else { 0 };

			if current == Tile::Empty {
				if neighbours == 0 {
					new[uy][ux] = Tile::Taken;
					changes += 1;
				}
			} else {
				if neighbours >= 4 {
					new[uy][ux] = Tile::Empty;
					changes += 1;
				}
			}
		}
	}
	return changes;
}

fn calc_taken_seats(map: &Map) -> usize {
	return map.iter().fold(0, |acc, row| {
		acc + row.iter().fold(0, |acc, tile| {
			acc + if *tile == Tile::Taken { 1 } else { 0 }
		})
	});
}

fn main() {
	let mut map = parse_input(io::stdin());
	loop {
		let mut new_map = map.clone();
		let changes = step(&map, &mut new_map);
		if changes == 0 { 
			break
		} else {
			println!("--> {} tiles changed", changes);
		}

		map = new_map;
	}
	println!("{}", calc_taken_seats(&map));
}
