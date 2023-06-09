#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec4 vertexColor;
out vec2 TexCoord;
out vec4 centerTransformed;
uniform float offset;
uniform float time;

uniform sampler2D normalTex;
uniform vec4 center;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // gl_Position = vec4(aPos.x + offset * 2 - 1, aPos.y * (time * 2 - 1), aPos.z, 1.0);
    TexCoord = aTexCoord;
    //vec4 n = texture(normalTex, TexCoord);
    //gl_Position = projection * view * transform * vec4(aPos, 1.0);

    gl_Position = projection * view * transform * vec4(aPos, 1.0);
    centerTransformed = projection * view * transform * vec4(center.x, center.y, center.z , 1.0);

    // float distance = distance(aPos, vec3(center.x, center.y, center.z));
    // if(distance > 0.5){
    //     vertexColor = vec4(0.0, 0.0, 0.0, 1.0);
    // }else{
    //     vertexColor = vec4(aColor, 1.0);
    // }
    vertexColor = vec4(aColor, 1.0);
    //vertexColor = vec4(0.0, 0.0, 0.0, 0.0);
    //vertexColor = vec4(1.0, 1.0, 1.0, 1.0);

};