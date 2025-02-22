#version 460 core

layout ( location = 0 ) in vec3 aPos;

uniform mat4 pvm = mat4(1.0);

void main()
{
    gl_Position = pvm * vec4(aPos, 1.0);
}