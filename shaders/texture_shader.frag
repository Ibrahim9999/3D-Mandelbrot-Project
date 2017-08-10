#version 130

in vec2 texturecoord;

uniform sampler2D drawtexture;

out vec3 outColor;

void main() {
    outColor = texture(drawtexture, texturecoord).rgb;
}
