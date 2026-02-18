#include "sphere.h"
#include <math.h>
#include <stdlib.h>

/* Ray-sphere intersection detection */
static int sphere_hit(const void *obj, const ray_t r, double t_min, double t_max,
                      hit_record_t *rec) {
    const sphere_t *sphere = (const sphere_t *)obj;
    vec3_t oc = vec3_sub(r.origin, sphere->center);
    double a = vec3_length_squared(r.direction);
    double half_b = vec3_dot(oc, r.direction);
    double c = vec3_length_squared(oc) - sphere->radius * sphere->radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
        return 0;
    }

    double sqrt_discriminant = sqrt(discriminant);
    double t = (-half_b - sqrt_discriminant) / a;

    if (t < t_min || t_max < t) {
        t = (-half_b + sqrt_discriminant) / a;
        if (t < t_min || t_max < t) {
            return 0;
        }
    }

    rec->t = t;
    rec->point = ray_at(r, t);
    vec3_t outward_normal =
        vec3_div(vec3_sub(rec->point, sphere->center), sphere->radius);
    set_face_normal(rec, r, outward_normal);
    rec->material = sphere->material;

    return 1;
}

/* Destroy sphere object */
static void sphere_destroy(void *obj) {
    free(obj);
}

/* Create a sphere */
sphere_t *sphere_create(const vec3_t center, double radius,
                        const material_t *material) {
    sphere_t *sphere = malloc(sizeof(sphere_t));
    if (!sphere) return NULL;

    sphere->center = center;
    sphere->radius = radius;
    sphere->material = material;
    return sphere;
}

/* Create a hittable sphere object */
hittable_t sphere_to_hittable(sphere_t *sphere) {
    return (hittable_t){.data = sphere, .hit = sphere_hit, .destroy = sphere_destroy};
}
