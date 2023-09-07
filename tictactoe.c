#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define unreachable() assert(0 && "Unreachable")

typedef enum {
    Player_X    = 'X',
    Player_O    = 'O',
    Player_NULL = ' ',
} Player;

typedef Player
  Square;

typedef Square
  Board[3][3];

typedef enum {
    State_PlayerTurn_X,
    State_PlayerTurn_O,
    State_GameOver_X_won,
    State_GameOver_O_won,
    State_GameOver_Tie,
} State;

typedef struct {
    Board board;
    State state;
} Game;

void   Game_init        (Game* g                        );
void   Game_play        (Game* g, size_t row, size_t col);
Player Game_check_winner(Game* g                        );
void   Game_print       (Game* g                        );

void Game_init(Game* g)
{
    assert(g);

    for (size_t r = 0; r < 3; ++r) {
        for (size_t c = 0; c < 3; ++c) {
            g->board[r][c] = Player_NULL;
        }
    }
    g->state = State_PlayerTurn_X;
}

void Game_play(Game* g, size_t row, size_t col)
{
    assert(g);
    assert(row <= 3 && col <= 3);

    if (g->board[row][col] != Player_NULL) {
        assert(0 && "TODO: return error: square already filled");
    }

    switch (g->state) {
    case State_PlayerTurn_X: {
        g->board[row][col] = Player_X;
        g->state = State_PlayerTurn_O;
    } break;

    case State_PlayerTurn_O: {
        g->board[row][col] = Player_O;
        g->state = State_PlayerTurn_X;
    } break;

    case State_GameOver_X_won: // FALLTHROUGH
    case State_GameOver_O_won: // FALLTHROUGH
    case State_GameOver_Tie:   assert(0 && "TODO: return error: game is over");

    default: unreachable();
    }

    switch (Game_check_winner(g)) {
    case Player_NULL:                                  break;
    case Player_X:    g->state = State_GameOver_X_won; break;
    case Player_O:    g->state = State_GameOver_O_won; break;
    default: unreachable();
    }
}

Player Game_check_winner(Game* g)
{
    // Horizontally
    for (size_t r = 0; r < 3; ++r) {
        if (g->board[r][0] != Player_NULL && g->board[r][0] == g->board[r][1]
                                          && g->board[r][0] == g->board[r][2]) {
            return g->board[r][0];
        }
    }

    // Vertically
    for (size_t c = 0; c < 3; ++c) {
        if (g->board[0][c] != Player_NULL && g->board[0][c] == g->board[1][c]
                                          && g->board[0][c] == g->board[2][c]) {
            return g->board[0][c];
        }
    }

    // Diagonally: top-left - bottom-right
    if (g->board[0][0] != Player_NULL && g->board[0][0] == g->board[1][1]
                                      && g->board[0][0] == g->board[2][2]) {
        return g->board[0][0];
    }

    // Diagonally: top-right - bottom-left
    if (g->board[0][2] != Player_NULL && g->board[0][2] == g->board[1][1]
                                      && g->board[0][2] == g->board[2][0]) {
        return g->board[0][2];
    }

    return Player_NULL;
}

void Game_print(Game* g)
{
    switch (g->state) {
    case State_PlayerTurn_X:   puts("X turn"); break;
    case State_PlayerTurn_O:   puts("O turn"); break;
    case State_GameOver_X_won: puts("X won");  break;
    case State_GameOver_O_won: puts("O won");  break;
    case State_GameOver_Tie:   puts("Tie");    break;
    default: unreachable();
    }

    for (size_t r = 0; r < 3; ++r) {
        for (size_t c = 0; c < 3; ++c) {
            printf("[%c]", g->board[r][c]);
        }
        putchar('\n');
    }
}

int main(void)
{
    Game g;
    Game* gp = &g;

    Game_init(gp);
    Game_print(gp);
    Game_play(gp, 1, 1);
    Game_print(gp);

    return 0;
}
