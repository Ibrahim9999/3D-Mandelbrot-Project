#ifndef VECTOR3F
#define VECTOR3F

#include <math.h>

typedef struct vector3f {
    float x;
    float y;
    float z;
} vec3f;

float v3f_length(vec3f v);
vec3f v3f_normalize(vec3f v);

#endif
