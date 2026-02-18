#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "sphere.h"
#include "camera.h"
#include <stdio.h>
#include <stdlib.h>

/* Image dimensions */
#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256
#define SAMPLES_PER_PIXEL 100

/* Construct a ray from origin and direction */
ray_t ray(const vec3_t origin, const vec3_t direction) {
    return (ray_t){origin, direction};
}

/* Get point at parameter t along the ray */
vec3_t ray_at(const ray_t r, double t) {
    return vec3_add(r.origin, vec3_mul(r.direction, t));
}

/* Calculate color based on ray-scene intersection */
static vec3_t ray_color(const ray_t r, const hittable_list_t *world) {
    hit_record_t rec = {0};
    if (hittable_list_hit(world, r, 0.0, 1e308, &rec)) {
        vec3_t color = vec3(rec.normal.e[0] + 1.0, rec.normal.e[1] + 1.0,
                           rec.normal.e[2] + 1.0);
        return vec3_mul(color, 0.5);
    }

    vec3_t unit_direction = vec3_normalize(r.direction);
    double t = 0.5 * (unit_direction.e[1] + 1.0);
    vec3_t white = vec3(1.0, 1.0, 1.0);
    vec3_t blue = vec3(0.5, 0.7, 1.0);
    return vec3_add(vec3_mul(white, 1.0 - t), vec3_mul(blue, t));
}

/* Write color value to PPM file (0-255) with gamma correction */
static void write_color(FILE *out, const vec3_t color, int samples) {
    double r = color.e[0] / samples;
    double g = color.e[1] / samples;
    double b = color.e[2] / samples;

    /* Gamma correction (gamma = 2.0) */
    r = sqrt(r);
    g = sqrt(g);
    b = sqrt(b);

    int ir = (int)(255.999 * r);
    int ig = (int)(255.999 * g);
    int ib = (int)(255.999 * b);
    fprintf(out, "%d %d %d\n", ir, ig, ib);
}

int main(void) {
    /* Create output directory if needed */
    (void)system("mkdir -p output");

    /* Create world (scene) */
    hittable_list_t *world = hittable_list_create();
    if (!world) {
        fprintf(stderr, "Error: could not create hittable list\n");
        return 1;
    }

    /* Add a sphere to the world */
    sphere_t *sphere = sphere_create(vec3(0.0, 0.0, -1.0), 0.5, NULL);
    if (sphere) {
        hittable_list_add(world, sphere_to_hittable(sphere));
    }

    /* Camera setup */
    camera_t camera = camera_create(
        vec3(0.0, 0.0, 0.0),           /* lookfrom */
        vec3(0.0, 0.0, -1.0),          /* lookat */
        vec3(0.0, 1.0, 0.0),           /* vup */
        90.0,                           /* vfov (degrees) */
        (double)IMAGE_WIDTH / IMAGE_HEIGHT, /* aspect ratio */
        0.0,                            /* aperture (no defocus) */
        1.0                             /* focus distance */
    );

    /* Open output file */
    FILE *out = fopen("output/antialiased.ppm", "w");
    if (!out) {
        fprintf(stderr, "Error: could not open output/antialiased.ppm\n");
        hittable_list_destroy(world);
        return 1;
    }

    /* Write PPM header */
    fprintf(out, "P3\n%d %d\n255\n", IMAGE_WIDTH, IMAGE_HEIGHT);

    /* Render each pixel with multisampling */
    for (int j = IMAGE_HEIGHT - 1; j >= 0; j--) {
        fprintf(stderr, "\rScanlines remaining: %d  ", j);
        fflush(stderr);

        for (int i = 0; i < IMAGE_WIDTH; i++) {
            vec3_t pixel_color = vec3(0.0, 0.0, 0.0);

            /* Multiple samples per pixel for antialiasing */
            for (int s = 0; s < SAMPLES_PER_PIXEL; s++) {
                double u = (i + random_double()) / (IMAGE_WIDTH - 1);
                double v = (j + random_double()) / (IMAGE_HEIGHT - 1);
                ray_t r = camera_get_ray(&camera, u, v);
                pixel_color = vec3_add(pixel_color, ray_color(r, world));
            }

            write_color(out, pixel_color, SAMPLES_PER_PIXEL);
        }
    }

    fprintf(stderr, "\nDone.\n");
    fclose(out);
    hittable_list_destroy(world);
    return 0;
}
