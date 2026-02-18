#ifndef HITTABLE_H
#define HITTABLE_H

#include "vec3.h"
#include "ray.h"

/* Forward declarations */
typedef struct material material_t;

/* Record of a ray-object intersection */
typedef struct hit_record {
    vec3_t point;
    vec3_t normal;
    double t;
    int front_face;
    const material_t *material;
} hit_record_t;

/* Set front face and adjust normal based on ray direction */
static inline void set_face_normal(hit_record_t *rec, const ray_t r,
                                   const vec3_t outward_normal) {
    rec->front_face = vec3_dot(r.direction, outward_normal) < 0;
    rec->normal = rec->front_face ? outward_normal
                                   : vec3_mul(outward_normal, -1.0);
}

/* Generic hittable object interface */
typedef struct {
    void *data;
    int (*hit)(const void *obj, const ray_t r, double t_min, double t_max,
               hit_record_t *rec);
    void (*destroy)(void *obj);
} hittable_t;

/* List of hittable objects */
typedef struct {
    hittable_t *objects;
    int count;
    int capacity;
} hittable_list_t;

/* Create an empty hittable list */
hittable_list_t *hittable_list_create(void);

/* Add an object to the list */
void hittable_list_add(hittable_list_t *list, hittable_t object);

/* Find closest intersection with any object */
int hittable_list_hit(const hittable_list_t *list, const ray_t r, double t_min,
                      double t_max, hit_record_t *rec);

/* Free all objects and the list */
void hittable_list_destroy(hittable_list_t *list);

#endif /* HITTABLE_H */
