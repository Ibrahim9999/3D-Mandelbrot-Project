#version 130

uniform vec3 FOV;
uniform vec3 cameradir;
uniform vec4 totalRotation;

out vec3 direction;

void getEulerFromVec(in vec4 rotation, inout float yaw, inout float pitch, inout float roll) {
    float w = rotation.w;
    float x = rotation.x;
    float y = rotation.y;
    float z = rotation.z;

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

    float yaw_angle = cameradir.x;
    float pitch_angle = cameradir.y;
    float roll_angle = cameradir.z;

    getEulerFromVec(totalRotation, yaw_angle, pitch_angle, roll_angle);

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
    direction = transform * direction;
}
