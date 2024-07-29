#version 450 core

in vec2 fTexCoord;

out vec4 FragCoord;

uniform float speed;
const float maxSpeed = 10.f;
const float minSpeed = 0.f;

void main()
{
    float cutoff = speed / (maxSpeed - minSpeed);
    float alpha = step(cutoff, fTexCoord.x);
    FragCoord = vec4(1,0,0,1.f - alpha);
}