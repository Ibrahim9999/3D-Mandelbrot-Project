#version 400

in vec3 direction;

uniform double step;
uniform int bail;
uniform vec3 camera;
uniform vec4 color;

bool mandelTest(in vec3 point) {
    
}

void main() {    
    vec3 pos = camera;
    
    while (!mandelTest(pos))
        pos = pos + step*direction;

    outputF = color/length(pos-camera);
}
