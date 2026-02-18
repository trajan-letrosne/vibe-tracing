#include "material.h"
#include "hittable.h"
#include "vec3.h"
#include <math.h>
#include <stdlib.h>

/* Lambertian (diffuse) material */
typedef struct {
    vec3_t albedo;
} lambertian_t;

static int lambertian_scatter(const void *mat, const ray_t r_in,
                              const hit_record_t *rec, vec3_t *attenuation,
                              ray_t *scattered) {
    (void)r_in;
    const lambertian_t *lamb = (const lambertian_t *)mat;
    *attenuation = lamb->albedo;

    vec3_t scatter_direction = vec3_add(rec->normal, random_unit_vector());
    if (vec3_length_squared(scatter_direction) < 1e-8) {
        scatter_direction = rec->normal;
    }
    *scattered = ray(rec->point, scatter_direction);
    return 1;
}

static void lambertian_destroy(void *mat) {
    free(mat);
}

material_t lambertian_create(const vec3_t albedo) {
    lambertian_t *lamb = malloc(sizeof(lambertian_t));
    if (!lamb) return (material_t){0};

    lamb->albedo = albedo;
    return (material_t){
        .data = lamb,
        .scatter = lambertian_scatter,
        .destroy = lambertian_destroy,
    };
}

/* Metal (reflective) material */
typedef struct {
    vec3_t albedo;
    double fuzz;
} metal_t;

static int metal_scatter(const void *mat, const ray_t r_in,
                         const hit_record_t *rec, vec3_t *attenuation,
                         ray_t *scattered) {
    const metal_t *metal = (const metal_t *)mat;
    vec3_t reflected =
        vec3_sub(r_in.direction,
                 vec3_mul(rec->normal, 2.0 * vec3_dot(r_in.direction, rec->normal)));
    reflected = vec3_normalize(reflected);

    vec3_t fuzz_vec = vec3_mul(random_in_unit_sphere(), metal->fuzz);
    *scattered = ray(rec->point, vec3_add(reflected, fuzz_vec));
    *attenuation = metal->albedo;
    return vec3_dot(scattered->direction, rec->normal) > 0;
}

static void metal_destroy(void *mat) {
    free(mat);
}

material_t metal_create(const vec3_t albedo, double fuzz) {
    metal_t *metal = malloc(sizeof(metal_t));
    if (!metal) return (material_t){0};

    metal->albedo = albedo;
    metal->fuzz = fuzz < 1.0 ? fuzz : 1.0;
    return (material_t){
        .data = metal,
        .scatter = metal_scatter,
        .destroy = metal_destroy,
    };
}

/* Dielectric (glass) material with Schlick approximation */
typedef struct {
    double ir; /* index of refraction */
} dielectric_t;

/* Schlick's approximation for reflectance */
static double reflectance(double cosine, double ref_idx) {
    double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * pow(1.0 - cosine, 5.0);
}

/* Refraction using Snell's law */
static vec3_t refract(const vec3_t uv, const vec3_t n, double etai_over_etat) {
    double cos_theta = fmin(-vec3_dot(uv, n), 1.0);
    vec3_t r_out_perp = vec3_mul(vec3_add(uv, vec3_mul(n, cos_theta)), etai_over_etat);
    double r_out_parallel_len_sq =
        1.0 - vec3_length_squared(r_out_perp);
    vec3_t r_out_parallel = vec3_mul(n, -sqrt(fabs(r_out_parallel_len_sq)));
    return vec3_add(r_out_perp, r_out_parallel);
}

static int dielectric_scatter(const void *mat, const ray_t r_in,
                              const hit_record_t *rec, vec3_t *attenuation,
                              ray_t *scattered) {
    const dielectric_t *diel = (const dielectric_t *)mat;
    *attenuation = vec3(1.0, 1.0, 1.0);

    double etai_over_etat =
        rec->front_face ? (1.0 / diel->ir) : diel->ir;

    vec3_t unit_direction = vec3_normalize(r_in.direction);
    double cos_theta = fmin(-vec3_dot(unit_direction, rec->normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    int cannot_refract = etai_over_etat * sin_theta > 1.0;
    vec3_t direction;

    if (cannot_refract ||
        reflectance(cos_theta, etai_over_etat) > random_double()) {
        direction = vec3_sub(unit_direction,
                            vec3_mul(rec->normal, 2.0 * vec3_dot(unit_direction, rec->normal)));
    } else {
        direction = refract(unit_direction, rec->normal, etai_over_etat);
    }

    *scattered = ray(rec->point, direction);
    return 1;
}

static void dielectric_destroy(void *mat) {
    free(mat);
}

material_t dielectric_create(double index_of_refraction) {
    dielectric_t *diel = malloc(sizeof(dielectric_t));
    if (!diel) return (material_t){0};

    diel->ir = index_of_refraction;
    return (material_t){
        .data = diel,
        .scatter = dielectric_scatter,
        .destroy = dielectric_destroy,
    };
}
