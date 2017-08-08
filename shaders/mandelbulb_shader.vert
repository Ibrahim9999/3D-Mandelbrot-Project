#version 130

uniform vec3 FOV;
uniform vec3 horizontalAxis;
uniform vec3 verticalAxis;
uniform vec3 depthAxis;

out vec3 direction;

vec3 VecDoubleMultiply(in vec3 v, in float d)
{
    v.x *= d;
    v.y *= d;
    v.z *= d;

	return v;
}

vec3 VecVecAdd(in vec3 a, in vec3 b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;

    return a;
}

vec3 MoveAlongAxis(in vec3 position, in vec3 axis, in float scalar)
{
	 return VecVecAdd(position, VecDoubleMultiply(axis, scalar));
}

void main() {
    if (gl_VertexID == 0) {
        direction = FOV;
        gl_Position = vec4(1.0, 1.0, 0.0, 1.0);
    }
    else if (gl_VertexID == 1) {
        direction = vec3(-FOV.x, FOV.y, FOV.z);
        gl_Position = vec4(-1.0, 1.0, 0.0, 1.0);
    }
    else if (gl_VertexID == 2) {
        direction = vec3(-FOV.x, -FOV.y, FOV.z);
        gl_Position = vec4(-1.0, -1.0, 0.0, 1.0);
    }
    else {
        direction = vec3(FOV.x, -FOV.y, FOV.z);
        gl_Position = vec4(1.0, -1.0, 0.0, 1.0);
    }

	vec3 p = vec3 (0,0,0);
	
	p = MoveAlongAxis(p, horizontalAxis, direction.x);
	p = MoveAlongAxis(p, verticalAxis, direction.y);
	p = MoveAlongAxis(p, depthAxis, direction.z);

	direction = p;
}