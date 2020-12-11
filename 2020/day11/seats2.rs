use std::io;
use std::io::BufRead as _;
use std::vec::Vec;

#[derive(Clone, Copy, Eq, PartialEq)]
enum Tile {
	Floor,
	Empty,
	Taken,
	OutOfBounds,
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
		return Tile::OutOfBounds;
	}

	let row = match map.get(y as usize) {
		Some(v) => v,
		None => return Tile::OutOfBounds,
	};
	let tile = match row.get(x as usize) {
		Some(v) => v,
		None => return Tile::OutOfBounds,
	};
	return *tile;
}


struct Coords {
	pub x: usize,
	pub y: usize,
}

type Visibility = [Option<Coords>; 8];
type VisibilityMap = Vec<Vec<Visibility>>;

fn calc_visible_coords(map: &Map, start_x: isize, start_y: isize, step_x: isize, step_y: isize) -> Option<Coords> {
	let mut x = start_x;
	let mut y = start_y;
	loop {
		x += step_x;
		y += step_y;

		match get_tile(map, x, y) {
			Tile::Empty | Tile::Taken => return Some(Coords { x: x as usize, y: y as usize }),
			Tile::OutOfBounds => return None,
			_ => {},
		}
	}
}

fn calculate_visibility_map(map: &Map) -> VisibilityMap {
	let mut vmap: VisibilityMap = vec![];

	let rows = map.len() as isize;
	let cols = map[0].len() as isize;

	let mut changes: usize = 0;
	for y in 0..rows {
		let mut row: Vec<Visibility> = vec![];
		for x in 0..cols {
			let v: Visibility = [
				// rustc doesn't allow to use unary + to write "+1"
				// just... why
				// I demand to see rust's manager!
				calc_visible_coords(map, x, y, -1, -1),
				calc_visible_coords(map, x, y,  0, -1),
				calc_visible_coords(map, x, y,  1, -1),
				calc_visible_coords(map, x, y, -1,  0),
				calc_visible_coords(map, x, y,  1,  0),
				calc_visible_coords(map, x, y, -1,  1),
				calc_visible_coords(map, x, y,  0,  1),
				calc_visible_coords(map, x, y,  1,  1),
			];
			row.push(v);
		}
		vmap.push(row);
	}
	return vmap;
}

fn step(old: &Map, new: &mut Map, vis: &VisibilityMap) -> usize {
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
			for n in 0..8 {
				match &vis[uy][ux][n] {
					Some(crd) => {
						if old[crd.y][crd.x] == Tile::Taken {
							neighbours += 1;
						}
					}
					None => {},
				}
			}

			if current == Tile::Empty {
				if neighbours == 0 {
					new[uy][ux] = Tile::Taken;
					changes += 1;
				}
			} else {
				if neighbours >= 5 {
					new[uy][ux] = Tile::Empty;
					changes += 1;
				}
			}
		}
	}
	return changes;
}

fn calc_taken_seats(map: &Map) -> usize {
	map.iter().fold(0, |acc, row| {
		acc + row.iter().fold(0, |acc, tile| {
			acc + if *tile == Tile::Taken { 1 } else { 0 }
		})
	})
}

fn main() {
	let mut map = parse_input(io::stdin());
	let vis = calculate_visibility_map(&map);
	loop {
		let mut new_map = map.clone();
		let changes = step(&map, &mut new_map, &vis);
		if changes == 0 { 
			break
		} else {
			println!("--> {} tiles changed", changes);
		}

		map = new_map;
	}
	println!("{}", calc_taken_seats(&map));
}
