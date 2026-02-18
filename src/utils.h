#ifndef UTILS_H
#define UTILS_H

#define PI 3.1415926535897932385

/* Clamp value between min and max */
static inline double clamp(double x, double min, double max) {
    return x < min ? min : (x > max ? max : x);
}

#endif /* UTILS_H */
