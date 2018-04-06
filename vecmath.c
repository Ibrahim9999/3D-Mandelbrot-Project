#include "vecmath.h"

// Cheicking if two vec3f's are equal to each other
int VecEquals(vec3f a, vec3f b)
{
	return (a.x == b.x && a.y == b.y && a.z == b.z);
}

// Returns length of vec3f
float v3f_length(vec3f v) {
    return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

// Normalizes vec3f
vec3f v3f_normalize(vec3f v) {
    double len = v3f_length(v);

    return VecDoubleDivide(v, len);
}

// Multiplies vec3f by a double
vec3f VecDoubleDivide(vec3f v, double d)
{
	v.x /= d;
	v.y /= d;
	v.z /= d;

	return v;
}

// Multiplies vec3f by a double
vec3f VecDoubleMultiply(vec3f v, double d)
{
	v.x *= d;
	v.y *= d;
	v.z *= d;

	return v;
}

// Adds to vec3f's together
vec3f VecVecAdd(vec3f a, vec3f b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;

    return a;
}

// Subtracts a vec3f from another
vec3f VecVecSubtract(vec3f a, vec3f b) {
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;

    return a;
}

// Returns a quaterion as a vec4f from a real double and an imaginary vec3f 
vec4f QuatFromDoubleVec(double d, vec3f v)
{
	vec4f q;
	
	q.w = d;
	q.x = v.x;
	q.y = v.y;
	q.z = v.z;
	
	return q;
}

// Returns a normalized quaterion as a vec4f from an angle and axis, represents
// a rotation in 3D space
vec4f QuatFromAxisAngle(double angle, vec3f axis)
{
	return QuatFromDoubleVec(cos(angle / 2), VecDoubleMultiply(axis, sin(angle / 2)));
}

// Adds two vec4f's together
vec4f QuatQuatAdd(vec4f a, vec4f b)
{
	vec4f c;
	
	c.w = a.w + b.w;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	
	return c;
}

// Subtracts a vec4f from another
vec4f QuatQuatSubtract(vec4f a, vec4f b)
{
	vec4f c;
	
	c.w = a.w - b.w;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	
	return c;
}

// Multiplies a vec4f by a double
vec4f QuatDoubleMultiply(vec4f q, double d)
{
	return (vec4f){q.x * d, q.y * d, q.z * d, q.w * d};
}

// Divides a vec4f by another
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

// Divides a vec4f by a double
vec4f QuatDoubleDivide(vec4f a, double b)
{
	a.w = a.w / b;
	a.x = a.x / b;
	a.y = a.y / b;
	a.z = a.z / b;
	
	return a;
}

// Returns the inverse of a quaternion as a vec4f
vec4f QuatInverse(vec4f q)
{
	return QuatDoubleDivide(q, q.z*q.z + q.x*q.x + q.y*q.y + q.w*q.w);
}

// Takes the Exp of a quaternion as a vec4f
vec4f QuatExp(vec4f q)
{
	double r = sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
	double et = exp(q.w);
	double s = r == 0 ? 0 : et * sin(r) / r;
	
	q.w = et * cos(r);
	q.x = q.x * s;
	q.y = q.y * s;
	q.z = q.z * s;
	
	return q;
}

// Takes the natural logarithm of a quaternion as a vec4f
vec4f QuatLn(vec4f q)
{
	double r = sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
	double t = r == 0 ? 0 : atan2(r, q.w) / r ;
	
	q.w = .5 * log(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
	q.x = q.x * t;
	q.y = q.y * t;
	q.z = q.z * t;
	
	return q;
}

// Raises quaternon as a vec4f to a power ~ Unsure of decimal powers work or not
vec4f QuatPower(vec4f q, double p)
{
	return QuatExp(QuatDoubleMultiply(QuatLn(q),p));
}

// Moves a point as a vec3f along an axis as a vec3f
vec3f MoveAlongAxis(vec3f position, vec3f axis, double scalar) {
	 return VecVecAdd(position, VecDoubleMultiply(axis, scalar));
}

// Extracts yaw, pitch, and roll angles from a unit quaternion as a vec4f
void QuatToEuler(vec4f rotation, double* yaw, double* pitch, double* roll)
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

// Multiplies two quaternions as vec4f's to each other
vec4f QuatQuatMultiply(vec4f a, vec4f b)
{
	vec4f c;
	
	c.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
	c.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
	c.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
	c.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
	
	return c;
}

// Multiplies a Quaternion as a vec4f by a vec3f
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

// Rotates axis as ve3f by quaternion rotation as a vec4f
void ApplyRotationToVector(vec4f rotation, vec3f* axis)
{
	*axis =  QuatVecMultiply(QuatInverse(rotation), QuatVecMultiply(rotation, *axis));
}

// Applies yaw rotation by a double angle that updates all the relative axes
void Yaw(double angle, vec3f* horizontalAxis, vec3f* verticalAxis, vec3f* depthAxis)
{
	// Convert angle to radians
	angle = angle * PI / 180;

	// Generates yaw rotation terms of a quaternion
	vec4f localRotation = QuatFromAxisAngle(angle / 2, *verticalAxis);

	// Applies rotation to the unused rotation axis so future
	// rotations will be relative to the camera
	ApplyRotationToVector(localRotation, horizontalAxis);
	ApplyRotationToVector(localRotation, depthAxis);
}

// Applies pitch rotation by a double angle that updates all the relative axes
void Pitch(double angle, vec3f* horizontalAxis, vec3f* verticalAxis, vec3f* depthAxis)
{
	// Convert angle to radians
	angle = angle * PI / 180;

	// Applies pitch rotation to the vector
	vec4f localRotation = QuatFromAxisAngle(angle / 2, *horizontalAxis);

	// Applies rotation to the unused rotation axis so future
	// rotations will be relative to the camera
	ApplyRotationToVector(localRotation, verticalAxis);
	ApplyRotationToVector(localRotation, depthAxis);
}

// Applies roll rotation by a double angle that updates all the relative axes
void Roll(double angle, vec3f* horizontalAxis, vec3f* verticalAxis, vec3f* depthAxis)
{
	// Convert angle to radians
	angle = angle * PI / 180;

	// Applies roll rotation to the facingForward vector
	vec4f localRotation = QuatFromAxisAngle(angle / 2, *depthAxis);

	// Applies rotation to the unused rotation axis so future
	// rotations will be relative to the vector
	ApplyRotationToVector(localRotation, horizontalAxis);
	ApplyRotationToVector(localRotation, verticalAxis);
}

/*  _     _  _ _     _            _
 * |       ||   |   |              |
 * | a b c || x |   | ax + by + cz |
 * | p q r || y | = | px + qy + rz |
 * | u v w || z |   | ux + vy + wz |
 * |_     _||_ _|   |_            _|
 */

void RotateYZ(double angle, vec4f* v) {
    v->y = v->y*cos(angle) - v->z*sin(angle);
    v->z = v->y*sin(angle) + v->z*cos(angle);
}
void RotateZW(double angle, vec4f* v) {
    v->z = v->z*cos(angle) - v->w*sin(angle);
    v->w = v->z*sin(angle) + v->w*cos(angle);
}
void RotateWX(double angle, vec4f* v) {
    v->w = v->w*cos(angle) - v->x*sin(angle);
    v->x = v->w*sin(angle) + v->x*cos(angle);
}
void RotateXZ(double angle, vec4f* v) {
    v->x = v->x*cos(angle) - v->z*sin(angle);
    v->z = v->x*sin(angle) + v->z*cos(angle);
}
void RotateYW(double angle, vec4f* v) {
    v->y = v->y*cos(angle) - v->w*sin(angle);
    v->w = v->y*sin(angle) + v->w*cos(angle);
}

