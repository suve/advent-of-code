use std::collections::HashMap;
use std::io;
use std::io::Read as _;

#[derive(Clone,Copy,Eq,PartialEq)]
enum Status {
	Active,
	Dead,
}

#[derive(Clone,Eq,PartialEq,Hash)]
struct Coords {
	pub x: isize,
	pub y: isize,
	pub z: isize,
}

#[derive(Clone)]
struct BoundRange {
	pub min: isize,
	pub max: isize,
}

impl BoundRange {
	fn enlarge(&self) -> Self {
		return Self {
			min: self.min - 1,
			max: self.max + 1,
		};
	}
}

#[derive(Clone)]
struct Bounds {
	pub x: BoundRange,
	pub y: BoundRange,
	pub z: BoundRange,
}

impl Bounds {
	fn enlarge(&self) -> Self {
		return Self {
			x: self.x.enlarge(),
			y: self.y.enlarge(),
			z: self.z.enlarge(),
		}
	}
}

#[derive(Clone)]
struct Map {
	pub cubes: HashMap<Coords, Status>,
	pub bounds: Bounds,
}

impl Map {
	fn get_status(&self, coords: &Coords) -> Status {
		match self.cubes.get(coords) {
			Some(cube) => *cube,
			None => Status::Dead,
		}
	}

	fn count_active(&self) -> usize {
		self.cubes.iter().fold(0, |accumulator, (_coords, status)| { 
			accumulator + (if *status == Status::Active { 1 } else { 0 })
		})
	}

	fn print(&self) {
		for z in self.bounds.z.min..=self.bounds.z.max {
			eprintln!("z: {}", z);
			for y in self.bounds.y.min..=self.bounds.y.max {
				for x in self.bounds.x.min..=self.bounds.x.max {
					if self.get_status(&Coords { x, y, z }) == Status::Active {
						eprint!("#");
					} else {
						eprint!(".");
					}
				}
				eprintln!("");
			}
		}
	}
}

fn parse_input() -> Map {
	let mut text = String::from("");
	io::stdin().lock().read_to_string(&mut text).expect("Failed to read from stdin");

	let mut map = HashMap::<Coords, Status>::new();
	let mut ymax = 0;
	let mut xmax = 0;

	for (y, line) in text.trim().split("\n").enumerate() {
		for (x, chr) in line.chars().enumerate() {
			let status: Status;
			if chr == '#' {
				status = Status::Active;
			} else if chr == '.' {
				status = Status::Dead;
			} else {
				panic!("Unexpected character \"{}\", expected \"#\" or \".\"\n", chr);
			}
			
			map.insert(Coords { x: x as isize, y: y as isize, z: 0 }, status);
			xmax = x;
		}
		// Here we should check if each line has the same length, but whatever
		ymax = y;
	}

	return Map {
		cubes: map,
		bounds: Bounds {
			x: BoundRange { min: 0, max: xmax as isize },
			y: BoundRange { min: 0, max: ymax as isize },
			z: BoundRange { min: 0, max: 0 },
		},
	};
}

fn cycle(old: Map) -> Map {
	let mut new = Map {
		cubes: HashMap::new(),
		bounds: old.bounds.enlarge(),
	};

	for z in new.bounds.z.min..=new.bounds.z.max {
		for y in new.bounds.y.min..=new.bounds.y.max {
			for x in new.bounds.x.min..=new.bounds.x.max {
				let mut count_active = 0;
				for delta_z in -1..=1 {
					for delta_y in -1..=1 {
						for delta_x in -1..=1 {
							if (delta_x == 0) && (delta_y == 0) && (delta_z == 0) {
								continue
							}

							let coords = Coords { x: x + delta_x, y: y + delta_y, z: z + delta_z };
							if old.get_status(&coords) == Status::Active {
								count_active += 1;
							}
						}
					}
				}

				let coords = Coords { x, y, z };
				let new_status = match old.get_status(&coords) {
					Status::Active => {
						if (count_active == 2) || (count_active == 3) {
							Status::Active
						} else {
							Status::Dead
						}
					},
					Status::Dead => {
						if count_active == 3 {
							Status::Active
						} else {
							Status::Dead
						}
					}
				};
				new.cubes.insert(coords, new_status);
			}
		}
	}
	return new;
}

fn main() {
	let mut current = parse_input();
	eprintln!("Cycle 0: {}", current.count_active());
	// current.print();

	for c in 1..=6 {
		current = cycle(current);

		// eprintln!("");
		eprintln!("Cycle {}: {}", c, current.count_active());
		// current.print();
	}
}
