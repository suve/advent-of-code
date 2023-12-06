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

fn find_lowest(
	time_limit: usize,
	dist_to_beat: usize,
) -> usize {
	let mut lower_bound: usize = 0;
	let mut upper_bound: usize = (time_limit / 2) + 1;

	while (upper_bound - lower_bound) > 1 {
		let middle = (lower_bound + upper_bound) / 2;
		if calc_dist(time_limit, middle) > dist_to_beat {
			upper_bound = middle;
		} else {
			lower_bound = middle + 1;
		}
	}

	if calc_dist(time_limit, lower_bound) > dist_to_beat {
		lower_bound
	} else {
		upper_bound
	}
}

fn find_highest(
	time_limit: usize,
	dist_to_beat: usize,
) -> usize {
	let mut lower_bound: usize = time_limit / 2;
	let mut upper_bound: usize = time_limit;

	while (upper_bound - lower_bound) > 1 {
		let middle = (lower_bound + upper_bound) / 2;
		if calc_dist(time_limit, middle) > dist_to_beat {
			lower_bound = middle;
		} else {
			upper_bound = middle - 1;
		}
	}

	if calc_dist(time_limit, upper_bound) > dist_to_beat {
		upper_bound
	} else {
		lower_bound
	}
}

fn calc_num_options(time_limit: usize, dist_to_beat: usize) -> usize {
	let low = find_lowest(time_limit, dist_to_beat);
	let high = find_highest(time_limit, dist_to_beat);
	high - low + 1
}

fn main() {
	let mut part_one: usize = 1;

	let mut big_time = "".to_string();
	let mut big_dist = "".to_string();

	let (time, dist) = read_input();
	for (t, d) in time.split_ascii_whitespace().zip(dist.split_ascii_whitespace()) {
		big_time += t;
		big_dist += d;

		let time_limit: usize = t.parse().unwrap();
		let dist_to_beat: usize = d.parse().unwrap();
		part_one *= calc_num_options(time_limit, dist_to_beat);
	}
	println!("Part1: {}", part_one);

	let big_time: usize = big_time.parse().unwrap();
	let big_dist: usize = big_dist.parse().unwrap();
	let part_two = calc_num_options(big_time, big_dist);
	println!("Part2: {}", part_two);
}
