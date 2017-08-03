#version 130

uniform vec3 FOV;
uniform vec4 totalRotation;

out vec3 direction;

vec4 QuatInverse(in vec4 q)
{
    return q / (q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
}

vec3 QuatVecMultiply(in vec4 q, in vec3 v)
{
    float xx = q.x*q.x;
    float yy = q.y*q.y;
    float zz = q.z*q.z;
    float xy = q.x*q.y;
    float xz = q.x*q.z;
    float yz = q.y*q.z;
    float wx = q.w*q.x;
    float wy = q.w*q.y;
    float wz = q.w*q.z;
    
    vec3 result;
    
    result.x = v.x * (1 - 2 * (yy + zz)) + v.y * 2 * (xy - wz) + v.z * 2 * (xz + wy);
    result.y = v.x * 2 * (xy + wz) + v.y * (1 - (2 * (xx + zz))) + v.z * 2 * (yz - wx);
    result.z = v.x * 2 * (xz - wy) + v.y * 2 * (yz + wx) + v.z * (1 - 2 * (xx + yy));
    
    return result;
}

void ApplyRotationToVector(in vec4 rotation, inout vec3 axis)
{
    axis = QuatVecMultiply(QuatInverse(rotation), QuatVecMultiply(rotation, axis));
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

    float magnitude = sqrt(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);

	direction.x /= magnitude;
	direction.y /= magnitude;
	direction.y /= magnitude;

    ApplyRotationToVector(totalRotation, direction);
}
