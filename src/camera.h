#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vec3.h"

/* Camera for controlling viewport and ray generation */
typedef struct {
    vec3_t origin;
    vec3_t lower_left_corner;
    vec3_t horizontal;
    vec3_t vertical;
    vec3_t u, v, w;
    double lens_radius;
} camera_t;

/* Create a camera with look-at and field of view
 * vfov: vertical field of view in degrees
 * aspect_ratio: width / height
 * aperture: diameter of lens (0 = no defocus blur)
 * focus_dist: distance to plane of perfect focus
 */
camera_t camera_create(vec3_t lookfrom, vec3_t lookat, vec3_t vup, double vfov,
                       double aspect_ratio, double aperture, double focus_dist);

/* Generate a ray through the camera at (u, v) with optional random offset */
ray_t camera_get_ray(const camera_t *cam, double u, double v);

#endif /* CAMERA_H */
