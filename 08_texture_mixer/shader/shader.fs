#version 330

in vec2 textCoord;
out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    FragColor = mix(texture(texture0 , textCoord) , texture(texture1,textCoord) , 0.5);
}

