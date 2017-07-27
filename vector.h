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

typedef struct vector3f {
    float x;
    float y;
    float z;
} vec3f;
typedef struct vector4f {
    float x;
    float y;
    float z;
	float w;
} vec4f;

float v3f_length(vec3f v);
vec3f v3f_normalize(vec3f v);
vec3f VecDoubleMultiply(vec3f v, double d);

vec4f QuatFromDoubleVec(double d, vec3f v);
vec4f QuatFromAxisAngle(double angle, vec3f axis);
vec4f QuatQuatAdd(vec4f a, vec4f b);
vec4f QuatQuatSubtract(vec4f a, vec4f b);
vec4f QuatQuationMultiply(vec4f a, vec4f b);
vec3f QuatVecMultiply(vec4f q, vec3f v);
vec3f QuatDoubleMultiply(vec4f q, double v);
vec4f QuatQuatDivide(vec4f a, vec4f b);
vec4f QuatDoubleDivide(vec4f a, double b);
vec4f QuatInverse(vec4f q);
vec4f QuatExp(vec4f a);
vec4f QuatLn(vec4f a);
vec4f QuatPower(vec4f q, double p);
void MoveForward(out vec3f position, vec3f forward, double scalar);
void ApplyRotationToVector(vec4f rotation, out vec3f axis);
void InitializeCamera(out vec3f facingForward, out vec3f cameraPosition, out vec3f depthAxis, out vec3f horizontalAxis, out vec3f verticalAxis);
void Yaw(double angle, out vec3f vector vec3f horizontalAxis, vec3f verticalAxis, vec3f depthAxis);
void Pitch(double angle, out vec3f vector vec3f horizontalAxis, vec3f verticalAxis, vec3f depthAxis);
void Roll(double angle, out vec3f vector vec3f horizontalAxis, vec3f verticalAxis, vec3f depthAxis);

#endif
