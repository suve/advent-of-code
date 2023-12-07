use std::{
	cmp::Ordering,
	convert::TryInto,
	io::{BufRead, BufReader},
};

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

#[derive(Debug, Eq, PartialOrd, PartialEq)]
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

		// This single fucking line took me more time than half of the program
		let cards: [Card; 5] = cards.try_into().unwrap();
		Self {
			kind: HandKind::from(&cards),
			cards
		}
	}
}

impl Ord for Hand {
	fn cmp(&self, other: &Self) -> Ordering {
		if self.kind > other.kind {
			return Ordering::Greater;
		}
		if self.kind < other.kind {
			return Ordering::Less;
		}

		for i in 0..5 {
			let card_cmp = self.cards[i].cmp(&other.cards[i]);
			if card_cmp != Ordering::Equal {
				return card_cmp;
			}
		}

		Ordering::Equal
	}
}

#[derive(Debug, Eq, PartialEq, PartialOrd)]
struct Bid {
	pub hand: Hand,
	pub amount: usize
}

impl From<&str> for Bid {
	fn from(s: &str) -> Self {
		let mut i = s.split_ascii_whitespace();
		let hand = Hand::from(i.next().unwrap());
		let amount: usize = i.next().unwrap().parse().unwrap();

		Self { hand, amount }
	}
}

impl Ord for Bid {
	fn cmp(&self, other: &Self) -> Ordering {
		self.hand.cmp(&other.hand)
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

fn main() {
	let mut bids = read_input();
	bids.sort();

	let mut winnings: usize = 0;
	for (idx, bid) in bids.iter().enumerate() {
		winnings += bid.amount * (idx + 1);
	}
	println!("Part1: {}", winnings);
}
