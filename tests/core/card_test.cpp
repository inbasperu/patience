#include "core/card.hpp"
#include <gtest/gtest.h>

using namespace patience;

class CardTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Create a test card before each test
    testCard = std::make_unique<Card>(Suit::HEARTS, Rank::ACE);
  }

  std::unique_ptr<Card> testCard;
};

TEST_F(CardTest, Constructor) {
  Card card(Suit::SPADES, Rank::KING);
  EXPECT_EQ(card.getSuit(), Suit::SPADES);
  EXPECT_EQ(card.getRank(), Rank::KING);
  EXPECT_FALSE(card.isFaceUp());
}

TEST_F(CardTest, Flip) {
  EXPECT_FALSE(testCard->isFaceUp());
  testCard->flip();
  EXPECT_TRUE(testCard->isFaceUp());
  testCard->flip();
  EXPECT_FALSE(testCard->isFaceUp());
}

TEST_F(CardTest, Clone) {
  testCard->flip(); // Make it face up
  auto clonedCard = testCard->clone();

  EXPECT_EQ(clonedCard->getSuit(), testCard->getSuit());
  EXPECT_EQ(clonedCard->getRank(), testCard->getRank());
  EXPECT_EQ(clonedCard->isFaceUp(), testCard->isFaceUp());
  EXPECT_NE(clonedCard.get(), testCard.get()); // Different memory addresses
}

TEST_F(CardTest, Comparison) {
  Card card1(Suit::HEARTS, Rank::ACE);
  Card card2(Suit::HEARTS, Rank::ACE);
  Card card3(Suit::SPADES, Rank::ACE);

  EXPECT_EQ(card1, card2);
  EXPECT_NE(card1, card3);
}