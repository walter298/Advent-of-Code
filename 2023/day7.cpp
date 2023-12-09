#include "day7.h"

enum HandType {
	High,
	OnePair,
	TwoPair,
	ThreeOfAKind,
	FullHouse,
	FourOfAKind,
	FiveOfAKind
};

class Hand {
public:
	std::array<int, 5> m_hand;
	int m_bid;
	HandType m_handType;
public:
	Hand(const std::string& line, int bid);
	int getBid() const;
	bool operator<(const Hand& other) const;
};

Hand::Hand(const std::string& line, int bid) : m_bid{ bid } {
	constexpr int JOKER = 1;

	for (const auto& [idx, c] : std::views::enumerate(line)) {
		switch (c) {
		case 'A':
			m_hand[idx] = 14;
			break;
		case 'K':
			m_hand[idx] = 13;
			break;
		case 'Q':
			m_hand[idx] = 12;
			break;
		case 'J':
			m_hand[idx] = JOKER;
			break;
		case 'T':
			m_hand[idx] = 10;
			break;
		default:
			m_hand[idx] = c - '0';
			break;
		}
	}
	
	auto tempHand = m_hand;
	std::ranges::sort(tempHand);

	int run1C = 0;
	int run2C = 0;
	int run1 = 0;
	int run2 = 0;

	int last = -1;
	for (const auto& n : tempHand) {
		if (n == last) {
			continue;
		}
		last = n;
		auto currC = static_cast<int>(std::ranges::count(tempHand, n));
		if (currC > run1C) {
			run2C = run1C;
			run1C = currC;
			run2 = run1;
			run1 = n;
		} else if (currC > run2C) {
			run2C = currC;
			run2 = n;
		}
	}

	auto order = [](const int& run1C, const int& run2C) {
		if (run1C == 5) {
			return FiveOfAKind;
		} else if (run1C == 4) {
			return FourOfAKind;
		} else if (run1C == 3 && run2C == 2) {
			return FullHouse;
		} else if (run1C == 3) {
			return ThreeOfAKind;
		} else if (run1C == 2 && run2C == 2) {
			return TwoPair;
		} else if (run1C == 2) {
			return OnePair;
		} else {
			return High;
		}
	};

	auto jokerC = static_cast<int>(std::ranges::count(tempHand, JOKER));

	if (jokerC > 0) {
		if (run1 != JOKER) {
			run1C += jokerC;
			if (run2 == JOKER) {
				run2C -= jokerC; 
			}
		} else {
			run1C += run2C;
		}
		m_handType = order(run1C, run2C);
	} else {
		m_handType = order(run1C, run2C);
	}
}

int Hand::getBid() const {
	return m_bid;
}

bool Hand::operator<(const Hand& other) const {
	if (m_handType != other.m_handType) {
		return m_handType < other.m_handType;
	}
	for (const auto& [c1, c2] : std::views::zip(m_hand, other.m_hand)) {
		if (c1 != c2) {
			return c1 < c2;
		} 
	}
	std::unreachable();
}

void runDay7() {
	std::set<Hand> hands;
	
	advent::scanForEachLine("2023/inputs/day7.txt"s, 
		[&hands](std::string& handStr, int bid) {
			hands.emplace(std::move(handStr), bid);
		},
		std::string{}, 0
	);
	
	int total = 0;
	for (const auto& [idx, hand] : std::views::enumerate(hands)) {
		total += (hand.getBid() * (static_cast<int>(idx) + 1));
	}
	std::cout << "Total: " << total << '\n';
}