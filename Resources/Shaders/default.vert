#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
uniform mat4 model, view, projection;

out vec4 col;

void main() {
   gl_Position = vec4(position, 1.0);

   vec3 colorNorm = 0.5*(normal+vec3(1.0));
   col = vec4(colorNorm, 1.f);
}
