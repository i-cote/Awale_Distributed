#include <stdbool.h>
#include <string.h>

#include "game_types.h"

bool is_opponent_hole(uint32_t hole, enum player self) {
	return ((hole/NB_HOLES_PLAYER) != self);
}

bool exists_move_that_feeds_opponent(struct board * b) {

	bool move_exists = false;
	for(int i = NB_HOLES_PLAYER * b->to_play;
			i < NB_HOLES_PLAYER*(b->to_play + 1);
			i++) {
		if(is_opponent_hole(((i+b->holes[i])%(NB_HOLES_PLAYER*2)), b->to_play)) {
			move_exists = true;
			break;
		}
	}
	return move_exists;
}

void opponent_captures_its_balls(struct board * b) {

	for(int i = NB_HOLES_PLAYER * OPPONENT(b->to_play);
			i < NB_HOLES_PLAYER*(OPPONENT(b->to_play) + 1);
			i++) {
		b->points[OPPONENT(b->to_play)] += b->holes[i];
		b->holes[i] = 0;
	}
}

bool is_starved_opponent(struct board * b) {

	uint32_t sum = 0;

	for(int i = NB_HOLES_PLAYER * OPPONENT(b->to_play);
			i < NB_HOLES_PLAYER*(OPPONENT(b->to_play) + 1);
			i++)
		sum += b->holes[i];

	return !sum;
}

bool is_starved_self(struct board * b) {

	uint32_t sum = 0;

	for(int i = NB_HOLES_PLAYER * b->to_play;
			i < NB_HOLES_PLAYER*(b->to_play + 1);
			i++)
		sum += b->holes[i];

	return !sum;
}

enum board_status update_board(struct board * final_board, uint32_t move) {

	struct board temp_board;
	memcpy(&temp_board,final_board,sizeof(struct board));
	struct board * b = &temp_board;

	bool was_starved_opponent = is_starved_opponent(b);

	bool player_won = false;

	if(move >= NB_HOLES_PLAYER)
		return INVALID_MOVE;

	uint32_t hole = NB_HOLES_PLAYER * b->to_play + move;

	uint32_t nb_balls = b->holes[hole];

	if(nb_balls == 0) {
		if(!is_starved_self(b))
			return INVALID_MOVE;
		opponent_captures_its_balls(b);
		memcpy(final_board,&temp_board,sizeof(struct board));
		return GAME_ENDED;
	}


	b->holes[hole] = 0;

	bool capture = false;


	while(true) {
		hole = (hole+1)%(NB_HOLES_PLAYER*2);
		if(hole == NB_HOLES_PLAYER * b->to_play + move)
			continue;
		b->holes[hole]++;
		nb_balls--;
		if(nb_balls == 0) {
			if(is_opponent_hole(hole,b->to_play) && (b->holes[hole] == 2 || b->holes[hole] == 3))	
				capture = true;
			break;
		}
	}


	while(capture) {
		b->points[b->to_play] += b->holes[hole];	
		if(b->points[b->to_play] >= WINNING_POINTS)
			player_won = true;
		b->holes[hole] = 0;
		hole = (hole-1)%(NB_HOLES_PLAYER*2);
		if(!is_opponent_hole(hole, b->to_play) || !(b->holes[hole]==2 || b->holes[hole]==3))
			capture = false;
	}


	if(!was_starved_opponent && is_starved_opponent(b))
		return INVALID_MOVE;

	else if(was_starved_opponent && is_starved_opponent(b)) {

		if(!exists_move_that_feeds_opponent(final_board))
			return GAME_ENDED;
		else
			return INVALID_MOVE;

	}


	memcpy(final_board, &temp_board, sizeof(struct board));

	if(player_won)
		return GAME_ENDED;

	b->to_play = OPPONENT(b->to_play);

	return NEXT_PLAY;
}
