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
	
	
	return new Quaternion(Math.Cos(angle/2), axis * Math.Sin(angle/2));
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

vec4f QuatQuationMultiply(vec4f a, vec4f b)
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
	double xx = q.x * q.x;
	double yy = q.y * q.y;
	double zz = q.z * q.z;
	double xy = q.x * q.y;
	double xz = q.x * q.z;
	double yz = q.y * q.z;
	double wx = q.w * q.x;
	double wy = q.w * q.y;
	double wz = q.w * q.z;
	
	vec3f result;
	
	result.X = v.x * (1 - 2 * (yy + zz)) + v.y * 2 * (xy - wz) + v.z * 2 * (xz + wy);
	result.Y = v.x * 2 * (xy + wz) + v.y * (1 - 2 * (xx + zz)) + v.z * 2 * (yz - wx);
	result.Z = v.x * 2 * (xz - wy) + v.y * 2 * (yz + wx) + v.z * (1 - 2 * (xx + yy));
	
	return result;
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
	b.w = b.w / b;
	b.x = b.x / b;
	b.y = b.y / b;
	b.z = b.z / b;
	
	return b;
}

vec4f QuatInverse(vec4f q)
{
	return QuatDoubleDivide(q, v.z*v.z + v.x*v.x + v.y*v.y + v.z*v.z);
}

vec4f QuatExp(vec4f a)
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

vec4f QuatLn(vec4f a)
{
	vec4f c;
	double r = sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
	double t = r == 0 ? 0 : atan(r, q.w) / r ;
	
	q.w = .5 * log(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
	q.x = q.x * t;
	q.y = q.y * t;
	q.z = q.z * t;
	
	return c;
}

vec4f QuatPower(vec4f q, double p)
{
	return QuaternionExp(QuaternionMultiply(QuaternionLn(q),power));
}

void MoveForward(out vec3f position, vec3f forward, double scalar)
{
	position += forward * scalar;
}

void ApplyRotationToVector(vec4f rotation, out vec3f axis)
{
	axis =  QuatVecMultiply(QuatInverse(rotation), QuatVecMultiply(rotation, axis));
}

void InitializeCamera(out vec3f facingForward, out vec3f cameraPosition, out vec3f depthAxis, out vec3f horizontalAxis, out vec3f verticalAxis)
{
	Vector v;
	
	v.x = 0;
	v.y = 0;
	v.z = 0;
	cameraPosition = v;
	
	v.z = 1;
	depthAxis = v;
	facingForward = v;
	
	v.x = 1;
	v.z = 0;
	horizontalAxis = v;
	
	v.x = 0;
	v.y = 1;
	horizontalAxis = v;
}

void Yaw(double angle, out vec3f vector vec3f horizontalAxis, vec3f verticalAxis, vec3f depthAxis)
{
	// Convert angle to radians
	angle = angle * PI / 180;
	
	// Generates yaw rotation terms of a quaternion
	vec4f localRotation = QuaternionFromAxisAngle(angle / 2, verticalAxis);
	
	// Applies yaw rotation to the vector
	PointQuatToAxis(localRotation, vector);
	
	// Applies rotation to the unused rotation axis so future
	// rotations will be relative to the vector
	ApplyRotationToVector(localRotation, horizontalAxis);
	ApplyRotationToVector(localRotation, depthAxis);
}

void Pitch(double angle, out vec3f vector vec3f horizontalAxis, vec3f verticalAxis, vec3f depthAxis)
{
	// Convert angle to radians
	angle = angle * PI / 180;
	
	// Applies pitch rotation to the vector
	vec4f localRotation = QuaternionFromAxisAngle(angle / 2, horizontalAxis);
	
	// Applies pitch rotation to the vector
	PointQuatToAxis(localRotation, vector);
	
	// Applies rotation to the unused rotation axis so future
	// rotations will be relative to the vector
	ApplyRotationToVector(localRotation, verticalAxis);
	ApplyRotationToVector(localRotation, depthAxis);
}

void Roll(double angle, out vec3f vector vec3f horizontalAxis, vec3f verticalAxis, vec3f depthAxis)
{
	// Convert angle to radians
	angle = angle * PI / 180;
	
	// Applies roll rotation to the facingForward vector
	vec4f localRotation = QuaternionFromAxisAngle(angle / 2, depthAxis);
	
	// Applies roll rotation to the vector
	PointQuatToAxis(localRotation, vector);
	
	// Applies rotation to the unused rotation axis so future
	// rotations will be relative to the vector
	ApplyRotationToVector(localRotation, depthAxis);
	ApplyRotationToVector(localRotation, verticalAxis);
}
