#version 300 es

precision mediump float;

out vec4 FragColor;

in vec4 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPosition;

uniform sampler2D Texture;

vec3 lightColor = vec3(1,1,1);
vec3 lightPos = vec3(2,5,0);
float ambient = 0.1;

vec4 calculateLighting(vec4 col)
{
    vec4 o = col * vec4(lightColor,1);
    vec3 n = normalize(normal);
    vec3 lightDir = normalize(lightPos-currentPosition);

    float diffuse = max(dot(n, lightDir), 0.f) + ambient;

    o *= vec4(diffuse, diffuse, diffuse, 1);

    return o;
}

void main()
{
    FragColor = calculateLighting(texture(Texture, texCoord)+color);
}