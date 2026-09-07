#version 330

in vec3 FragPos;
in vec3 normal;
in vec2 TextCoord;

uniform sampler2D texture0;

uniform vec3 lightColor;
uniform vec3 lightPos;

out vec4 FragColor;

void main()
{
    vec3 texture = texture(texture0,TextCoord).rgb;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm , lightDir) , 0);

    vec3 diffuse = diff * lightColor;
    vec3 result = diffuse * texture;

    FragColor = vec4(result , 1.0);
}
