#include "vector.h"

float v3f_length(vec3f v) {
    return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

vec3f v3f_normalize(vec3f v) {
    float len = v3f_length(v);
    v.x /= len;
    v.y /= len;
    v.z /= len;

    return v;
}
