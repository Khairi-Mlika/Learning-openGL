#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 lightSpace;

void main()
{
    gl_Position = lightSpace * model * vec4(aPos , 1.0);
}