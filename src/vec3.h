#ifndef VEC3_H
#define VEC3_H

#include <math.h>

/* 3D vector type for points, colors, and directions */
typedef struct {
    double e[3];
} vec3_t;

/* Basic vector operations */
vec3_t vec3_add(const vec3_t a, const vec3_t b);
vec3_t vec3_sub(const vec3_t a, const vec3_t b);
vec3_t vec3_mul(const vec3_t v, double t);
vec3_t vec3_div(const vec3_t v, double t);

/* Vector arithmetic */
double vec3_dot(const vec3_t a, const vec3_t b);
vec3_t vec3_cross(const vec3_t a, const vec3_t b);
double vec3_length(const vec3_t v);
double vec3_length_squared(const vec3_t v);
vec3_t vec3_normalize(const vec3_t v);

/* Utility constructors */
vec3_t vec3(double x, double y, double z);

/* Random utilities */
double random_double(void);
double random_double_range(double min, double max);
vec3_t random_vec3(void);
vec3_t random_vec3_range(double min, double max);
vec3_t random_unit_vector(void);
vec3_t random_in_unit_sphere(void);

#endif /* VEC3_H */
