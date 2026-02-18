#include "hittable.h"
#include <stdlib.h>

#define INITIAL_CAPACITY 10

/* Create an empty hittable list */
hittable_list_t *hittable_list_create(void) {
    hittable_list_t *list = malloc(sizeof(hittable_list_t));
    if (!list) return NULL;

    list->capacity = INITIAL_CAPACITY;
    list->count = 0;
    list->objects = malloc(list->capacity * sizeof(hittable_t));
    if (!list->objects) {
        free(list);
        return NULL;
    }
    return list;
}

/* Add an object to the list */
void hittable_list_add(hittable_list_t *list, hittable_t object) {
    if (!list) return;

    if (list->count >= list->capacity) {
        list->capacity *= 2;
        hittable_t *new_objects =
            realloc(list->objects, list->capacity * sizeof(hittable_t));
        if (new_objects) {
            list->objects = new_objects;
        }
    }
    list->objects[list->count++] = object;
}

/* Find closest intersection with any object */
int hittable_list_hit(const hittable_list_t *list, const ray_t r, double t_min,
                      double t_max, hit_record_t *rec) {
    if (!list) return 0;

    hit_record_t temp_rec = {0};
    int hit_anything = 0;
    double closest_so_far = t_max;

    for (int i = 0; i < list->count; i++) {
        hittable_t *obj = &list->objects[i];
        if (obj->hit(obj->data, r, t_min, closest_so_far, &temp_rec)) {
            hit_anything = 1;
            closest_so_far = temp_rec.t;
            *rec = temp_rec;
        }
    }

    return hit_anything;
}

/* Free all objects and the list */
void hittable_list_destroy(hittable_list_t *list) {
    if (!list) return;

    for (int i = 0; i < list->count; i++) {
        if (list->objects[i].destroy) {
            list->objects[i].destroy(list->objects[i].data);
        }
    }
    free(list->objects);
    free(list);
}
