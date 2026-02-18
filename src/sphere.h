#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

/* Sphere object */
typedef struct {
    vec3_t center;
    double radius;
    const material_t *material;
} sphere_t;

/* Create a sphere */
sphere_t *sphere_create(const vec3_t center, double radius,
                        const material_t *material);

/* Create a hittable sphere object */
hittable_t sphere_to_hittable(sphere_t *sphere);

#endif /* SPHERE_H */
