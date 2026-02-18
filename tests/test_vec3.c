#include "../src/vec3.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>

#define EPSILON 1e-9

int assert_vec3_equal(const char *test_name, vec3_t result, vec3_t expected) {
    if (fabs(result.e[0] - expected.e[0]) < EPSILON &&
        fabs(result.e[1] - expected.e[1]) < EPSILON &&
        fabs(result.e[2] - expected.e[2]) < EPSILON) {
        printf("✓ %s\n", test_name);
        return 1;
    } else {
        printf("✗ %s (got [%f, %f, %f], expected [%f, %f, %f])\n",
               test_name, result.e[0], result.e[1], result.e[2],
               expected.e[0], expected.e[1], expected.e[2]);
        return 0;
    }
}

int assert_double_equal(const char *test_name, double result, double expected) {
    if (fabs(result - expected) < EPSILON) {
        printf("✓ %s\n", test_name);
        return 1;
    } else {
        printf("✗ %s (got %f, expected %f)\n", test_name, result, expected);
        return 0;
    }
}

int main(void) {
    int passed = 0, failed = 0;

    /* Test vec3 constructor */
    vec3_t v1 = vec3(1.0, 2.0, 3.0);
    if (assert_vec3_equal("vec3 constructor", v1, vec3(1.0, 2.0, 3.0)))
        passed++;
    else
        failed++;

    /* Test addition */
    vec3_t a = vec3(1.0, 2.0, 3.0);
    vec3_t b = vec3(4.0, 5.0, 6.0);
    if (assert_vec3_equal("vec3_add", vec3_add(a, b), vec3(5.0, 7.0, 9.0)))
        passed++;
    else
        failed++;

    /* Test subtraction */
    if (assert_vec3_equal("vec3_sub", vec3_sub(b, a), vec3(3.0, 3.0, 3.0)))
        passed++;
    else
        failed++;

    /* Test scalar multiplication */
    if (assert_vec3_equal("vec3_mul", vec3_mul(a, 2.0), vec3(2.0, 4.0, 6.0)))
        passed++;
    else
        failed++;

    /* Test scalar division */
    if (assert_vec3_equal("vec3_div", vec3_div(a, 2.0), vec3(0.5, 1.0, 1.5)))
        passed++;
    else
        failed++;

    /* Test dot product */
    if (assert_double_equal("vec3_dot", vec3_dot(a, b), 32.0))
        passed++;
    else
        failed++;

    /* Test cross product */
    vec3_t i = vec3(1.0, 0.0, 0.0);
    vec3_t j = vec3(0.0, 1.0, 0.0);
    if (assert_vec3_equal("vec3_cross i×j", vec3_cross(i, j), vec3(0.0, 0.0, 1.0)))
        passed++;
    else
        failed++;

    /* Test length */
    vec3_t v = vec3(3.0, 4.0, 0.0);
    if (assert_double_equal("vec3_length", vec3_length(v), 5.0))
        passed++;
    else
        failed++;

    /* Test length squared */
    if (assert_double_equal("vec3_length_squared", vec3_length_squared(v), 25.0))
        passed++;
    else
        failed++;

    /* Test normalization */
    vec3_t normalized = vec3_normalize(v);
    if (assert_double_equal("vec3_normalize length", vec3_length(normalized), 1.0))
        passed++;
    else
        failed++;

    if (assert_vec3_equal("vec3_normalize value", normalized, vec3(0.6, 0.8, 0.0)))
        passed++;
    else
        failed++;

    /* Test random_double range */
    for (int i = 0; i < 100; i++) {
        double r = random_double();
        if (r < 0.0 || r >= 1.0) {
            printf("✗ random_double out of range: %f\n", r);
            failed++;
            break;
        }
    }
    printf("✓ random_double range [0, 1)\n");
    passed++;

    /* Test random_unit_vector has unit length */
    int unit_vector_valid = 1;
    for (int i = 0; i < 10; i++) {
        vec3_t ruv = random_unit_vector();
        double len = vec3_length(ruv);
        if (fabs(len - 1.0) > EPSILON) {
            printf("✗ random_unit_vector has length %f\n", len);
            unit_vector_valid = 0;
            failed++;
            break;
        }
    }
    if (unit_vector_valid) {
        printf("✓ random_unit_vector has unit length\n");
        passed++;
    }

    printf("\n%d/%d tests passed\n", passed, passed + failed);
    return failed == 0 ? 0 : 1;
}
