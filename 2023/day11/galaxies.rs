use std::{
	io::{BufRead, BufReader},
};

#[derive(Clone)]
struct Galaxy {
	pub x: usize,
	pub y: usize,
}

impl Galaxy {
	fn distance_to(&self, other: &Self) -> usize {
		let x_dist = if self.x >= other.x {
			self.x - other.x
		} else {
			other.x - self.x
		};
		let y_dist = if self.y >= other.y {
			self.y - other.y
		} else {
			other.y - self.y
		};
		x_dist + y_dist
	}
}

#[derive(Clone, Default)]
struct Universe {
	pub galaxies: Vec<Galaxy>,
	pub max_x: usize,
	pub max_y: usize,
}

impl Universe {
	fn expand(&mut self, step: usize) {
		let mut x_count = vec![0_usize; self.max_x + 1];
		let mut y_count = vec![0_usize; self.max_y + 1];
		for g in &self.galaxies {
			x_count[g.x] += 1;
			y_count[g.y] += 1;
		}

		let calc_expand = |count: &[usize]| -> Vec<usize> {
			let mut expand = vec![0; count.len()];
			if count[0] == 0 {
				expand[0] = step;
			}
			for i in 1..(count.len()) {
				expand[i] = expand[i-1];
				if count[i] == 0 {
					expand[i] += step;
				}
			}
			expand
		};

		let x_expand = calc_expand(&x_count);
		let y_expand = calc_expand(&y_count);
		for g in self.galaxies.iter_mut() {
			g.x += x_expand[g.x];
			g.y += y_expand[g.y];
		}
	}
}

fn read_input() -> Universe {
	let mut result = Universe::default();
	let mut y = 0;

	let mut reader = BufReader::new(std::io::stdin());
	loop {
		let mut line = String::new();
		let bytes = reader.read_line(&mut line).unwrap();
		if bytes == 0 {
			break;
		}

		for (x, c) in line.chars().enumerate() {
			if c == '#' {
				result.galaxies.push(Galaxy { x, y });
				if x > result.max_x {
					result.max_x = x;
				}
				if y > result.max_y {
					result.max_y = y;
				}
			}
		}
		y += 1;
	}

	result
}

fn sum_distances(universe: &Universe) -> usize {
	let mut sum = 0;
	for i in 0..universe.galaxies.len() {
		for j in (i+1)..universe.galaxies.len() {
			sum += universe.galaxies[i].distance_to(&universe.galaxies[j]);
		}
	}
	sum
}

fn main() {
	let universe = read_input();

	let mut first = universe.clone();
	first.expand(1);
	println!("Part1: {}", sum_distances(&first));

	let mut second = universe;
	second.expand(1000000 - 1);
	println!("Part2: {}", sum_distances(&second));
}
