#ifndef MAP_H
#define MAP_H

/* a hashmap.
 * the exact content of the structure may depend on the implementation
 */
struct map {
    struct couple* couples;
    int nb_couples;
};

/* default couple struct implementation */
struct couple {
    int key;
    void* val;
};

/* initialize a hashmap */
struct map* map_init();

/* free a map and all the stored values */
void map_free(struct map* map);

/* insert the couple (key, val) */
void map_put(struct map* map, int key, void* val);

/* return the val associated with key */
void* map_get(struct map* map, int key);

#endif /* MAP_H */
