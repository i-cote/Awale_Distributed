#include "include/test_game.h"
#include "../src/game_types.h"

bool is_starved_opponent(struct board * b);

bool test_is_starved_opponent_player_one_should_return_true() {
	struct board b = {
	.holes = {2,5,0,4,0,7,0,0,0,0,0,0},
	.to_play = PLAYER1
	};
	return (is_starved_opponent(&b) == true);
}

bool test_is_starved_opponent_player_one_should_return_false() {
	struct board b = {
	.holes = {2,5,0,4,0,7,0,0,0,0,0,1},
	.to_play = PLAYER1
	};
	return (is_starved_opponent(&b) == false);
}

bool test_is_starved_opponent_player_two_should_return_true() {
	struct board b = {
	.holes = {0,0,0,0,0,0,4,0,9,0,0,1},
	.to_play = PLAYER2
	};
	return (is_starved_opponent(&b) == true);
}

bool test_is_starved_opponent_player_two_should_return_false() {
	struct board b = {
	.holes = {0,0,0,0,0,7,0,3,9,0,7,1},
	.to_play = PLAYER2
	};
	return (is_starved_opponent(&b) == false);
}
