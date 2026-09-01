#version 330

in vec2 TextCoord;
out vec4 FragColor;

uniform sampler2D texture0;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
    float depth = gl_FragCoord.z;
    FragColor = texture(texture0 , TextCoord);
}