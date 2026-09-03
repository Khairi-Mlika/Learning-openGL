#version 330

in vec2 TextCoord;
out vec4 FragColor;

uniform sampler2D texture0;

void main()
{
    vec3 white = vec3(1.0);
    FragColor = vec4(white , 1.0);
}