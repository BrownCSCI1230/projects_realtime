#version 330 core

layout (location = 0) in vec3 objSpace_pos;
layout (location = 1) in vec3 objSpace_norm;
layout (location = 2) in vec2 uv_coords;

uniform mat4 model, view, projection;

out vec2 uv;
out vec4 camSpace_pos;
out vec4 camSpace_norm;

void main() {
    uv = uv_coords;
    camSpace_pos = view*model*vec4(objSpace_pos, 1.0);
    camSpace_norm = transpose(inverse(view*model))*vec4(objSpace_norm, 0.0);
    //camSpace_pos = vec4(objSpace_pos, 1.0);
    //camSpace_norm = vec4(objSpace_norm, 0.0);

    gl_Position = projection*view*model*vec4(objSpace_pos, 1.0);
    //gl_Position = vec4(objSpace_pos, 1.0);
}
