#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "vec3.h"

/* Forward declaration */
typedef struct hit_record hit_record_t;

/* Material scatter function pointer */
typedef int (*scatter_fn)(const void *mat, const ray_t r_in,
                          const hit_record_t *rec, vec3_t *attenuation,
                          ray_t *scattered);

/* Generic material */
typedef struct material {
    void *data;
    scatter_fn scatter;
    void (*destroy)(void *mat);
} material_t;

#endif /* MATERIAL_H */
