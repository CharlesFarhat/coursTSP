#include <stdio.h>
#include <malloc.h>

struct node {
    int key;
    void *val;
    struct node *left;
    struct node *right;
};


struct map {
    struct node *root;
};


struct map *map_init() {
    struct map *map = malloc(sizeof(struct map));
    map->root = NULL;
    return map;
}

void free_node(struct node *n) {
    if (!n) {
        return;
    }
    free_node(n->left);
    free_node(n->right);
    free(n);
}

void map_free(struct map *map) {
    if (map->root)
        free_node(map->root);
    free(map);
}

static struct node* map_find_key(struct node* node, int key) {
    if (!node)
        return NULL;
    if (node->key == key)
        return node;
    if (node->key<key)
        return map_find_key(node->right, key);
    return map_find_key(node->left, key);
}

struct node* new_node(int key, void* val) {
    struct node* node = malloc(sizeof(struct node));
    node->left=NULL;
    node->right=NULL;
    node->key=key;
    node->val=val;
    return node;
}

/* Insert a node in a tree*/
static struct node* insert_node(struct node* tree, struct node* n) {
    if (!tree)
        return n;
    if (tree->key < n->key)
        tree->right = insert_node(tree->right, n);
    else
        tree->left = insert_node(tree->left, n);
    return tree;
}

void map_put(struct map *map, int key, void *val) {
    struct node* n = map_find_key(map, key);
    if (!n) {
        n = new_node(key, val);
        map->root = insert_node(map->root, n);
    }
    else {
        n->val = val;
    }
}

void *map_get(struct map *map, int key) {
    struct node*n = map_find_key(map->root, key);
    if(n)
        return n->val;
    return NULL;
}


