use std::io;
use std::io::BufRead as _;

const CYCLE_NORMAL: usize = 7;
const CYCLE_EXTENDED: usize = 9;

const SIMULATION_STEPS: usize = 256;

fn step_simulation(state: &mut [usize; CYCLE_EXTENDED]) {
	let mut old_state = [0; CYCLE_EXTENDED];
	for i in 0..CYCLE_EXTENDED {
		old_state[i] = state[i];
	}

	for i in 1..CYCLE_EXTENDED {
		state[i-1] = old_state[i];
	}
	state[CYCLE_NORMAL-1] += old_state[0];  // += because rollover from extended cycle
	state[CYCLE_EXTENDED-1] = old_state[0]; //  = because no rollover
}

fn count(state: &[usize; CYCLE_EXTENDED]) -> usize {
	state.iter().fold(0, |acc, item| acc + item)
}

fn main() {
	for line in io::BufReader::new(io::stdin()).lines() {
		let mut state = [0; CYCLE_EXTENDED];
		for number in line.unwrap().trim().split(',') {
			let number = number.parse::<usize>().unwrap();
			state[number] += 1;
		}

		eprintln!("Initial state: {}", count(&state));
		for step in 1..=SIMULATION_STEPS {
			step_simulation(&mut state);
			eprintln!("After day {:03}: {}", step, count(&state));
		}
	}
}
