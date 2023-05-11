#version 460 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform float time;
uniform sampler2D mainTex;

void main() {
    // FragColor = vertexColor * (abs(time * 2 - 1)*0.75 + 0.25) ;
    FragColor = texture(mainTex, TexCoord) * vertexColor * (abs(time * 2 - 1)*0.75 + 0.25);
};