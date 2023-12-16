#version 400

// Default colour output
layout ( location = 0 ) out vec4 out_Color;

in vec4 ex_Position;
in vec4 ex_PositionWorld;
in vec4 ex_Normal;
in vec4 ex_UV_x2;

uniform vec3 Color;

void main(void)
{    
    // Example lighting calculation (replace with your own lighting model)
    vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0)); // Example light direction

    float lightIntensity = max(dot(ex_Normal.xyz, lightDirection), 0.0);
    lightIntensity = max(lightIntensity, 0.2f);
    // Output color based on the calculated light intensity
    out_Color = vec4(lightIntensity * Color, 1.0);
}
