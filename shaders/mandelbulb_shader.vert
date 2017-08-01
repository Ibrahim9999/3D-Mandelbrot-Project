#version 130

uniform vec3 FOV;
uniform vec3 cameradir;
uniform vec4 totalRotation;

out vec3 direction;

void getEulerFromVec(in vec4 rotation, inout float yaw, inout float pitch, inout float roll) {
    float x = rotation.x;
    float y = rotation.y;
    float z = rotation.z;
    float w = rotation.w;

    float ySquared = y*y;

    float t0 = 2 * (w * x + y * z);
    float t1 = 1 - 2 * (x * x + ySquared);
    roll = atan(t0, t1);

    t0 = 2 * (w * y - z * x);
    clamp(t0, -1, 1);
    
    pitch = asin(t0);

    t0 = 2 * (w * z + x * y);
    t1 = 1 - 2 * (ySquared + z * z);
    yaw = atan(t0, t1);
}

vec4 QuatInverse(in vec4 q)
{
    return q / (q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
}

vec4 QuatQuatMultiply(in vec4 a, in vec4 b)
{
    vec4 c;
    
    c.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*a.z;
    c.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*a.y;
    c.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*a.x;
    c.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*a.w;
    
    return c;
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
        direction = vec3(-FOV.x, FOV.yz);
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

    float yaw_angle = 0.0;
    float pitch_angle = 0.0;
    float roll_angle = 0.0;

    getEulerFromVec(totalRotation, roll_angle, yaw_angle, pitch_angle);

    mat3 pitch =    mat3(              1,                 0,                 0, // around x axis
                                       0,  cos(pitch_angle), -sin(pitch_angle),
                                       0,  sin(pitch_angle),  cos(pitch_angle));

    mat3 yaw =      mat3( cos(yaw_angle),                 0,  sin(yaw_angle), // around y axis
                                       0,                 1,               0,
                         -sin(yaw_angle),                 0,  cos(yaw_angle));

    mat3 roll =     mat3( cos(roll_angle), -sin(roll_angle),               0, // around z axis
                          sin(roll_angle),  cos(roll_angle),               0,
                                        0,                0,               1);

    mat3 transform = roll*pitch*yaw;
    //direction = transform * direction;
    
    ApplyRotationToVector(totalRotation, direction);
}
