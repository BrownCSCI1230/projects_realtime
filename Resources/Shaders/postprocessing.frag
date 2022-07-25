#version 330 core

in vec2 texCoords;
uniform sampler2D screenTexture;
out vec4 fragColor;
uniform bool invertColors;
uniform bool sharpenImage;

const float offset = 1.0 / 300.0;

vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );

float sharpenKernel[9] = float[](
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);

vec3 sharpen(){
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * sharpenKernel[i];

    return col;
}

vec3 invert(vec3 color){
    return vec3(1.0f - color.r, 1.0f - color.g, 1.0f - color.b);
}

void main() {
    fragColor = texture(screenTexture, texCoords);
    if(sharpenImage){
        fragColor = vec4(sharpen(), 1.0);
        //fragColor = vec4(1.0);
    }
    if(invertColors){
        fragColor = vec4(invert(fragColor.xyz), 1.0);
    }
}
