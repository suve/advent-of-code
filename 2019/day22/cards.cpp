#include <cstdlib>
#include <deque>
#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::string;

int CARDS_TOTAL;

using Deck = std::deque<int>;

void newstack(Deck &deck) {
	for(int i = 0; i < CARDS_TOTAL / 2; ++i) {
		int temp = deck[i];
		deck[i] = deck[CARDS_TOTAL - 1 - i];
		deck[CARDS_TOTAL - 1 - i] = temp;
	}
}

void cut(Deck &deck, int number) {
	if(number > 0) {
		while(number --> 0) {
			int first = deck[0];
			deck.push_back(first);
			deck.pop_front();
		}
	} else {
		while(number ++< 0) {
			int last = deck[CARDS_TOTAL-1];
			deck.push_front(last);
			deck.pop_back();
		}
	}
}

void increment(Deck &deck, int number) {
	Deck copy = deck;

	int position = 0;
	while(copy.size() > 0) {
		deck[position] = copy[0];
		copy.pop_front();

		position = (position + number) % CARDS_TOTAL;
	}
}

int main() {
	string line;
	std::getline(cin, line);
	CARDS_TOTAL = std::stoi(line);

	Deck deck;
	for(int i = 0; i < CARDS_TOTAL; ++i) {
		deck.push_back(i);
	}

	while(std::getline(cin, line)) {
		if(line.find("deal into new stack") != string::npos) {
			newstack(deck);
		} else if(line.find("deal with increment") != string::npos) {
			increment(deck, std::stoi(line.substr(19)));
		} else if(line.find("cut") != string::npos) {
			cut(deck, std::stoi(line.substr(3)));
		} else {
			cout << "Unknown operation: \"" << line << "\"\n";
			exit(EXIT_FAILURE);
		}
	}

	if(CARDS_TOTAL <= 100) {
		for(int i = 0; i < CARDS_TOTAL; ++i) cout << deck[i] << " ";
		cout << "\n";
	} else {
		cout << "deck[2019]: " << deck[2019] << "\n";

		for(int i = 0; i < CARDS_TOTAL; ++i) {
			if(deck[i] == 2019) {
				cout << "deck[" << i << "]: 2019\n";
				break;
			}
		}
	}
}
