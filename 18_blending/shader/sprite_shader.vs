#version 330

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

out vec2 TextCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos , 0.0 , 1.0);
    TextCoord = aUV;
}