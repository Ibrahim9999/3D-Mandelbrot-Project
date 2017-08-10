#version 130

out vec2 texturecoord;

void main() {
    if (gl_VertexID == 0) {
        texturecoord = vec2(1.0, 1.0);
        gl_Position = vec4(1.0, 1.0, 0.0, 1.0);
    }
    else if (gl_VertexID == 1) {
        texturecoord = vec2(0.0, 1.0);
        gl_Position = vec4(-1.0, 1.0, 0.0, 1.0);
    }
    else if (gl_VertexID == 2) {
        texturecoord = vec2(0.0, 0.0);
        gl_Position = vec4(-1.0, -1.0, 0.0, 1.0);
    }
    else if (gl_VertexID == 3) {
        texturecoord = vec2(1.0, 0.0);
        gl_Position = vec4(1.0, -1.0, 0.0, 1.0);
    }
}
