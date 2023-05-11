#version 460 core
out vec4 FragColor;

uniform vec4 ObjColor;
uniform float TestVal;

void main() {
    // FragColor = vertexColor * (abs(time * 2 - 1)*0.75 + 0.25) ;
    //FragColor = vec4(0.0f, 1.0f, TestVal, 1.0f);
    FragColor = ObjColor;
};