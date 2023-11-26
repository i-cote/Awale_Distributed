#include "player_list.h"
#include <stdlib.h>
#include <string.h>

static void free_node(struct player_list_node* node) {
    free(node->name);
    free(node);
}
struct player_list* player_list_create() {
    struct player_list* list = malloc(sizeof(struct player_list));
    list->size = 0;
    list->first = NULL;
    return list;
}

void player_list_add(struct player_list* list, const char* name) {
    size_t name_len = strlen(name);

    struct player_list_node* node = malloc(sizeof(struct player_list_node));
    node->name = malloc(name_len + 1);
    strcpy(node->name, name);

    node->next = list->first;

    list->first = node;
}

void player_list_remove(struct player_list* list, const char* name) {

    while (list->first != NULL && strcmp(list->first->name, name) == 0) {
        struct player_list_node* new_first = list->first->next;
        free_node(list->first);
        list->first = new_first;
    }

    if (list->first == NULL) {
        return;
    }

    struct player_list_node* last = list->first;
    struct player_list_node* curr = list->first->next;
    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            last->next = curr->next;
            free_node(curr);
            curr = last->next;
        } else {
            last = curr;
            curr = curr->next;
        }
    }
}

void player_list_clear(struct player_list* list) {
    for (struct player_list_node* curr = list->first; curr != NULL; ) {
        struct player_list_node* to_free = curr;
        curr = curr->next;
        free_node(to_free);
    }
    list->first = NULL;
}
void player_list_free(struct player_list* list) {
    player_list_clear(list);
    free(list);
}
