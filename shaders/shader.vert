#version 400

uniform vec3 FOV;

in int gl_VertexID;
in int gl_InstanceID;

out vec3 direction;

void main() {
    if (gl_VertexID == 0) {
        FOVedge = FOV;
    }
    else if (gl_VertexID == 1) {
        FOVedge = vec3(-FOV.x, FOV.yz);
    }
    else if (gl_VertexID == 2) {
        FOVedge = vec3(-FOV.x, -FOV.y, FOV.z);
    }
    else {
        FOVedge = vec3(FOV.x, -FOV.y, FOV.z);
    }
}
