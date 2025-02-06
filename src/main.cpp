#include <iostream>
#include <memory>
#include "core/card.hpp"

using namespace patience;

int main() {
    std::cout << "Patience Game Starting...\n";
    
    // Create a test card to verify our core library is working
    auto card = std::make_unique<Card>(Suit::HEARTS, Rank::ACE);
    std::cout << "Created a card: " 
              << "Suit: " << static_cast<int>(card->getSuit()) 
              << ", Rank: " << static_cast<int>(card->getRank()) << "\n";
    
    std::cout << "Card is face " << (card->isFaceUp() ? "up" : "down") << "\n";
    card->flip();
    std::cout << "After flip, card is face " << (card->isFaceUp() ? "up" : "down") << "\n";
    
    return 0;
}