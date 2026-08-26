#version 330

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

out vec2 textCoord;

void main()
{
    gl_Position = vec4(aPos , 0.0 , 1.0);
    textCoord = aUV;
}