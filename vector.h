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

vec4f QuatFromDoubleVec(in double d, in vec3f v);
vec4f QuatFromAxisAngle(in double angle, in vec3f axis);
vec4f QuatQuatAdd(in vec4f a, in vec4f b);
vec4f QuatQuatSubtract(in vec4f a, in vec4f b);
vec4f QuatQuationMultiply(in vec4f a, in vec4f b);
vec3f QuatVecMultiply(in vec4f q, in vec3f v);
vec4f QuatQuatDivide(in vec4f a, in vec4f b);
vec4f QuatDoubleDivide(in vec4f a, in double b);
vec4f QuatInverse(in vec4f q);
vec4f QuatExp(in vec4f a);
vec4f QuatLn(in vec4f a);
vec4f QuatPower(in vec4f q, in double p);
void MoveForward(inout vec3f position, in vec3f forward, in double scalar);
void ApplyRotationToVector(in vec4f rotation, inout vec3f axis);
void InitializeCamera(inout vec3f facingForward, inout vec3f cameraPosition, inout vec3f depthAxis, inout vec3f horizontalAxis, inout vec3f verticalAxis);
void Yaw(in double angle, inout vec3f vector in vec3f horizontalAxis, in vec3f verticalAxis, in vec3f depthAxis);
void Pitch(in double angle, inout vec3f vector in vec3f horizontalAxis, in vec3f verticalAxis, in vec3f depthAxis);
void Roll(in double angle, inout vec3f vector in vec3f horizontalAxis, in vec3f verticalAxis, in vec3f depthAxis);

#endif
