#version 330

in vec3 FragPos;
in vec3 normal;
in vec2 TextCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 viewPos;

out vec4 FragColor; 

void main()
{
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 tex = texture(texture0 , TextCoord).rgb;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm , lightDir) , 0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = texture(texture1 , TextCoord).r;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir , norm);

    float spec = pow(max(dot(viewDir , reflectDir) , 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * tex;

    FragColor = vec4(result , 1.0);
}