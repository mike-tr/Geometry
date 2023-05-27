#version 460 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;
in vec4 centerTransformed;


uniform float time;
uniform vec4 center;
uniform sampler2D mainTex;
uniform sampler2D blendTex;

void main() {
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    //FragColor = mix(texture(mainTex, TexCoord), texture(blendTex, TexCoord), 0.33) * vertexColor * (abs(time * 2 - 1)*0.75 + 0.25);
    
    // vec2 uv = gl_FragCoord/iResolution.xy - 0.5;

    // float x = gl_FragCoord.x;
    // float distance = distance(vec3(gl_FragCoord.x, gl_FragCoord.y, 0), vec3(0.5, 0.5, 0));
    // vec4 color = vec4(distance, distance, distance, 1.0);

    FragColor = center;
};