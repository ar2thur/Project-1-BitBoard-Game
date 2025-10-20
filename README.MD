# Checkers Game Documentation

## Compilation and Execution

### Compilation Instructions
```bash
gcc -o checkers checkers.c
```

### Execution
```bash
./checkers
```

## Board Representation

The game board is displayed as an 8x8 grid with coordinate labels.

```
  0 1 2 3 4 5 6 7
  ---------------
7|b| |b| |b| |b| |
6| |b| |b| |b| |b|
5|b| |b| |b| |b| |
4| | | | | | | | |
3| | | | | | | | |
2| |r| |r| |r| |r|
1|r| |r| |r| |r| |
0| |r| |r| |r| |r|
  ---------------
```

### Piece Notation
- r: Red regular piece
- R: Red king piece
- b: Black regular piece
- B: Black king piece
- Empty space: Unoccupied playable square
- Filled square: Non-playable light square

### Coordinate System
- Columns are numbered 0-7 horizontally
- Rows are numbered 0-7 vertically
- Position is specified as (row, column)

## Move Input Format

Moves are entered as four space-separated integers:
```
from_row from_col to_row to_col
```

### Valid Move Examples
```
2 1 3 2
2 3 3 4
2 1 4 3
```

### Terminating the Game
Enter the following to quit:
```
-1 -1 -1 -1
```

## Game Rules

### Movement Restrictions

#### Regular Pieces
- Move diagonally forward only
- Red pieces move toward row 7
- Black pieces move toward row 0
- Cannot move backward
- Move one square per turn in non-capture moves

#### King Pieces
- Move diagonally in any direction
- Can move forward or backward
- Move one square per turn in non-capture moves

### Capture Mechanics
- Pieces capture by jumping diagonally over opponent pieces
- Captured pieces are removed from the board
- Capture moves span two squares with the opponent piece in the middle square
- Only opponent pieces can be captured

### King Promotion
- Red pieces reaching row 7 are promoted to kings
- Black pieces reaching row 0 are promoted to kings
- Promotion occurs automatically upon reaching the designated row

### Victory Conditions
- A player wins when the opponent has no remaining pieces
- The game announces the winner upon completion

## Error Messages

### Invalid Move
This error occurs when:
- Destination square is occupied
- Move is not diagonal
- Move direction violates piece type restrictions
- Source square does not contain a piece belonging to the current player
- Destination is a non-playable light square

### Invalid Input Format
This error occurs when input does not consist of four space-separated integers.

## Gameplay Strategy

### Opening Moves
Begin by advancing pieces in rows 2 or 5 depending on color assignment.

### Capture Opportunities
Identify positions where opponent pieces can be jumped and removed from play.

### King Advancement
Prioritize moving pieces to the opposite end of the board for promotion.

### Defensive Positioning
Maintain pieces in positions that prevent opponent captures.

## Technical Implementation

### Data Structure
The game uses bitboard representation for piece tracking:
- Four 64-bit unsigned integers represent piece positions
- Each bit corresponds to one board square
- Separate bitboards track red pieces, red kings, black pieces, and black kings

### Bitwise Operations
The implementation uses bitwise AND, OR, NOT, and shift operations for efficient board state manipulation.

### Position Indexing
Board positions are calculated as: position = row * 8 + column

## Program Execution Flow

### Initialization Phase
The program executes Phase 1 and Phase 2 tests before starting the main game loop.

#### Phase 1 Tests
Validates bit manipulation functions including SetBit, ClearBit, ToggleBit, and GetBit operations.

#### Phase 2 Tests
Initializes the game board and displays the starting position with piece counts.

### Main Game Loop
The program alternates between players, accepting move input and validating moves according to checkers rules until a victory condition is met.

## Troubleshooting

### No Output Display
If the program produces no output, verify:
- Compilation completed without errors
- Executable permissions are set correctly
- Terminal supports standard output

### Character Display Issues
Some terminals may display block characters incorrectly. This is a rendering issue and does not affect functionality.

### Input Buffer Issues
If input is not accepted, ensure proper formatting with space-separated integers and press Enter to submit.

## Program Specifications

### Starting Configuration
- 12 red pieces in rows 0-2
- 12 black pieces in rows 5-7
- All pieces placed on dark squares only
- Red player moves first

### Memory Usage
The program uses four 8-byte integers for board state representation, totaling 32 bytes for complete game state storage.

### Performance Characteristics
Bitwise operations execute in constant time, providing efficient move validation and board state updates.