#version 330

struct Light{
    vec3 color;
    vec3 direction;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TextCoord;

in vec4 LightFragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D shadowMap;

uniform Light light;

uniform vec3 viewPos;

out vec4 FragColor;

float ShadowCalculate()
{
    vec3 projCoord = LightFragPos.xyz / LightFragPos.w;
    projCoord = projCoord * 0.5 + 0.5;

    if(projCoord.z > 1.0)
    {
        return 0.0;
    }

    float closestDepth = texture(shadowMap , projCoord.xy).r;
    float currentDepth = projCoord.z;

    vec3 lightDir = normalize(-light.direction);
    vec3 normal = normalize(Normal);

    float bias = max(0.05 * (1.0 - dot(normal , lightDir)), 0.005);

    float shadow = 0.0;
    if (currentDepth - bias > closestDepth)
    {
        shadow = 1.0;
    }

    return shadow;

}

void main()
{
    vec3 tex = texture(texture_diffuse1 , TextCoord).xyz;

    vec3 lightDir = normalize(-light.direction);
    vec3 normal = normalize(Normal);

    //  ambient
    vec3 ambient = 0.15 * light.color;

    // diffuse
    float diff = max(dot(normal,lightDir) , 0.0);

    vec3 diffuse = diff * light.color;

    // specular
    vec3 viewDir =
    normalize(viewPos - FragPos);

    vec3 reflectDir =
        reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir),0.0),32.0);

    vec3 specular = 0.5 * spec * light.color;

    // shadow 
    float shadow = ShadowCalculate();

    // lighting 
    vec3 lighting = ambient + (1.0 - shadow) * ( diffuse + specular);

    FragColor = vec4(lighting * tex , 1.0);
}

