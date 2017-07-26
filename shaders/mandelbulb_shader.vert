#version 140

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
}
