#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct node {
    struct node* next;
    char element[256];
};

struct node* create(char* str) {
    struct node* new_node = malloc(sizeof (struct node));
    new_node->next = NULL;
    memcpy(new_node->element, str, strnlen_s(str, 255)+1);
    return new_node;
}

void display(struct node* entry) {
    struct node* cur = entry;
    while (cur != NULL) {
        printf("[%s] ", cur->element);
        cur = cur->next;
    }
    printf("\n");
}

struct node* insert(struct node* list, char* string) {
    struct node* new_node = create(string);
    if (list == NULL) {
        return new_node;
    }
    if (strcmp(list->element, string)>0) {
        new_node->next = list;
        return new_node;
    }
    else {
        struct node* cur_node = list;
        struct node* next_node = list->next;
        while (next_node != NULL && strcmp(next_node->element, string) <= 0) {
            cur_node = next_node;
            next_node = next_node->next;
        }
        cur_node->next = new_node;
        new_node->next = next_node;
        return list;
    }
}

struct node* destroy (struct node* list, char* str) {
    // Je ne comprends pas ce qu'on doit faire ? Supprimer noeud de la liste, ils sont déjà triée donc juste :
    if (list == NULL) {
        return NULL;
    }
    else {
        struct node* previous_node = NULL;
        struct node* cur_node = list;
        struct node* next_node = list->next;
        while (next_node != NULL && strcmp(cur_node->element, str) != 0) {
            previous_node = cur_node;
            cur_node = next_node;
            next_node = next_node->next;
        }
        if (previous_node == NULL) {
            // This mean we want to delete first node
            struct node *new_start = list->next;
            free(list);
            return new_start;
        }
        if (next_node == NULL && strcmp(cur_node->element, str) != 0) {
            // We didn't found any matching node
            return NULL;
        }
        // We delete the cur_node and make the junction
        free(cur_node);
        previous_node->next = next_node;
        return list;
    }
}

int main(int argc, char** argv) {
    printf("CI4!\n");
    struct node* list = insert(NULL, "que");
    list = insert(list, "la");
    list = insert(list, "force");
    list = insert(list, "soit");
    list = insert(list, "avec");
    list = insert(list, "toi");

    display(list);

    return 0;
}
