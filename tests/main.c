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


	if(all_tests_pass)
		printf("\n\033[1;32mAll tests are passing!\033[0m\n");
	else
		printf("\n\033[1;31mSome tests are failing!\033[0m\n");

}
