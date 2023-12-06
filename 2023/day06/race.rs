use std::{
	io::{BufRead, BufReader},
};

fn read_input() -> (String, String) {
	let mut reader = BufReader::new(std::io::stdin());

	let mut line_one = String::new();
	reader.read_line(&mut line_one).unwrap();
	
	let mut line_two = String::new();
	reader.read_line(&mut line_two).unwrap();

	(
		line_one.strip_prefix("Time:").unwrap().to_string(),
		line_two.strip_prefix("Distance:").unwrap().to_string(),
	)
}

fn calc_dist(time_limit: usize, button_time: usize) -> usize {
	(time_limit - button_time) * button_time
}

fn calc_num_options(time_limit: usize, dist_to_beat: usize) -> usize {
	// I could probably calculate this using some kind of binary-search manner,
	// but I haven't done any math for a long time and I'm so out of practice
	// that I failed to figure it out
	let mut low: usize = 0;
	while calc_dist(time_limit, low) <= dist_to_beat {
		low += 1;
	}

	let mut high: usize = time_limit;
	while calc_dist(time_limit, high) <= dist_to_beat {
		high -= 1;
	}

	high - low + 1
}

fn main() {
	let mut result: usize = 1;

	let (time, dist) = read_input();
	for (t, d) in time.split_ascii_whitespace().zip(dist.split_ascii_whitespace()) {
		let time_limit: usize = t.parse().unwrap();
		let dist_to_beat: usize = d.parse().unwrap();
		result *= calc_num_options(time_limit, dist_to_beat);
	}
	println!("Part1: {}", result);
}
