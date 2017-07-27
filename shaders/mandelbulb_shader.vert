#version 130

uniform vec3 FOV;
uniform vec3 cameradir;

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

    float x = cameradir.x;
    float y = cameradir.y;
    float z = cameradir.z;

    mat3 yaw =      mat3( cos(y),       0,  sin(y),
                               0,       1,       0,
                         -sin(y),       0,  cos(y));
    mat3 roll =     mat3( cos(z), -sin(z),       0,
                          sin(z),  cos(z),       0,
                               0,       0,       1);
    mat3 pitch =    mat3(      1,       0,       0,
                               0,  cos(x), -sin(x),
                               0,  sin(x),  cos(x));

    mat3 transform = roll*yaw*pitch;
    direction = transform * direction;
}

