
#ifndef GAME_H
#define GAME_H

typedef enum {
    GAME_ONGOING = 0,
    GAME_PLAYER0_WIN = 1,
    GAME_PLAYER1_WIN = 2,
    GAME_DRAW = 3,
    INVALID = 4
} GameResult;

typedef struct {
    int houses[12];
    int scores[2];
    int current_player;
    GameResult status;
    int move_count;
} Game;

// Public functions
void game_init(Game *game, int starting_player);
int is_valid_move(const Game *game, int house);
GameResult make_move(Game *game, int house);
GameResult game_check_end(const Game *game);
void print_board(const Game *game);

#endif
