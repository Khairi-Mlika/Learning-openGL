#version 330

in vec3 FragPos;
in vec3 normal;
in vec2 TextCoord;

uniform sampler2D texture0;

struct LightPoint
{
    vec3 lightColor;
    vec3 lightPos;
    vec3 direction;
    float innerCutoff;
    float outerCutoff;
};

uniform LightPoint lightPoint;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    vec3 tex = texture(texture0, TextCoord).rgb;

    // -------------------------
    // Ambient
    // -------------------------
    float ambient_strength = 0.2;
    vec3 ambient = ambient_strength * lightPoint.lightColor;


    // -------------------------
    // Directions
    // -------------------------
    vec3 norm = normalize(normal);

    // Fragment -> light
    vec3 lightDir = normalize(lightPoint.lightPos - FragPos);

    // Light -> fragment
    vec3 fromLight = normalize(FragPos - lightPoint.lightPos);


    // -------------------------
    // Spotlight test
    // -------------------------
    float theta = dot(
        fromLight,
        normalize(lightPoint.direction)
    );


    // -------------------------
    // Diffuse + Specular
    // -------------------------
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);


   
    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    diffuse = diff * lightPoint.lightColor;


    // Specular
    float specular_strength = 0.5;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(
        max(dot(reflectDir, viewDir), 0.0),
        32.0
    );

    specular = spec * specular_strength * lightPoint.lightColor;
    
    float intensity = smoothstep(
        lightPoint.outerCutoff,
        lightPoint.innerCutoff,
        theta
    );

    // -------------------------
    // Final color
    // -------------------------
    vec3 result = ambient + (diffuse + specular) * intensity;

    FragColor = vec4(result * tex, 1.0);
}