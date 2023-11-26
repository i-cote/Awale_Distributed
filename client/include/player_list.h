#ifndef PLAYER_LIST_H
#define PLAYER_LIST_H

#include <stdlib.h>

// Linked list of player name
struct player_list_node {
    struct player_list_node* next;
    char* name;
};

struct player_list {
    struct player_list_node* first;
    size_t size;
};

#define PLAYER_FOREACH(curr, list) for (struct player_list_node* curr = list->first; curr != NULL; curr = curr->next)

struct player_list* player_list_create();

void player_list_add(struct player_list* list, const char* name);

void player_list_remove(struct player_list* list, const char* name);

void player_list_clear(struct player_list* list);

void player_list_free(struct player_list* list);

// TODO: Linked list method and create .c file
#endif
