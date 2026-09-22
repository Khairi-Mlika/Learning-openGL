#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightSpace;

out vec3 FragPos;
out vec3 Normal;
out vec2 TextCoord;

out vec4 LightFragPos;

void main()
{
    FragPos = vec3(model * vec4(aPos , 1.0));
    Normal = vec3(transpose(inverse(model))) * aNormal;
    TextCoord = aUV;

    LightFragPos = lightSpace * model * vec4(aPos , 1.0);

    gl_Position = projection * view * model * vec4(aPos , 1.0);

}