#include "include/test_game.h"
#include "../src/game_types.h"

bool is_starved_opponent(struct board * b);

bool test_is_starved_opponent_player_one_should_return_true() {
	struct board b;
	b.holes[0] = 2;
	b.holes[1] = 5;
	b.holes[2] = 0;
	b.holes[3] = 4;
	b.holes[4] = 0;
	b.holes[5] = 7;
	b.holes[6] = 0;
	b.holes[7] = 0;
	b.holes[8] = 0;
	b.holes[9] = 0;
	b.holes[10] = 0;
	b.holes[11] = 0;
	b.to_play = PLAYER1;
	return (is_starved_opponent(&b) == true);
}

bool test_is_starved_opponent_player_one_should_return_false() {
	struct board b;
	b.holes[0] = 2;
	b.holes[1] = 5;
	b.holes[2] = 0;
	b.holes[3] = 4;
	b.holes[4] = 0;
	b.holes[5] = 7;
	b.holes[6] = 0;
	b.holes[7] = 0;
	b.holes[8] = 0;
	b.holes[9] = 0;
	b.holes[10] = 0;
	b.holes[11] = 1;
	b.to_play = PLAYER1;
	return (is_starved_opponent(&b) == false);
}

bool test_is_starved_opponent_player_two_should_return_true() {
	struct board b;
	b.holes[0] = 0;
	b.holes[1] = 0;
	b.holes[2] = 0;
	b.holes[3] = 0;
	b.holes[4] = 0;
	b.holes[5] = 0;
	b.holes[6] = 4;
	b.holes[7] = 0;
	b.holes[8] = 9;
	b.holes[9] = 0;
	b.holes[10] = 0;
	b.holes[11] = 1;
	b.to_play = PLAYER2;
	return (is_starved_opponent(&b) == true);
}

bool test_is_starved_opponent_player_two_should_return_false() {
	struct board b;
	b.holes[0] = 0;
	b.holes[1] = 0;
	b.holes[2] = 0;
	b.holes[3] = 0;
	b.holes[4] = 0;
	b.holes[5] = 7;
	b.holes[6] = 0;
	b.holes[7] = 3;
	b.holes[8] = 9;
	b.holes[9] = 0;
	b.holes[10] = 7;
	b.holes[11] = 1;
	b.to_play = PLAYER2;
	return (is_starved_opponent(&b) == false);
}
