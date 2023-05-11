#version 460 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;
in vec3 coord;

uniform float time;
uniform vec3 center;
uniform float dist;
uniform sampler2D mainTex;
uniform sampler2D blendTex;

void main() {
    // FragColor = vertexColor * (abs(time * 2 - 1)*0.75 + 0.25) ;
    vec3 diff = center - coord;
    if(dot(diff,diff) > dist * dist){
        FragColor = vec4(0.0,0.0,0.0,0.0);
    }else{
        FragColor = mix(texture(mainTex, TexCoord), texture(blendTex, TexCoord), 0.8) * vertexColor * (abs(time * 2 - 1)*0.75 + 0.25);
    }
};