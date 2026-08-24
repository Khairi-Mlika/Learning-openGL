#version 330

in vec2 pos;

out vec4 FragColor;

float f(float x){
    return x * x * 10;
}

void main(){
    vec2 uv ;

    uv.x = pos.x;
    uv.y = pos.y * 0.5 + 0.5;

    float thikness = 0.009;

    bool xAxis = uv.y < thikness;
    bool yAxis = abs(uv.x) < thikness;

    float t = pos.y * 0.5 + 0.5;

    float red;
    float green;

    if (t < 0.5)
    {
        green = 1.0;
        red = t * 2.0;
    }
    else
    {
        green = 1.0 - (t - 0.5) * 2.0;
        red = 1.0;
    }

    if (xAxis || yAxis){
        FragColor = vec4(0.0 , 0.0 , 0.0 , 1.0);
    }
    else if(abs(uv.y - f(uv.x)) <= 0.01) {
        FragColor = vec4(red, green, 0.0, 1.0);
    }
    else{
        FragColor = vec4(1.0 , 1.0 , 1.0 , 1.0);
    }
}