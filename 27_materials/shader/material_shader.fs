#version 330

in vec3 FragPos;
in vec3 normal;
in vec2 TextCoord;

uniform sampler2D texture0; // unused

struct LightSource{
    vec3 position;
    vec3 color;
};

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess; 
};

uniform LightSource light;
uniform Material material;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    // ambient 
    vec3 ambient = material.ambient * light.color;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm , lightDir) , 0.0);

    vec3 diffuse = diff * material.diffuse * light.color;

    // specular
    float specular_strength = 0.2;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir , norm);

    float spec = pow(max(dot(viewDir , reflectDir) , 0.0) , material.shininess);

    vec3 specular = spec * specular_strength * material.specular * light.color;

    // output
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result , 1.0); 
}

