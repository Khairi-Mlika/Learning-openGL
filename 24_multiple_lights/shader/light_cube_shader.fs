#version 330

in vec3 FragPos;
in vec3 normal;
in vec2 TextCoord;

uniform sampler2D texture0;
uniform vec3 Color;

out vec4 FragColor;

void main()
{
    FragColor = vec4(Color , 1.0);
}