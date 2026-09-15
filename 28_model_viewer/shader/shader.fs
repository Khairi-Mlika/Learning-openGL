#version 330

in vec2 TextCoord;

uniform sampler2D texture_diffuse1;

out vec4 FragColor;

void main()
{
    FragColor = texture(texture_diffuse1 , TextCoord);
}