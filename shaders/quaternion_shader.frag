#define PI 3.1415926535897932384626433832795

/* GLSL Code
 *.x-Axis: Up/Down
 *.y-Axis: Right/Left
 *.z-Axis: Forward/Backward
 * 
 * horizontalAxis, verticalAxis, and depthAxis
 * are all relative to the camera direction
 */

vec4 QuatFromDoubleVec(in float d, in vec3 v)
{
	vec4 q;
	
	q.w = d;
	q.x = v.x;
	q.y = v.y;
	q.z = v.z;
	
	return q;
}

vec4 QuatFromAxisAngle(in float angle, in vec3 axis)
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
	
