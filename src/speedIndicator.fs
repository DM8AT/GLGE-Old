#version 450 core

in vec2 fTexCoord;

layout(location = 0) out vec4 Albedo;
layout(location = 1) out vec4 Normal;
layout(location = 2) out vec4 Position;
layout(location = 3) out vec4 Roughness;
layout(location = 5) out vec4 DepthAndAlpha;

uniform float speed;
const float maxSpeed = 10.f;
const float minSpeed = 0.f;

void main()
{
    float cutoff = speed / (maxSpeed - minSpeed);
    float alpha = step(cutoff, fTexCoord.x);
    if (alpha == 1.f)
    {
        discard;
    }
    Albedo = vec4(1,0,0,1);
    Normal = vec4(0,0,0,1);
    Position = vec4(0,0,0,1);
    Roughness = vec4(0,0,0,1);
    DepthAndAlpha = vec4(0,0,0,1);
}