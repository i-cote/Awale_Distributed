#include "include/test_game.h"
#include <stdio.h>

int main() {

	char * test_status;
	char * passed = "\033[1;32mPassed\033[0m\n";
	char * failed = "\033[1;31mFailed\033[0m\n";

	bool all_tests_pass = true;

	test_status = test_is_starved_opponent_player_one_should_return_true() ? passed : failed;
	printf("test_is_starved_opponent_player_one_should_return_true\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_is_starved_opponent_player_one_should_return_false() ? passed : failed;
	printf("test_is_starved_opponent_player_one_should_return_false\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_is_starved_opponent_player_two_should_return_true() ? passed : failed;
	printf("test_is_starved_opponent_player_two_should_return_true\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_is_starved_opponent_player_two_should_return_false() ? passed : failed;
	printf("test_is_starved_opponent_player_two_should_return_false\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_exists_move_that_feeds_opponent_player_one_should_return_true() ? passed : failed;
	printf("test_exists_move_that_feeds_opponent_player_one_should_return_true\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_exists_move_that_feeds_opponent_player_one_should_return_false() ? passed : failed;
	printf("test_exists_move_that_feeds_opponent_player_one_should_return_false\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_exists_move_that_feeds_opponent_player_two_should_return_true() ? passed : failed;
	printf("test_exists_move_that_feeds_opponent_player_two_should_return_true\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_exists_move_that_feeds_opponent_player_two_should_return_false() ? passed : failed;
	printf("test_exists_move_that_feeds_opponent_player_two_should_return_false\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_is_opponent_hole_player_one_should_return_true() ? passed : failed;
	printf("test_is_opponent_hole_player_one_should_return_true\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_is_opponent_hole_player_one_should_return_false() ? passed : failed;
	printf("test_is_opponent_hole_player_one_should_return_false\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_is_opponent_hole_player_two_should_return_true() ? passed : failed;
	printf("test_is_opponent_hole_player_two_should_return_true\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_is_opponent_hole_player_two_should_return_false() ? passed : failed;
	printf("test_is_opponent_hole_player_two_should_return_false\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_is_starved_self_player_one_should_return_true() ? passed : failed;
	printf("test_is_starved_self_player_one_should_return_true\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_is_starved_self_player_one_should_return_false() ? passed : failed;
	printf("test_is_starved_self_player_one_should_return_false\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_is_starved_self_player_two_should_return_true() ? passed : failed;
	printf("test_is_starved_self_player_two_should_return_true\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_is_starved_self_player_two_should_return_false() ? passed : failed;
	printf("test_is_starved_self_player_two_should_return_false\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_opponent_captures_its_balls_player_one() ? passed : failed;
	printf("test_opponent_captures_its_balls_player_one\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_opponent_captures_its_balls_player_two() ? passed : failed;
	printf("test_opponent_captures_its_balls_player_two\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_update_board_normal_turn_with_capture_should_return_NEXT_PLAY() ? passed : failed;
	printf("test_update_board_normal_turn_with_capture_should_return_NEXT_PLAY\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_update_board_user_selected_empty_hole_should_return_INVALID_MOVE() ? passed : failed;
	printf("test_update_board_user_selected_empty_hole_should_return_INVALID_MOVE\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;

	test_status = test_update_board_player_ends_turn_with_25_or_plus_should_return_GAME_ENDED() ? passed : failed;
	printf("test_update_board_player_ends_turn_with_25_or_plus_should_return_GAME_ENDED\t%s",
			test_status);
	if(test_status == failed)
		all_tests_pass = false;



	if(all_tests_pass)
		printf("\n\033[1;32mAll tests are passing!\033[0m\n");
	else
		printf("\n\033[1;31mSome tests are failing!\033[0m\n");

}
