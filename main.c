#include <stdio.h>
#include <stdlib.h>

unsigned int SetBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return value;
    return value | (1 << position);
}

unsigned int ClearBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return value;
    return value & ~(1 << position);
}

unsigned int ToggleBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return value;
    return value ^ (1 << position);
}

int GetBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return 0;
    return (value >> position) & 1;
}

int CountBits(unsigned int value) {
    int count = 0;
    while (value) {
        count += value & 1;
        value >>= 1;
    }
    return count;
}

unsigned int ShiftLeft(unsigned int value, int positions) {
    if (positions < 0 || positions >= 32) return value;
    return value << positions;
}

unsigned int ShiftRight(unsigned int value, int positions) {
    if (positions < 0 || positions >= 32) return value;
    return value >> positions;
}

void PrintBinary(unsigned int value) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i % 8 == 0) printf(" ");
    }
    printf("\n");
}

void PrintHex(unsigned int value) {
    printf("0x%08X\n", value);
}

typedef struct {
    unsigned long long red_pieces;
    unsigned long long red_kings;
    unsigned long long black_pieces;
    unsigned long long black_kings;
    int current_player;
} GameState;

unsigned long long SetBit64(unsigned long long value, int position) {
    if (position < 0 || position >= 64) return value;
    return value | (1ULL << position);
}

unsigned long long ClearBit64(unsigned long long value, int position) {
    if (position < 0 || position >= 64) return value;
    return value & ~(1ULL << position);
}

int GetBit64(unsigned long long value, int position) {
    if (position < 0 || position >= 64) return 0;
    return (value >> position) & 1;
}

int CountBits64(unsigned long long value) {
    int count = 0;
    while (value) {
        count += value & 1;
        value >>= 1;
    }
    return count;
}

void InitializeGame(GameState* game) {
    game->red_pieces = 0ULL;
    game->red_kings = 0ULL;
    game->black_pieces = 0ULL;
    game->black_kings = 0ULL;
    game->current_player = 0;
    
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1) {
                int pos = row * 8 + col;
                game->red_pieces = SetBit64(game->red_pieces, pos);
            }
        }
    }
    
    for (int row = 5; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1) {
                int pos = row * 8 + col;
                game->black_pieces = SetBit64(game->black_pieces, pos);
            }
        }
    }
}

void PrintBoard(GameState* game) {
    printf("\n  0 1 2 3 4 5 6 7\n");
    printf("  ---------------\n");
    
    for (int row = 7; row >= 0; row--) {
        printf("%d|", row);
        for (int col = 0; col < 8; col++) {
            int pos = row * 8 + col;
            
            if (GetBit64(game->red_kings, pos)) {
                printf("R|");
            } else if (GetBit64(game->red_pieces, pos)) {
                printf("r|");
            } else if (GetBit64(game->black_kings, pos)) {
                printf("B|");
            } else if (GetBit64(game->black_pieces, pos)) {
                printf("b|");
            } else if ((row + col) % 2 == 1) {
                printf(" |");
            } else {
                printf("█|");
            }
        }
        printf("\n");
    }
    printf("  ---------------\n");
    printf("r=red piece, R=red king, b=black piece, B=black king\n\n");
}

int IsValidPosition(int row, int col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

int PositionToIndex(int row, int col) {
    return row * 8 + col;
}

int IsValidMove(GameState* game, int from_row, int from_col, int to_row, int to_col) {
    if (!IsValidPosition(from_row, from_col) || !IsValidPosition(to_row, to_col)) {
        return 0;
    }
    
    if ((from_row + from_col) % 2 == 0 || (to_row + to_col) % 2 == 0) {
        return 0;
    }
    
    int from_pos = PositionToIndex(from_row, from_col);
    int to_pos = PositionToIndex(to_row, to_col);
    
    if (GetBit64(game->red_pieces, to_pos) || GetBit64(game->red_kings, to_pos) ||
        GetBit64(game->black_pieces, to_pos) || GetBit64(game->black_kings, to_pos)) {
        return 0;
    }
    
    int is_red_piece = GetBit64(game->red_pieces, from_pos);
    int is_red_king = GetBit64(game->red_kings, from_pos);
    int is_black_piece = GetBit64(game->black_pieces, from_pos);
    int is_black_king = GetBit64(game->black_kings, from_pos);
    
    if (game->current_player == 0) {
        if (!is_red_piece && !is_red_king) return 0;
    } else {
        if (!is_black_piece && !is_black_king) return 0;
    }
    
    int row_diff = to_row - from_row;
    int col_diff = abs(to_col - from_col);
    
    if (abs(row_diff) != col_diff) return 0;
    
    if (abs(row_diff) == 1) {
        if (is_red_piece && row_diff < 0) return 0;
        if (is_black_piece && row_diff > 0) return 0;
        return 1;
    }
    
    if (abs(row_diff) == 2) {
        int mid_row = (from_row + to_row) / 2;
        int mid_col = (from_col + to_col) / 2;
        int mid_pos = PositionToIndex(mid_row, mid_col);
        
        if (is_red_piece && row_diff < 0) return 0;
        if (is_black_piece && row_diff > 0) return 0;
        
        if (game->current_player == 0) {
            if (GetBit64(game->black_pieces, mid_pos) || GetBit64(game->black_kings, mid_pos)) {
                return 1;
            }
        } else {
            if (GetBit64(game->red_pieces, mid_pos) || GetBit64(game->red_kings, mid_pos)) {
                return 1;
            }
        }
        return 0;
    }
    
    return 0;
}

void MovePiece(GameState* game, int from_row, int from_col, int to_row, int to_col) {
    int from_pos = PositionToIndex(from_row, from_col);
    int to_pos = PositionToIndex(to_row, to_col);
    
    int is_red_piece = GetBit64(game->red_pieces, from_pos);
    int is_red_king = GetBit64(game->red_kings, from_pos);
    int is_black_piece = GetBit64(game->black_pieces, from_pos);
    int is_black_king = GetBit64(game->black_kings, from_pos);
    
    int row_diff = abs(to_row - from_row);
    if (row_diff == 2) {
        int mid_row = (from_row + to_row) / 2;
        int mid_col = (from_col + to_col) / 2;
        int mid_pos = PositionToIndex(mid_row, mid_col);
        
        game->red_pieces = ClearBit64(game->red_pieces, mid_pos);
        game->red_kings = ClearBit64(game->red_kings, mid_pos);
        game->black_pieces = ClearBit64(game->black_pieces, mid_pos);
        game->black_kings = ClearBit64(game->black_kings, mid_pos);
    }
    
    if (is_red_piece) {
        game->red_pieces = ClearBit64(game->red_pieces, from_pos);
        game->red_pieces = SetBit64(game->red_pieces, to_pos);
        
        if (to_row == 7) {
            game->red_pieces = ClearBit64(game->red_pieces, to_pos);
            game->red_kings = SetBit64(game->red_kings, to_pos);
        }
    } else if (is_red_king) {
        game->red_kings = ClearBit64(game->red_kings, from_pos);
        game->red_kings = SetBit64(game->red_kings, to_pos);
    } else if (is_black_piece) {
        game->black_pieces = ClearBit64(game->black_pieces, from_pos);
        game->black_pieces = SetBit64(game->black_pieces, to_pos);
        
        if (to_row == 0) {
            game->black_pieces = ClearBit64(game->black_pieces, to_pos);
            game->black_kings = SetBit64(game->black_kings, to_pos);
        }
    } else if (is_black_king) {
        game->black_kings = ClearBit64(game->black_kings, from_pos);
        game->black_kings = SetBit64(game->black_kings, to_pos);
    }
    
    game->current_player = 1 - game->current_player;
}

int CheckWin(GameState* game) {
    int red_count = CountBits64(game->red_pieces) + CountBits64(game->red_kings);
    int black_count = CountBits64(game->black_pieces) + CountBits64(game->black_kings);
    
    if (red_count == 0) return 2;
    if (black_count == 0) return 1;
    
    return 0;
}

void TestPhase1() {
    printf("--- PHASE 1 TESTS ---\n\n");
    
    unsigned int test = 0;
    
    printf("Test 1: SetBit\n");
    test = SetBit(test, 3);
    printf("After setting bit 3: ");
    PrintBinary(test);
    printf("Bit count: %d\n\n", CountBits(test));
    
    printf("Test 2: SetBit multiple\n");
    test = SetBit(test, 7);
    test = SetBit(test, 15);
    printf("After setting bits 3, 7, 15: ");
    PrintBinary(test);
    printf("Bit count: %d\n\n", CountBits(test));
    
    printf("Test 3: ClearBit\n");
    test = ClearBit(test, 7);
    printf("After clearing bit 7: ");
    PrintBinary(test);
    printf("Bit count: %d\n\n", CountBits(test));
    
    printf("Test 4: ToggleBit\n");
    test = ToggleBit(test, 3);
    printf("After toggling bit 3: ");
    PrintBinary(test);
    printf("Bit count: %d\n\n", CountBits(test));
    
    printf("Test 5: GetBit\n");
    printf("Bit 15 value: %d\n", GetBit(test, 15));
    printf("Bit 3 value: %d\n\n", GetBit(test, 3));
}

void TestPhase2() {
    printf("\n=== PHASE 2 TESTS ===\n");
    
    GameState game;
    InitializeGame(&game);
    
    printf("Initial board:\n");
    PrintBoard(&game);
    
    printf("Red pieces: %d\n", CountBits64(game.red_pieces));
    printf("Black pieces: %d\n", CountBits64(game.black_pieces));
}

int main() {
    TestPhase1();
    TestPhase2();
    
    GameState game;
    InitializeGame(&game);
    
    printf("\n=== CHECKERS GAME START ===\n");
    printf("Enter moves as: from_row from_col to_row to_col\n");
    printf("Example: 2 1 3 2 (moves piece from row 2, col 1 to row 3, col 2)\n");
    printf("Enter -1 -1 -1 -1 to quit\n\n");
    
    while (!CheckWin(&game)) {
        PrintBoard(&game);
        
        printf("Current player: %s\n", game.current_player == 0 ? "RED" : "BLACK");
        printf("Enter move: ");
        
        int from_row, from_col, to_row, to_col;
        if (scanf("%d %d %d %d", &from_row, &from_col, &to_row, &to_col) != 4) {
            printf("Invalid input format!\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (from_row == -1) {
            printf("Game ended by player.\n");
            break;
        }
        
        if (IsValidMove(&game, from_row, from_col, to_row, to_col)) {
            MovePiece(&game, from_row, from_col, to_row, to_col);
            printf("Move executed!\n\n");
        } else {
            printf("Invalid move! Try again.\n\n");
        }
    }
    
    int winner = CheckWin(&game);
    if (winner == 1) {
        printf("\n RED WINS! \n");
    } else if (winner == 2) {
        printf("\n BLACK WINS! \n");
    }
    
    PrintBoard(&game);
    
    return 0;
}