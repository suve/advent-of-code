use std::{
	convert::TryInto,
	io::{BufRead, BufReader},
};

// --- Part1 structs

#[derive(Debug, Clone, Copy, Eq, Ord, PartialEq, PartialOrd)]
enum Card {
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Jack,
	Queen,
	King,
	Ace
}

impl From<char> for Card {
	fn from(c: char) -> Self {
		match c {
			'2' => Self::Two,
			'3' => Self::Three,
			'4' => Self::Four,
			'5' => Self::Five,
			'6' => Self::Six,
			'7' => Self::Seven,
			'8' => Self::Eight,
			'9' => Self::Nine,
			'T' => Self::Ten,
			'J' => Self::Jack,
			'Q' => Self::Queen,
			'K' => Self::King,
			'A' => Self::Ace,
			_ => panic!("Invalid card: \"{}\"", c)
		}
	}
}

#[derive(Debug, Eq, Ord, PartialEq, PartialOrd)]
enum HandKind { // type is a reserved word, so let's use "kind"
	HighCard,
	OnePair,
	TwoPairs,
	ThreeOfAKind,
	FullHouse,
	FourOfAKind,
	FiveOfAKind,
}

impl From<&[Card; 5]> for HandKind {
	fn from(cards: &[Card; 5]) -> Self {
		const KINDS: usize = (Card::Ace as usize) + 1;

		let mut count: [usize; KINDS] = [0; KINDS];
		for c in cards {
			count[*c as usize] += 1;
		}

		for i in 0..KINDS {
			if count[i] == 5 {
				return Self::FiveOfAKind;
			}
			if count[i] == 4 {
				return Self::FourOfAKind;
			}
			if count[i] == 3 {
				for j in (i+1)..KINDS {
					if count[j] == 2 {
						return Self::FullHouse;
					}
				}
				return Self::ThreeOfAKind;
			}
			if count[i] == 2 {
				for j in (i+1)..KINDS {
					if count[j] == 3 {
						return Self::FullHouse;
					}
					if count[j] == 2 {
						return Self::TwoPairs;
					}
				}
				return Self::OnePair;
			}
		}

		Self::HighCard
	}
}

#[derive(Debug, Eq, Ord, PartialOrd, PartialEq)]
struct Hand {
	pub kind: HandKind,
	pub cards: [Card; 5],
}

impl From<&str> for Hand {
	fn from(s: &str) -> Self {
		let cards: Vec<_> = s.chars().map(Card::from).collect();
		if cards.len() != 5 {
			panic!("Invalid hand size: expected 5 cards, got {}", cards.len());
		}

		let cards: [Card; 5] = cards.try_into().unwrap();
		Self {
			kind: HandKind::from(&cards),
			cards
		}
	}
}

// --- Part2 struct

#[derive(Debug, Clone, Copy, Eq, Ord, PartialEq, PartialOrd)]
enum JokeCard {
	Joker,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Queen,
	King,
	Ace
}

impl From<char> for JokeCard {
	fn from(c: char) -> Self {
		match c {
			'J' => Self::Joker,
			'2' => Self::Two,
			'3' => Self::Three,
			'4' => Self::Four,
			'5' => Self::Five,
			'6' => Self::Six,
			'7' => Self::Seven,
			'8' => Self::Eight,
			'9' => Self::Nine,
			'T' => Self::Ten,
			'Q' => Self::Queen,
			'K' => Self::King,
			'A' => Self::Ace,
			_ => panic!("Invalid card: \"{}\"", c)
		}
	}
}

#[derive(Debug, Eq, Ord, PartialEq, PartialOrd)]
enum JokeHandKind { // type is a reserved word, so let's use "kind"
	HighCard,
	OnePair,
	TwoPairs,
	ThreeOfAKind,
	FullHouse,
	FourOfAKind,
	FiveOfAKind,
}

impl From<&[JokeCard; 5]> for JokeHandKind {
	fn from(cards: &[JokeCard; 5]) -> Self {
		const KINDS: usize = JokeCard::Ace as usize;

		let mut jokers = 0;
		let mut count: [usize; KINDS] = [0; KINDS];
		for c in cards {
			if *c == JokeCard::Joker {
				jokers += 1;
			} else {
				count[(*c as usize) - 1] += 1;
			}
		}

		count.sort();
		count.reverse();
		if count[0] + jokers == 5 {
			return Self::FiveOfAKind;
		}
		if count[0] + jokers == 4 {
			return Self::FourOfAKind;
		}
		if count[0] + jokers == 3 {
			if count[1] == 2 {
				return Self::FullHouse;
			}
			return Self::ThreeOfAKind;
		}
		if count[0] + jokers == 2 {
			if count[1] == 2 {
				return Self::TwoPairs;
			}
			return Self::OnePair;
		}

		Self::HighCard
	}
}

#[derive(Debug, Eq, Ord, PartialOrd, PartialEq)]
struct JokeHand {
	pub kind: JokeHandKind,
	pub cards: [JokeCard; 5],
}

impl From<&str> for JokeHand {
	fn from(s: &str) -> Self {
		let cards: Vec<_> = s.chars().map(JokeCard::from).collect();
		if cards.len() != 5 {
			panic!("Invalid hand size: expected 5 cards, got {}", cards.len());
		}

		let cards: [JokeCard; 5] = cards.try_into().unwrap();
		Self {
			kind: JokeHandKind::from(&cards),
			cards
		}
	}
}

// --- Common stuff

#[derive(Debug)]
struct Bid {
	pub hand: Hand,
	pub joke_hand: JokeHand,
	pub amount: usize
}

impl From<&str> for Bid {
	fn from(s: &str) -> Self {
		let mut i = s.split_ascii_whitespace();
		let hand_str = i.next().unwrap();
		let amount_str = i.next().unwrap();

		Self { 
			hand: Hand::from(hand_str),
			joke_hand: JokeHand::from(hand_str),
			amount: amount_str.parse().unwrap(),
		}
	}
}

fn read_input() -> Vec<Bid> {
	let mut reader = BufReader::new(std::io::stdin());

	let mut bids = vec![];
	loop {
		let mut line = String::new();
		let bytes = reader.read_line(&mut line).unwrap();
		if bytes > 0 {
			bids.push(Bid::from(&*line));
		} else {
			return bids;
		}
	}
}

fn winnings(bids: &[Bid]) -> usize {
	bids.iter().enumerate().fold(0, |acc, (idx, bid)| acc + bid.amount * (idx + 1))
}

fn main() {
	let mut bids = read_input();

	bids.sort_by(|left, right| left.hand.cmp(&right.hand));
	println!("Part1: {}", winnings(&bids));

	bids.sort_by(|left, right| left.joke_hand.cmp(&right.joke_hand));
	println!("Part2: {}", winnings(&bids));
}
