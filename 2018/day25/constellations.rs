use std::io;
use std::io::Write;
use std::process;
use std::vec;

const MAX_DISTANCE: i32 = 3;

#[derive(Copy, Clone)]
struct Coords {
	x: i32,
	y: i32,
	z: i32,
	w: i32,
}

impl Coords {
	fn distance_to(&self, other: &Self) -> i32 {
		let xd = (self.x - other.x).abs();
		let yd = (self.y - other.y).abs();
		let zd = (self.z - other.z).abs();
		let wd = (self.w - other.w).abs();
		return xd + yd + zd + wd;
	}

	fn from_string(string: &str) -> Self {
		let mut values: [i32; 4] = [0; 4];

		let mut iter = string.trim().split(',');
		for v in 0..4 {
			let next = iter.next();
			if next.is_none() {
				eprintln!("Expected {} values in line, but only got {}", 4, v);
				process::exit(1);
			}

			let parse = next.unwrap().parse();
			if parse.is_err() {
				eprintln!("Failed to parse integer value: {}", parse.unwrap_err());
				process::exit(1);
			}

			values[v] = parse.unwrap();
		}

		return Coords{ x: values[0], y: values[1], z: values[2], w: values[3] };
	}
}

struct Constellation {
	data: Vec<Coords>,
}

impl Constellation {
	fn add(&mut self, new: Coords) {
		self.data.push(new);
	}

	fn can_add(&self, candidate: &Coords) -> bool {
		match self.data.iter().find(|elem| elem.distance_to(candidate) <= MAX_DISTANCE) {
			Some(_) => return true,
			None => return false,
		}
	}

	fn can_join(&self, other: &Constellation) -> bool {
		for our in self.data.iter() {
			for theirs in other.data.iter() {
				if our.distance_to(&theirs) <= MAX_DISTANCE {
					return true;
				}
			}
		}
		return false;
	}

	fn join(&mut self, other: &Constellation) {
		self.data.reserve(other.data.len());
		self.data.extend(other.data.iter());
	}

	fn len(&self) -> usize {
		return self.data.len();
	}

	fn new() -> Self {
		let vector: Vec<Coords> = Vec::new();
		return Constellation{ data: vector };
	}

	fn new_from_coords(origin: Coords) -> Self { 
		let mut vector: Vec<Coords> = Vec::new();
		vector.push(origin);

		return Constellation{ data: vector };
	}
}

fn read_stdin() -> Vec<Constellation> {
	let mut result: Vec<Constellation> = Vec::new();

	let mut line = String::new();
	loop {
		line.clear();
		match io::stdin().read_line(&mut line) {
			Ok(bytes) => {
				if bytes == 0 {
					break;
				}
				result.push(Constellation::new_from_coords(Coords::from_string(&line)));
			}
			Err(err) => {
				eprintln!("An error occurred while reading from stdin: {}", err);
				process::exit(1);
			}
		}
	}

	return result;
}

fn main() { 
	let mut cons = read_stdin();

	'join: loop {
		let len = cons.len();
		for i in 0..(len-1) {
			for j in (i+1)..len {
				if cons[i].can_join(&cons[j]) {
					eprintln!("Joined constellation [{}]({}) and [{}]({})", i, cons[i].len(), j, cons[j].len());

					let removed = cons.remove(j);
					cons[i].join(&removed);
					continue 'join;
				}
			}
		}

		break;
	}
	println!("Total number of constellations: {}", cons.len());
}
