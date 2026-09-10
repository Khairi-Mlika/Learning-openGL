#version 330

struct LightPoint{
    vec3 lightPos;
    vec3 lightColor;
};

in vec3 FragPos;
in vec3 normal;
in vec2 TextCoord;

uniform sampler2D texture0;

uniform LightPoint lightPoints[3];

uniform vec3 viewPos;

out vec4 FragColor;

vec3 calcAmbient(LightPoint lightPoint)
{
    float ambient_strength = 0.2;
    return ambient_strength * lightPoint.lightColor;
}

vec3 calcDiffuse(LightPoint lightPoint , vec3 normal)
{
    vec3 lightDir = normalize(lightPoint.lightPos - FragPos);

    float diff = max(dot(normal,lightDir) , 0.0);

    return diff * lightPoint.lightColor;
}

vec3 calcSpecular(LightPoint lightPoint , vec3 normal , vec3 viewPos)
{
    float specular_strength = 0.5;

    vec3 lightDir = normalize(lightPoint.lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir , normal);

    float spec = pow(max(dot(viewDir,reflectDir) , 0.0), 32.0);

    return spec * specular_strength * lightPoint.lightColor;
}

void main()
{
    vec3 tex = texture(texture0 , TextCoord).rgb;

    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 norm = normalize(normal);

    for (int i = 0 ; i < 3 ; i++)
    {
        ambient += calcAmbient(lightPoints[i]);
        diffuse += calcDiffuse(lightPoints[i] , norm);
        specular += calcSpecular(lightPoints[i] , norm , viewPos);
    }

    vec3 result = (ambient + diffuse + specular) * tex;

    FragColor = vec4( result * tex , 1.0);
}