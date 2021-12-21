use std::collections::HashMap;
use std::io;
use std::io::BufRead as _;

const MAX_SCORE: usize = 21;

#[derive(Clone, Copy, Eq, Hash, PartialEq)]
struct State {
	pos: [usize; 2],
	score: [usize; 2],
	turn: usize,
}

fn read_input() -> State {
	let mut lines = io::BufReader::new(io::stdin()).lines();
	let p1 = lines.next().unwrap().unwrap().split(' ').last().unwrap().parse::<usize>().unwrap();
	let p2 = lines.next().unwrap().unwrap().split(' ').last().unwrap().parse::<usize>().unwrap();
	return State {
		pos: [p1 - 1, p2 - 1],
		score: [0, 0],
		turn: 0,
	}
}

fn add_state(map: &mut HashMap<State, usize>, state: State, count: usize) {
	if let Some(new_count) = map.get_mut(&state) {
		*new_count += count;
	} else {
		map.insert(state, count);
	}
}

fn main() {
	let mut states = HashMap::<State, usize>::new();
	states.insert(read_input(), 1);

	loop {
		let mut changed: usize = 0;

		let mut new_states = HashMap::<State, usize>::new();
		for (state, count) in states {
			if state.score[0] >= MAX_SCORE || state.score[1] >= MAX_SCORE {
				add_state(&mut new_states, state, count);
				continue;
			}

			/*
			 * The dice can return one of three values: 1, 2 or 3.
			 * A player rolls the dice three times.
			 * This gives us 3**3, or 27 possible outcomes:
			 * - 1 + 1 + 1 -> 3
			 * - 1 + 1 + 2 -> 4
			 * - 1 + 1 + 3 -> 5
			 * - 1 + 2 + 1 -> 4
			 * - 1 + 2 + 2 -> 5
			 * - 1 + 2 + 3 -> 6
			 * - 1 + 3 + 1 -> 5
			 * - 1 + 3 + 2 -> 6
			 * - 1 + 3 + 3 -> 7
			 * - 2 + 1 + 1 -> 4
			 * - 2 + 1 + 2 -> 5
			 * - 2 + 1 + 3 -> 6
			 * - 2 + 2 + 1 -> 5
			 * - 2 + 2 + 2 -> 6
			 * - 2 + 2 + 3 -> 7
			 * - 2 + 3 + 1 -> 6
			 * - 2 + 3 + 2 -> 7
			 * - 2 + 3 + 3 -> 8
			 * - 3 + 1 + 1 -> 5
			 * - 3 + 1 + 2 -> 6
			 * - 3 + 1 + 3 -> 7
			 * - 3 + 2 + 1 -> 6
			 * - 3 + 2 + 2 -> 7
			 * - 3 + 2 + 3 -> 8
			 * - 3 + 3 + 1 -> 7
			 * - 3 + 3 + 2 -> 8
			 * - 3 + 3 + 3 -> 9
			 * Despite 27 possible throws, there are only 7 distinct results:
			 * - 3 occurs 1 time
			 * - 4 occurs 3 times
			 * - 5 occurs 6 times
			 * - 6 occurs 7 times
			 * - 7 occurs 6 times
			 * - 8 occurs 3 times
			 * - 9 occurs 1 time
			 */
			let possibilities = vec![(3, 1), (4, 3), (5, 6), (6, 7), (7, 6), (8, 3), (9, 1)];
			for (value, throws) in possibilities {
				let mut new_state = state;
				new_state.pos[new_state.turn] = (new_state.pos[new_state.turn] + value) % 10;
				new_state.score[new_state.turn] += new_state.pos[new_state.turn] + 1;
				new_state.turn = (new_state.turn + 1) % 2;

				add_state(&mut new_states, new_state, count * throws);
				changed += 1;
			}
		}

		states = new_states;
		if changed == 0 {
			break
		}
	}

	let mut wins: [usize; 2] = [0, 0];
	for (state, count) in &states {
		if state.score[0] > state.score[1] {
			wins[0] += count;
		} else {
			wins[1] += count;
		}
	}

	println!("Player 1 wins in {} universes", wins[0]);
	println!("Player 2 wins in {} universes", wins[1]);
	println!("The winner is player {}!", if wins[0] > wins[1] { 1 } else { 2 });
}
