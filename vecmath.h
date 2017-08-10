#ifndef VECTOR3F
#define VECTOR3F

#define PI 3.1415926535897932384626433832795

#include <math.h>

/* C Code
 *
 * X-Axis: Up/Down
 * Y-Axis: Right/Left
 * Z-Axis: Forward/Backward
 * 
 * horizontalAxis, verticalAxis, and depthAxis
 * are all relative to the camera direction
 *
 */

typedef struct vector2f {
    double x;
    double y;
} vec2f;
typedef struct vector3f {
    double x;
    double y;
    double z;
} vec3f;
typedef struct vector4f {
    double x;
    double y;
    double z;
	double w;
} vec4f;

int VecEquals(vec3f a, vec3f b);
float v3f_length(vec3f v);
vec3f v3f_normalize(vec3f v);
vec3f VecDoubleMultiply(vec3f v, double d);
vec3f VecDoubleDivide(vec3f v, double d);
vec3f VecVecAdd(vec3f a, vec3f b);
vec3f VecVecSubtract(vec3f a, vec3f b);
vec4f QuatFromDoubleVec(double d, vec3f v);
vec4f QuatFromAxisAngle(double angle, vec3f axis);
vec4f QuatQuatAdd(vec4f a, vec4f b);
vec4f QuatQuatSubtract(vec4f a, vec4f b);
vec4f QuatQuatMultiply(vec4f a, vec4f b);
vec3f QuatVecMultiply(vec4f q, vec3f v);
vec4f QuatDoubleMultiply(vec4f q, double v);
vec4f QuatQuatDivide(vec4f a, vec4f b);
vec4f QuatDoubleDivide(vec4f a, double b);
vec4f QuatInverse(vec4f q);
vec4f QuatExp(vec4f a);
vec4f QuatLn(vec4f a);
vec4f QuatPower(vec4f q, double p);
vec3f MoveAlongAxis(vec3f position, vec3f axis, double scalar);
void VecToEuler(vec4f rotation, double* yaw, double* pitch, double* roll);
void ApplyRotationToVector(vec4f rotation, vec3f* axis);
void Yaw(double angle, vec3f* horizontalAxis, vec3f* verticalAxis, vec3f* depthAxis);
void Pitch(double angle, vec3f* horizontalAxis, vec3f* verticalAxis, vec3f* depthAxis);
void Roll(double angle, vec3f* horizontalAxis, vec3f* verticalAxis, vec3f* depthAxis);
void RotateYZ(double angle, vec4f* v);
void RotateZW(double angle, vec4f* v);
void RotateWX(double angle, vec4f* v);
void RotateXZ(double angle, vec4f* v);
void RotateYW(double angle, vec4f* v);

#endif
