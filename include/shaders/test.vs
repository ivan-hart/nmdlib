R"(
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 pvm;

out vec4 fragColor;

void main() {
    gl_Position = pvm * vec4(aPos, 1.0);
    fragColor = vec4(aColor, 1.0);
}
)"
