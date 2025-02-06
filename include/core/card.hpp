#pragma once

#include <memory>

namespace patience {

// Enums for card properties
enum class Suit { HEARTS, DIAMONDS, CLUBS, SPADES };
enum class Rank {
  ACE = 1,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  TEN,
  JACK,
  QUEEN,
  KING
};

class Card {
public:
  // Constructors
  Card(Suit suit, Rank rank);
  Card(const Card &other);
  Card(Card &&other) noexcept;

  // Getters
  Suit getSuit() const;
  Rank getRank() const;
  bool isFaceUp() const;

  // Actions
  void flip();
  std::unique_ptr<Card> clone() const;

  // Operators
  Card &operator=(const Card &other);
  Card &operator=(Card &&other) noexcept;
  bool operator==(const Card &other) const;
  bool operator!=(const Card &other) const;

private:
  Suit suit_;
  Rank rank_;
  bool faceUp_;
};

} // namespace patience