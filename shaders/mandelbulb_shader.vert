#version 400

uniform vec3 FOV;
uniform vec3 cameradir;

in int gl_VertexID;
in int gl_InstanceID;

out vec3 direction;

void main() {
    if (gl_VertexID == 0) {
        direction = FOV;
    }
    else if (gl_VertexID == 1) {
        direction = vec3(-FOV.x, FOV.yz);
    }
    else if (gl_VertexID == 2) {
        direction = vec3(-FOV.x, -FOV.y, FOV.z);
    }
    else {
        direction = vec3(FOV.x, -FOV.y, FOV.z);
    }

    double x = cameradir.x;
    double y = cameradir.y;
    double z = cameradir.z;

    mat3 pitch =    mat3( cos(x),       0,  sin(x),
                               0,       1,       0,
                         -sin(x),       0,  cos(x));
    mat3 yaw =      mat3( cos(y), -sin(y),       0,
                          sin(y),  cos(y),       0,
                               0,       0,       1);
    mat3 roll =     mat3(      1,       0,       0,
                               0,  cos(z), -sin(z),
                               0,  sin(z),  cos(z));

    mat3 transform = roll*yaw*pitch;
    direction = transform * direction;
}

#define PI 3.1415926535897932384626433832795

/* GLSL Code
 *.x-Axis: Up/Down
 *.y-Axis: Right/Left
 *.z-Axis: Forward/Backward
 * 
 * horizontalAxis, verticalAxis, and depthAxis
 * are all relative to the camera direction
 */

vec4 QuatFromDoubleVec(in double d, in vec3 v)
{
	vec4 q;
	
	q.w = d;
	q.x = v.x;
	q.y = v.y;
	q.z = v.z;
	
	return q;
}

vec4 QuatFromAxisAngle(in double angle, in vec3 axis)
{
	vec4 q;
	
	q.w = cos(angle / 2);
	
	
	return new Quaternion(cos(angle/2), axis * sin(angle/2));
}

vec4 QuatQuatAdd(in vec4 a, in vec4 b)
{
	vec4 c;
	
	c.w = a.w + b.w;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	
	return c;
}

vec4 QuatQuatSubtract(in vec4 a, in vec4 b)
{
	vec4 c;
	
	c.w = a.w - b.w;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	
	return c;
}

vec4 QuatQuationMultiply(in vec4 a, in vec4 b)
{
	vec4 c;
	
	c.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
	c.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
	c.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
	c.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
	
	return c;
}

vec3 QuatVecMultiply(in vec4 q, in vec3 v)
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
	
	vec3 result;
	
	result.x = v.x * (1 - 2 * (yy + zz)) + v.y * 2 * (xy - wz) + v.z * 2 * (xz + wy);
	result.y = v.x * 2 * (xy + wz) + v.y * (1 - 2 * (xx + zz)) + v.z * 2 * (yz - wx);
	result.z = v.x * 2 * (xz - wy) + v.y * 2 * (yz + wx) + v.z * (1 - 2 * (xx + yy));
	
	return result;
}

vec4 QuatQuatDivide(in vec4 a, in vec4 b)
{
	vec4 c;
	double s = b.w*b.w + b.x*b.x + b.y*b.y + b.z*b.z;
	
	c.w = (a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z) / s;
	c.x = (-a.w*b.x + a.x*b.w - a.y*b.z + a.z*b.y) / s;
	c.y = (-a.w*b.y + a.x*b.z + a.y*b.w - a.z*b.x) / s;
	c.z = (-a.w*b.z - a.x*b.y + a.y*b.x + a.z*b.w) / s;
	
	return c;
}

vec4 QuatDoubleDivide(in vec4 a, in double b)
{
	b.w = b.w / b;
	b.x = b.x / b;
	b.y = b.y / b;
	b.z = b.z / b;
	
	return b;
}

vec4 QuatInverse(in vec4 q)
{
	return QuatDoubleDivide(q, v.z*v.z + v.x*v.x + v.y*v.y + v.z*v.z);
}

vec4 QuatExp(in vec4 a)
{
	vec4 c;
	double r = sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
	double et = exp(q.w);
	double s = r == 0 ? 0 : et * sin(r) / r;
	
	q.w = et * cos(r);
	q.x = q.x * s;
	q.y = q.y * s;
	q.z = q.z * s;
	
	return c;
}

vec4 QuatLn(in vec4 a)
{
	vec4 c;
	double r = sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
	double t = r == 0 ? 0 : atan(r, q.w) / r ;
	
	q.w = .5 * log(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
	q.x = q.x * t;
	q.y = q.y * t;
	q.z = q.z * t;
	
	return c;
}

vec4 QuatPower(in vec4 q, in double p)
{
	return QuaternionExp(QuaternionMultiply(QuaternionLn(q),power));
}

void MoveForward(inout vec3 position, in vec3 forward, in double scalar)
{
	position += forward * scalar;
}

void ApplyRotationToVector(in vec4 rotation, inout vec3 axis)
{
	axis =  QuatVecMultiply(QuatInverse(rotation), QuatVecMultiply(rotation, axis));
}

void InitializeCamera(inout vec3 facingForward, inout vec3 cameraPosition, inout vec3 depthAxis, inout vec3 horizontalAxis, inout vec3 verticalAxis)
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
	verticalAxis = v;
}

void yaw(in double angle, inout vec3 vector, in vec3 horizontalAxis, in vec3 verticalAxis, in vec3 depthAxis)
{
	// Convert angle to radians
	angle = angle * PI / 180;
	
	// Generates yaw rotation in terms of a quaternion
	vec4 localRotation = QuaternionFromAxisAngle(angle / 2, verticalAxis);
	
	// Applies yaw rotation to the vector
	PointQuatToAxis(localRotation, vector);
	
	// Applies rotation to the unused rotation axis so future
	// rotations will be relative to the vector
	ApplyRotationToVector(localRotation, horizontalAxis);
	ApplyRotationToVector(localRotation, depthAxis);
}

void pitch(in double angle, inout vec3 vector, in vec3 horizontalAxis, in vec3 verticalAxis, in vec3 depthAxis)
{
	// Convert angle to radians
	angle = angle * PI / 180;
	
	// Applies pitch rotation to the vector
	vec4 localRotation = QuaternionFromAxisAngle(angle / 2, horizontalAxis);
	
	// Applies pitch rotation to the vector
	PointQuatToAxis(localRotation, vector);
	
	// Applies rotation to the unused rotation axis so future
	// rotations will be relative to the vector
	ApplyRotationToVector(localRotation, verticalAxis);
	ApplyRotationToVector(localRotation, depthAxis);
}

void roll(in double angle, inout vec3 vector, in vec3 horizontalAxis, in vec3 verticalAxis, in vec3 depthAxis)
{
	// Convert angle to radians
	angle = angle * PI / 180;
	
	// Applies roll rotation to the facingForward vector
	vec4 localRotation = QuaternionFromAxisAngle(angle / 2, depthAxis);
	
	// Applies roll rotation to the vector
	PointQuatToAxis(localRotation, vector);
	
	// Applies rotation to the unused rotation axis so future
	// rotations will be relative to the vector
	ApplyRotationToVector(localRotation, depthAxis);
	ApplyRotationToVector(localRotation, verticalAxis);
}
