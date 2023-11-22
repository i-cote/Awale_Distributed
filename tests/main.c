#include "include/test_game.h"
#include <stdio.h>

int main() {

	char * test_status;
	char * passed = "\033[1;32mPassed\033[0m\n";
	char * failed = "\033[1;31mFailed\033[0m\n";

	test_status = test_is_starved_opponent_player_one_should_return_true() ? passed : failed;
	printf("test_is_starved_opponent_player_one_should_return_true\t%s",
			test_status);


	test_status = test_is_starved_opponent_player_one_should_return_false() ? passed : failed;
	printf("test_is_starved_opponent_player_one_should_return_false\t%s",
			test_status);

	test_status = test_is_starved_opponent_player_two_should_return_true() ? passed : failed;
	printf("test_is_starved_opponent_player_two_should_return_true\t%s",
			test_status);

	test_status = test_is_starved_opponent_player_two_should_return_false() ? passed : failed;
	printf("test_is_starved_opponent_player_two_should_return_false\t%s",
			test_status);

	test_status = test_exists_move_that_feeds_opponent_player_one_should_return_true() ? passed : failed;
	printf("test_exists_move_that_feeds_opponent_player_one_should_return_true\t%s",
			test_status);

	test_status = test_exists_move_that_feeds_opponent_player_one_should_return_false() ? passed : failed;
	printf("test_exists_move_that_feeds_opponent_player_one_should_return_false\t%s",
			test_status);

	test_status = test_exists_move_that_feeds_opponent_player_two_should_return_true() ? passed : failed;
	printf("test_exists_move_that_feeds_opponent_player_two_should_return_true\t%s",
			test_status);

	test_status = test_exists_move_that_feeds_opponent_player_two_should_return_false() ? passed : failed;
	printf("test_exists_move_that_feeds_opponent_player_two_should_return_false\t%s",
			test_status);

	test_status = test_is_opponent_hole_player_one_should_return_true() ? passed : failed;
	printf("test_is_opponent_hole_player_one_should_return_true\t%s",
			test_status);

	test_status = test_is_opponent_hole_player_one_should_return_false() ? passed : failed;
	printf("test_is_opponent_hole_player_one_should_return_false\t%s",
			test_status);

	test_status = test_is_opponent_hole_player_two_should_return_true() ? passed : failed;
	printf("test_is_opponent_hole_player_two_should_return_true\t%s",
			test_status);

	test_status = test_is_opponent_hole_player_two_should_return_false() ? passed : failed;
	printf("test_is_opponent_hole_player_two_should_return_false\t%s",
			test_status);
}
