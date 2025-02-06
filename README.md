
# Patience Game

C++ implementation of Patience (Solitaire) card game with network play support and Cairo-based rendering.

## Architecture Overview

This project follows a layered architecture with separation of concerns:

### Core Layer

- Card/Deck/Pile: Game entities
- GameEngine: Game logic and state management
- Command Pattern: For move execution and undo/redo support

### UI Layer

- SolitaireUI: Main UI controller using MVC pattern
- IRenderer/CairoRenderer: Graphics rendering using Bridge pattern
- Observer Pattern: For UI state synchronization

### Network Layer

- NetworkHandler: Manages remote play capabilities
- X11 Forwarding: For remote display support

## Prerequisites

- CMake 3.20+
- C++17 compliant compiler
- Cairo graphics library
- Google Test framework
- X11 development libraries

## UML Diagrams

### Class diagram

``` mermaid
classDiagram
    %% Core Domain Models implementing Domain-Driven Design
    class Card {
        -Suit suit
        -Rank rank
        -bool faceUp
        +flip() void
        +getSuit() Suit
        +getRank() Rank
        +isFaceUp() bool
        +clone() Card*
    }

    class Deck {
        -Stack~Card~ cards
        -IDeckStrategy* shuffleStrategy
        +shuffle() void
        +draw() Optional~Card~
        +reset() void
        +setShuffleStrategy(IDeckStrategy*)
        +remainingCards() int
    }

    class Pile {
        -Stack~Card~ cards
        -PileType type
        -IPileValidator* validator
        +canAddCard(Card*) bool
        +canRemoveCards(int count) bool
        +addCard(Card*) bool
        +removeCards(int count) vector~Card*~
        +topCard() Optional~Card*~
        +setValidator(IPileValidator*)
    }

    %% Game Logic Layer with Strategy Pattern
    class GameEngine {
        -unique_ptr~Deck~ deck
        -vector~unique_ptr~Pile~~ piles
        -GameState state
        -MoveValidator validator
        -CommandHistory history
        +processMove(unique_ptr~ICommand~) bool
        +undoLastMove() bool
        +redoMove() bool
        +checkWinCondition() bool
        +getState() const GameState&
    }

    %% Observer Pattern for UI Updates
    class IGameObserver {
        <<interface>>
        +onGameStateChanged(GameState&)
        +onMoveRejected(string reason)
        +onGameWon()
    }

    %% Command Pattern for Move Handling
    class ICommand {
        <<interface>>
        +execute() bool
        +undo() bool
    }

    %% Strategy Pattern for Pile Rules
    class IPileValidator {
        <<interface>>
        +isValidMove(Card*, Pile*) bool
    }

    %% UI Layer with MVC Pattern
    class SolitaireUI {
        -GameEngine& engine
        -unique_ptr~IRenderer~ renderer
        -InputHandler inputHandler
        +handleInput(InputEvent)
        +render()
        +registerObserver(IGameObserver*)
    }

    %% Bridge Pattern for Rendering
    class IRenderer {
        <<interface>>
        +renderCard(Card*, Position)
        +renderPile(Pile*, Position)
        +renderGameState(GameState&)
    }

    class CairoRenderer {
        -Cairo::Surface* surface
        -map~string, Texture~ cardTextures
        +loadResources()
        +createBuffer() FrameBuffer
    }

    %% Network Layer with Facade Pattern
    class NetworkHandler {
        -unique_ptr~X11Forwarder~ x11
        -unique_ptr~SSHConnection~ ssh
        -CircularBuffer frameBuffer
        +sendFrame(FrameBuffer&)
        +pollEvents() vector~InputEvent~
        +isConnected() bool
    }

    %% Application Core
    class App {
        -unique_ptr~GameEngine~ engine
        -unique_ptr~SolitaireUI~ ui
        -unique_ptr~NetworkHandler~ network
        -ConfigManager config
        +run()
        +initialize()
        +shutdown()
    }

    %% Relationships
    Deck "1" *-- "*" Card
    GameEngine "1" *-- "1" Deck
    GameEngine "1" *-- "7..10" Pile
    Pile "1" *-- "0..n" Card
    GameEngine --> ICommand
    SolitaireUI --|> IGameObserver
    SolitaireUI --> GameEngine
    SolitaireUI --> IRenderer
    CairoRenderer --|> IRenderer
    App *-- GameEngine
    App *-- SolitaireUI
    App *-- NetworkHandler

```

### State diagram

``` mermaid
stateDiagram-v2
    [*] --> Initialization: Launch App
    
    state Initialization {
        [*] --> LoadingResources
        LoadingResources --> ConnectingNetwork
        ConnectingNetwork --> GameSetup
        GameSetup --> [*]
    }
    
    Initialization --> GameIdle: Ready
    
    state "Game Running" as GameRunning {
        GameIdle --> ProcessingInput: User Input
        ProcessingInput --> ValidatingMove: Command Created
        
        ValidatingMove --> ExecutingMove: Valid
        ValidatingMove --> GameIdle: Invalid
        
        ExecutingMove --> UpdateGameState: Success
        ExecutingMove --> GameIdle: Failure
        
        UpdateGameState --> RenderingFrame: State Changed
        RenderingFrame --> NetworkTransfer: Buffer Ready
        NetworkTransfer --> GameIdle: Frame Sent
    }
    
    GameRunning --> GameOver: Win Condition
    GameOver --> [*]: Exit

```

### Sequence diagram

``` mermaid
sequenceDiagram
    participant Client
    participant Network as NetworkHandler
    participant App
    participant UI as SolitaireUI
    participant Engine as GameEngine
    participant Command as CommandExecutor

    Note over Client,Command: Input Processing Flow
    
    Client->>Network: Send Input Event
    Network->>App: Process Network Input
    App->>UI: Handle Input Event
    
    UI->>Engine: Create Move Command
    Engine->>Command: Validate Command
    
    alt Valid Command
        Command->>Engine: Execute Move
        Engine->>Engine: Update Game State
        Engine->>UI: Notify State Change
        UI->>Network: Send Frame Buffer
        Network->>Client: Forward Frame
    else Invalid Command
        Command-->>UI: Report Invalid Move
        UI->>Network: Send Error Frame
        Network->>Client: Display Error
    end

```

## System-specific Installation Instructions

### macOS ARM64

```bash
# Install dependencies using Homebrew
brew install cmake cairo googletest
```

### Linux

#### Ubuntu/Debian

```bash
# Update package list
sudo apt update

# Install build tools
sudo apt install -y build-essential cmake git

# Install Cairo development libraries
sudo apt install -y libcairo2-dev

# Install X11 development libraries
sudo apt install -y libx11-dev

# Install Google Test
sudo apt install -y libgtest-dev

# Build and install Google Test (required on some distributions)
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp lib/*.a /usr/lib
```

#### Fedora/RHEL

```bash
# Install build tools
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake git

# Install Cairo development libraries
sudo dnf install cairo-devel

# Install X11 development libraries
sudo dnf install libX11-devel

# Install Google Test
sudo dnf install gtest-devel
```

#### Arch Linux

```bash
# Install build tools
sudo pacman -S base-devel cmake git

# Install Cairo development libraries
sudo pacman -S cairo

# Install X11 development libraries
sudo pacman -S libx11

# Install Google Test
sudo pacman -S gtest
```

#### Verifying Installation

After installing dependencies, verify they're correctly installed:

```bash
# Check CMake version
cmake --version  # Should be 3.20 or higher

# Check GCC/Clang version
g++ --version   # Should support C++17
# or
clang++ --version

# Verify pkg-config can find Cairo
pkg-config --modversion cairo

# Verify X11 installation
pkg-config --modversion x11
```

## Building the Project

1. Clone and build:

```bash
git clone https://github.com/inbasperu/patience-game.git
cd patience-game
mkdir build && cd build
cmake ..
make
```

2. Run tests:

```bash
ctest --output-on-failure
```

3. Run the game:

```bash
./bin/patience_game
```

## Project Structure

```bash
.
├── CMakeLists.txt
├── README.md
├── build.sh
├── include         # Public headers
│   └── core
│       └── card.hpp
├── resources       # Game assets
├── src             # Source files
│   ├── CMakeLists.txt
│   ├── core        # Game logic
│   │   └── card.cpp
│   ├── main.cpp
│   ├── network     # Network handling
│   └── ui          # User interface
└── tests           # Test files
    ├── CMakeLists.txt
    └── core
        └── card_test.cpp


```

## License

For more information read the [LICENSE](LICENSE) file.
