#include "core/card.hpp"

namespace patience {

Card::Card(Suit suit, Rank rank) : suit_(suit), rank_(rank), faceUp_(false) {}

Card::Card(const Card &other)
    : suit_(other.suit_), rank_(other.rank_), faceUp_(other.faceUp_) {}

Card::Card(Card &&other) noexcept
    : suit_(other.suit_), rank_(other.rank_), faceUp_(other.faceUp_) {}

Suit Card::getSuit() const { return suit_; }

Rank Card::getRank() const { return rank_; }

bool Card::isFaceUp() const { return faceUp_; }

void Card::flip() { faceUp_ = !faceUp_; }

std::unique_ptr<Card> Card::clone() const {
  return std::make_unique<Card>(*this);
}

Card &Card::operator=(const Card &other) {
  if (this != &other) {
    suit_ = other.suit_;
    rank_ = other.rank_;
    faceUp_ = other.faceUp_;
  }
  return *this;
}

Card &Card::operator=(Card &&other) noexcept {
  if (this != &other) {
    suit_ = other.suit_;
    rank_ = other.rank_;
    faceUp_ = other.faceUp_;
  }
  return *this;
}

bool Card::operator==(const Card &other) const {
  return suit_ == other.suit_ && rank_ == other.rank_;
}

bool Card::operator!=(const Card &other) const { return !(*this == other); }

} // namespace patience