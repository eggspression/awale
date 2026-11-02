#include <stdio.h>

typedef enum {
    GAME_ONGOING = 0,
    GAME_PLAYER0_WIN = 1,
    GAME_PLAYER1_WIN = 2,
    GAME_DRAW = 3,
    INVALID = 4
} GameResult;

typedef struct game{
    int houses[12];
    int scores[2];
    int current_player;
    GameResult status;
    int winner; //player 0 ,1. 2 if draw
    int move_count;
} Game;

void game_init(Game *game, int starting_player){
    for (int i = 0; i < 12; i++) {
        game->houses[i] = 4;
    }
    for (int i = 0; i < 2; i++) {
        game->scores[i] = 0;
    }
    game->winner = 0;
    game->status = GAME_ONGOING;
    game->move_count = 0;
    game->current_player = starting_player;
}

int is_valid_move(const Game *game, int house){
    int flag = 1;

    if (house / 6 != game->current_player){
        flag = 0;
    }
    if (game->houses[house] == 0) {
        flag = 0;
    }
    int a = 1, b = 1;
    for (int i = 0; i < 6 && (a || b); i++) {
        a &= (game->houses[i] == 0);
        b &= (game->houses[i + 6] == 0);
    }
    if (a || b) {
        if (a) {
            if (game->houses[house] + house - 11 > 0) {
                flag = 0;
            }  
        }
        if (b) {
            if (game->houses[house] + house - 5 > 0) {
                flag = 0;
            }
        }
    }
    return flag;
}
int count_side_seeds(const Game *game, int player) {
    int total = 0;
    for (int i = 0; i < 6; i++) {
        total += game->houses[player*6 +i];
    }
    return total;
}
int sow_seeds(Game *game, int house){
    int last = 0;
    int pos = 0;
    int seeds = game->houses[house];
    game->houses[house] = 0;
    for (int i = 0; i < seeds; i++) {
        pos = (house + i) % 12;
        if (pos == house) continue;
        game->houses[pos] += 1;
        seeds--;
    }
    return pos;
}

int capture(Game *game, int last){
    int potential_capture = 0;
    int pos = last;
    int opponent = 1 - game->current_player;
    while (pos >= 0 && pos/6 == opponent && (game->houses[pos] == 2 || game->houses[pos] == 3)){
        potential_capture += game->houses[pos];
        pos--;
        }
    if (potential_capture == count_side_seeds(game, opponent)){
        return 0;
    }
    for (int i = pos + 1; i <= last; i++) {
        game->houses[i] = 0;
    }

    return potential_capture;
}
GameResult game_check_end(const Game *game){
    int opponent = 1 - game->current_player;
    GameResult result = GAME_ONGOING;
    if (game->scores[0] >= 25) {
        result = GAME_PLAYER0_WIN;
    }
    if (game->scores[1] >= 25) {
        result = GAME_PLAYER1_WIN;
    }
    if (game->scores[0] == 24 && game->scores[1] == 24) {
        result = GAME_DRAW;
    }
    if (count_side_seeds(game, opponent) == 0) {
        int flag = 0;
        int boundary = (game->current_player) ? 6 : 12;
        int pos = game->current_player*6;
        for (int i = pos; i < pos + 6; i++) {
            if (game->houses[i]> 0 && i + game->houses[i] >= boundary) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            int final0 = game->scores[0] + count_side_seeds(game, 0);
            int final1 = game->scores[1] + count_side_seeds(game, 1);

            if (final0 > final1) return GAME_PLAYER0_WIN;
            if (final1 > final0) return GAME_PLAYER1_WIN;
            return GAME_DRAW;
        }
    }
    return result;
}
GameResult make_move(Game *game, int house) {
    int valid = is_valid_move(game, house);
    if (!valid) {
        return INVALID;
    }
    int last = sow_seeds(game, house);
    int points = capture(game, last);
    game->scores[game->current_player] += points;
    GameResult result = game_check_end(game);
    if (result == GAME_ONGOING) {
        game->current_player = 1 - game->current_player;
        game->move_count++;
    }
    return result;


}
void print_board(Game *game) {
    for (int i = 0; i < 12; i++) {
        printf("%d ", game->houses[i]);
        if (i == 5) {
            printf("\n");
        }
    }
}

int main(void) {
    Game game;
    game_init(&game, 0);

    while (game.status == GAME_ONGOING) {
        print_board(&game);

        int house;
        printf("\nPlayer %d, choose house (-1 to quit): ",
               game.current_player);

        if (scanf("%d", &house) != 1 || house == -1)
            break;

        GameResult result = make_move(&game, house);

        if (result == INVALID) {
            printf("Invalid move! Try again.\n");
            continue;
        }

        if (result != GAME_ONGOING) {
            print_board(&game);

            if (result == GAME_DRAW)
                printf("\nDRAW!\n");
            else
                printf("\nPLAYER %d WINS!\n",
                       result == GAME_PLAYER0_WIN ? 0 : 1);
        }
    }

    return 0;
}

