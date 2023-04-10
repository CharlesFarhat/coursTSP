#include <stdio.h>
#include <malloc.h>
#include "map.h"

struct map* map_init() {
    struct map* our_map = malloc(sizeof(struct map));
    our_map->couples = NULL;
    our_map->nb_couples=0;
    return our_map;
}


void map_free(struct map* map) {
    if (map->nb_couples)
        free(map->couples);
    free(map);
}

void map_put(struct map* map, int key, void* val) {
    int already_in = 0;
    for (int i = 0; i < map->nb_couples; i++) {
        if (map->couples[i].key == key) {
            already_in = 1;
        }
    }
    if (!already_in) {
        map->nb_couples++;
        map->couples = realloc(map->couples, map->nb_couples * sizeof(struct couple));
        map->couples[map->nb_couples - 1].key = key;
        map->couples[map->nb_couples - 1].val = val;
    }
}

void* map_get(struct map* map, int key) {
    for (int i = 0; i < map->nb_couples; i++) {
        if (map->couples[i].key == key) {
            return map->couples[i].val;
        }
    }
    return NULL;
}


