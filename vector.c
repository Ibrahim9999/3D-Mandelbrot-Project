#include "vector.h"

void InitializeCamera(vec3f* facingForward, vec3f* cameraPosition, vec3f* depthAxis, vec3f* horizontalAxis, vec3f* verticalAxis)
{
	vec3f v;
	
	v.x = 0;
	v.y = 0;
	v.z = -4;
	*cameraPosition = v;
	
	v.x = 0;
	v.y = 0;
	v.z = 1;
	*depthAxis = v;
	*facingForward = v;
	
	v.x = 1;
	v.y = 0;
	v.z = 0;
	*horizontalAxis = v;
	
	v.x = 0;
	v.y = 1;
	v.z = 0;
	*verticalAxis = v;
}

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

vec3f VecDoubleMultiply(vec3f v, double d)
{
	return (vec3f){v.x * d, v.y * d, v.z*d};
}

vec3f VecVecAdd(vec3f a, vec3f b) {
    return (vec3f){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3f VecVecSubtract(vec3f a, vec3f b) {
    return (vec3f){a.x - b.x, a.y - b.y, a.z - b.z};
}

vec4f QuatFromDoubleVec(double d, vec3f v)
{
	vec4f q;
	
	q.w = d;
	q.x = v.x;
	q.y = v.y;
	q.z = v.z;
	
	return q;
}

vec4f QuatFromAxisAngle(double angle, vec3f axis)
{
	vec4f q;
	
	q.w = cos(angle / 2);
	
    vec3f a = VecDoubleMultiply(axis, sin(angle/2));
    q.x = a.x;
    q.y = a.y;
    q.z = a.z;
	
	return q;
}

vec4f QuatQuatAdd(vec4f a, vec4f b)
{
	vec4f c;
	
	c.w = a.w + b.w;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	
	return c;
}

vec4f QuatQuatSubtract(vec4f a, vec4f b)
{
	vec4f c;
	
	c.w = a.w - b.w;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	
	return c;
}

vec4f QuatDoubleMultiply(vec4f q, double d)
{
	return (vec4f){q.x * d, q.y * d, q.z * d, q.w * d};
}

vec4f QuatQuatDivide(vec4f a, vec4f b)
{
	vec4f c;
	double s = b.w*b.w + b.x*b.x + b.y*b.y + b.z*b.z;
	
	c.w = (a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z) / s;
	c.x = (-a.w*b.x + a.x*b.w - a.y*b.z + a.z*b.y) / s;
	c.y = (-a.w*b.y + a.x*b.z + a.y*b.w - a.z*b.x) / s;
	c.z = (-a.w*b.z - a.x*b.y + a.y*b.x + a.z*b.w) / s;
	
	return c;
}

vec4f QuatDoubleDivide(vec4f a, double b)
{
	a.w = a.w / b;
	a.x = a.x / b;
	a.y = a.y / b;
	a.z = a.z / b;
	
	return a;
}

vec4f QuatInverse(vec4f q)
{
	return QuatDoubleDivide(q, q.z*q.z + q.x*q.x + q.y*q.y + q.w*q.w);
}

vec4f QuatExp(vec4f q)
{
	vec4f c;
	double r = sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
	double et = exp(q.w);
	double s = r == 0 ? 0 : et * sin(r) / r;
	
	q.w = et * cos(r);
	q.x = q.x * s;
	q.y = q.y * s;
	q.z = q.z * s;
	
	return c;
}

vec4f QuatLn(vec4f q)
{
	vec4f c;
	double r = sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
	double t = r == 0 ? 0 : atan2(r, q.w) / r ;
	
	q.w = .5 * log(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
	q.x = q.x * t;
	q.y = q.y * t;
	q.z = q.z * t;
	
	return c;
}

vec4f QuatPower(vec4f q, double p)
{
	return QuatExp(QuatDoubleMultiply(QuatLn(q),p));
}

vec3f MoveAlongAxis(vec3f position, vec3f axis, double scalar) {
	 return VecVecAdd(position, VecDoubleMultiply(axis, scalar));
}


void VecToEuler(vec4f rotation, double* yaw, double* pitch, double* roll)
{
	double W = rotation.w;
	double X = rotation.x;
	double Y = rotation.y;
	double Z = rotation.z;
	
	double ysqr = Y * Y;
	
	// pitch (x-axis rotation)
	double t0 = 2 * (W * X + Y * Z);
	double t1 = 1 - 2 * (X * X + ysqr);
	*roll = atan2(t0, t1);
	
	// roll (y-axis rotation)
	t0 = 2 * (W * Y - Z * X);
	t0 = t0 > 1 ? 1 : t0;
	t0 = t0 < -1 ? -1 : t0;
	*pitch = asin(t0);
	
	// yaw (z-axis rotation)
	t0 = 2 * (W * Z + X * Y);
	t1 = 1 - 2 * (ysqr + Z * Z);  
	*yaw = atan2(t0, t1);
}

vec4f QuatQuatMultiply(vec4f a, vec4f b)
{
	vec4f c;
	
	c.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
	c.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
	c.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
	c.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
	
	return c;
}

vec3f QuatVecMultiply(vec4f q, vec3f v)
{
	long double xx = q.x * q.x;
	long double yy = q.y * q.y;
	long double zz = q.z * q.z;
	long double xy = q.x * q.y;
	long double xz = q.x * q.z;
	long double yz = q.y * q.z;
	long double wx = q.w * q.x;
	long double wy = q.w * q.y;
	long double wz = q.w * q.z;
	
	vec3f result;
	
	result.x = v.x * (1 - 2 * (yy + zz)) + v.y * 2 * (xy - wz) + v.z * 2 * (xz + wy);
	result.y = v.x * 2 * (xy + wz) + v.y * (1 - 2 * (xx + zz)) + v.z * 2 * (yz - wx);
	result.z = v.x * 2 * (xz - wy) + v.y * 2 * (yz + wx) + v.z * (1 - 2 * (xx + yy));
	
	return result;
}

void ApplyRotationToVector(vec4f rotation, vec3f* axis)
{
	*axis =  QuatVecMultiply(QuatInverse(rotation), QuatVecMultiply(rotation, *axis));
}

void Yaw(double angle, vec4f* totalRotation, vec3f* vector, vec3f* horizontalAxis, vec3f* verticalAxis, vec3f* depthAxis)
{
	// Convert angle to radians
	angle = angle * PI / 180;

	// Generates yaw rotation terms of a quaternion
	vec4f localRotation = QuatFromAxisAngle(angle / 2, *verticalAxis);

	// Updated total rotation with the local rotation
	*totalRotation = QuatQuatMultiply(localRotation, *totalRotation);

	// Applies yaw rotation to the vector
	ApplyRotationToVector(localRotation, vector);

	// Applies rotation to the unused rotation axis so future
	// rotations will be relative to the vector
	ApplyRotationToVector(localRotation, horizontalAxis);
	ApplyRotationToVector(localRotation, depthAxis);
}

void Pitch(double angle, vec4f* totalRotation, vec3f* vector, vec3f* horizontalAxis, vec3f* verticalAxis, vec3f* depthAxis)
{
	// Convert angle to radians
	angle = angle * PI / 180;

	// Applies pitch rotation to the vector
	vec4f localRotation = QuatFromAxisAngle(angle / 2, *horizontalAxis);

	// Updated total rotation with the local rotation
	*totalRotation = QuatQuatMultiply(localRotation, *totalRotation);

	// Applies pitch rotation to the vector
	ApplyRotationToVector(localRotation, vector);

	// Applies rotation to the unused rotation axis so future
	// rotations will be relative to the vector
	ApplyRotationToVector(localRotation, verticalAxis);
	ApplyRotationToVector(localRotation, depthAxis);
}

void Roll(double angle, vec4f* totalRotation, vec3f* vector, vec3f* horizontalAxis, vec3f* verticalAxis, vec3f* depthAxis)
{
	// Convert angle to radians
	angle = angle * PI / 180;

	// Applies roll rotation to the facingForward vector
	vec4f localRotation = QuatFromAxisAngle(angle / 2, *depthAxis);

	// Updated total rotation with the local rotation
	*totalRotation = QuatQuatMultiply(localRotation, *totalRotation);

	// Applies roll rotation to the vector
	ApplyRotationToVector(localRotation, vector);

	// Applies rotation to the unused rotation axis so future
	// rotations will be relative to the vector
	ApplyRotationToVector(localRotation, horizontalAxis);
	ApplyRotationToVector(localRotation, verticalAxis);
}

/*
void Yaw(double angle, vec4f* totalRotation, vec3f* vector, vec3f* horizontalAxis, vec3f* verticalAxis, vec3f* depthAxis)
{
	vec3f right = { 1.0,0.0,0.0 };
	vec3f up = { 0.0, 1.0, 0.0 };
	vec3f in = { 0.0, 0.0, 1.0 };

	// Convert angle to radians
	angle = angle * PI / 180;

	// Applies pitch rotation to the vector
	vec4f localRotation = QuatFromAxisAngle(angle / 2, *verticalAxis);

	// Updated total rotation with the local rotation
	*totalRotation = QuatQuatMultiply(localRotation, *totalRotation);

	// Applies pitch rotation to the vector
	ApplyRotationToVector(localRotation, vector);

	// Applies rotation to the unused rotation axis so future
	// rotations will be relative to the vector
	ApplyRotationToVector(localRotation, &right);
	ApplyRotationToVector(localRotation, &up);
	ApplyRotationToVector(localRotation, &in);

	*horizontalAxis = right;
	*verticalAxis = up;
	*depthAxis = in;
}

void Pitch(double angle, vec4f* totalRotation, vec3f* vector, vec3f* horizontalAxis, vec3f* verticalAxis, vec3f* depthAxis)
{
	vec3f right = { 1.0,0.0,0.0 };
	vec3f up = { 0.0, 1.0, 0.0 };
	vec3f in = { 0.0, 0.0, 1.0 };

	// Convert angle to radians
	angle = angle * PI / 180;
	
	// Applies pitch rotation to the vector
	vec4f localRotation = QuatFromAxisAngle(angle / 2, *horizontalAxis);
	
	// Updated total rotation with the local rotation
	*totalRotation = QuatQuatMultiply(localRotation, *totalRotation);
	
	// Applies pitch rotation to the vector
	ApplyRotationToVector(localRotation, vector);
	
	// Applies rotation to the unused rotation axis so future
	// rotations will be relative to the vector
	ApplyRotationToVector(localRotation, &right);
	ApplyRotationToVector(localRotation, &up);
	ApplyRotationToVector(localRotation, &in);

	*horizontalAxis = right;
	*verticalAxis = up;
	*depthAxis = in;
}

void Roll(double angle, vec4f* totalRotation, vec3f* vector, vec3f* horizontalAxis, vec3f* verticalAxis, vec3f* depthAxis)
{
	vec3f right = { 1.0,0.0,0.0 };
	vec3f up = { 0.0, 1.0, 0.0 };
	vec3f in = { 0.0, 0.0, 1.0 };

	// Convert angle to radians
	angle = angle * PI / 180;

	// Applies pitch rotation to the vector
	vec4f localRotation = QuatFromAxisAngle(angle / 2, *depthAxis);

	// Updated total rotation with the local rotation
	*totalRotation = QuatQuatMultiply(localRotation, *totalRotation);

	// Applies pitch rotation to the vector
	ApplyRotationToVector(localRotation, vector);

	// Applies rotation to the unused rotation axis so future
	// rotations will be relative to the vector
	ApplyRotationToVector(localRotation, &right);
	ApplyRotationToVector(localRotation, &up);
	ApplyRotationToVector(localRotation, &in);

	*horizontalAxis = right;
	*verticalAxis = up;
	*depthAxis = in;
}
*/