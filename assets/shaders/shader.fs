#version 460 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform float time;
uniform sampler2D mainTex;
uniform sampler2D blendTex;

void main() {
    // FragColor = vertexColor * (abs(time * 2 - 1)*0.75 + 0.25) ;
    FragColor = mix(texture(mainTex, TexCoord), texture(blendTex, TexCoord), 0.33) * vertexColor * (abs(time * 2 - 1)*0.75 + 0.25);
};