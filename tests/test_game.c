#include "include/test_game.h"
#include "../src/game_types.h"

bool is_starved_opponent(struct board * b);
bool exists_move_that_feeds_opponent(struct board * b);
bool is_opponent_hole(uint32_t hole, enum player self);

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

bool test_exists_move_that_feeds_opponent_player_one_should_return_true() {
	struct board b = {
	.holes = {5,0,3,0,2,0,0,0,0,0,0,0},
	.to_play = PLAYER1
	};
	return (exists_move_that_feeds_opponent(&b) == true);
}

bool test_exists_move_that_feeds_opponent_player_one_should_return_false() {
	struct board b = {
	.holes = {5,0,3,0,1,0,0,0,0,0,0,0},
	.to_play = PLAYER1
	};
	return (exists_move_that_feeds_opponent(&b) == false);
}

bool test_exists_move_that_feeds_opponent_player_two_should_return_true() {
	struct board b = {
	.holes = {0,0,0,0,0,0,1,0,4,0,0,0},
	.to_play = PLAYER2
	};
	return (exists_move_that_feeds_opponent(&b) == true);
}

bool test_exists_move_that_feeds_opponent_player_two_should_return_false() {
	struct board b = {
	.holes = {0,0,0,0,0,0,1,0,3,0,1,0},
	.to_play = PLAYER2
	};
	return (exists_move_that_feeds_opponent(&b) == false);
}

bool test_is_opponent_hole_player_one_should_return_true() {
	enum player p = PLAYER1;
	bool passed = true;
	for(int i = 6; i < 12; i++) {
		if(is_opponent_hole(i,p) != true)
			passed = false;
	}
	return passed;
}

bool test_is_opponent_hole_player_one_should_return_false() {
	enum player p = PLAYER1;
	bool passed = true;
	for(int i = 0; i < 6; i++) {
		if(is_opponent_hole(i,p) != false)
			passed = false;
	}
	return passed;
}

bool test_is_opponent_hole_player_two_should_return_true() {
	enum player p = PLAYER2;
	bool passed = true;
	for(int i = 0; i < 6; i++) {
		if(is_opponent_hole(i,p) != true)
			passed = false;
	}
	return passed;
}

bool test_is_opponent_hole_player_two_should_return_false() {
	enum player p = PLAYER2;
	bool passed = true;
	for(int i = 6; i < 12; i++) {
		if(is_opponent_hole(i,p) != false)
			passed = false;
	}
	return passed;
}
