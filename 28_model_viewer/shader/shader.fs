#version 330

struct LightPoint{
    vec3 color;
    vec3 position;
};

in vec3 FragPos;
in vec3 N;
in vec3 B;
in vec3 T;
in vec2 TextCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

uniform LightPoint light;
uniform vec3 viewPos;

out vec4 FragColor;

vec3 calcAmbient(vec3 color)
{
    float ambient_strength = 0.5;
    return ambient_strength * color;
}

vec3 calcDiffuse(vec3 norm , vec3 lightDir , vec3 color)
{
    float diff = max(dot(norm , lightDir) , 0.0);
    
    return diff * color;
}

vec3 calcSpecular(vec3 norm , vec3 lightDir , vec3 viewDir , vec3 color)
{
    float specular_strength = 0.2;

    vec3 reflectDir = reflect(-lightDir , norm);

    float spec = pow(max(dot(viewDir,reflectDir) , 0.0) , 32.0);

    return specular_strength * spec * color ;
}

void main()
{
    vec3 tex = texture(texture_diffuse1 , TextCoord).xyz;
    vec3 normal = texture(texture_normal1 , TextCoord).rgb;

    mat3 TBN = mat3(T , B , N);

    normal = normal * 0.2 - 1.0;
    normal = TBN * normal;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 ambient = calcAmbient(light.color);
    vec3 diffuse = calcDiffuse(norm ,lightDir , light.color);
    vec3 specular = calcSpecular(norm , lightDir , viewDir ,light.color);

    vec3 result = ambient + diffuse  + specular;
    FragColor = vec4(result * tex , 1.0);
}