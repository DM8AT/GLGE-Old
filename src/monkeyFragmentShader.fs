#version 300 es

#define PI 3.14159265

precision highp float;

out vec4 FragColor;

in vec4 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPosition;

//mesh data
float emmission = 0.2f;
uniform float roughness;

uniform sampler2D Texture;

uniform vec3 cameraPos;

//light data
uniform vec3 lightColor[255]; // = vec3(1,1,1);
uniform float lightInt[255];// = float[](100.f, 100.f); // = 100.f;
uniform vec3 lightPos[255];// = vec3(2,5,0);
uniform float activeLights;

float gamma = 2.2f;

int iteration = 0;

vec3 schlickFresnel(float vDotH)
{
    vec3 F0 = vec3(0.04f);

    vec3 ret = F0 + (1.f - F0) * pow(clamp(1.f - vDotH, 0.f, 1.f), 5.f);

    return ret;
}

float geomSmith(float dp)
{
    float k = (roughness + 1.f) * (roughness + 1.f) / 8.f;
    float denom = dp * (1.f - k) + k;
    return dp / denom;
}


float ggxDistribution(float nDotH)
{
    float alpha2 = roughness * roughness * roughness * roughness;
    float d = nDotH * nDotH * (alpha2 - 1.f) + 1.f;
    float ggxdistrib = alpha2 / (PI * d * d);
    return ggxdistrib;
}

vec4 calculatePBR(vec4 col)
{
    vec3 lightIntensity = lightColor[iteration] * lightInt[iteration];

    vec3 l = vec3(0.f);

    l = lightPos[iteration] - currentPosition;
    float lightToPixelDist = length(l);
    l = normalize(l);
    lightIntensity /= (lightToPixelDist * lightToPixelDist);

    vec3 n = normal;
    vec3 v = normalize(cameraPos - currentPosition);
    vec3 h = normalize(v+l);

    float nDotH = max(dot(n, h), 0.f);
    float vDotH = max(dot(v, h), 0.f);
    float nDotL = max(dot(n, l), 0.f);
    float nDotV = max(dot(n, v), 0.f);

    vec3 F = schlickFresnel(vDotH);

    vec3 ks = F;
    vec3 kd = 1.f - ks;

    vec3 SpecBRDFnom = ggxDistribution(nDotH) * F * geomSmith(nDotL) * geomSmith(nDotV);

    float SpecBRDFdenom = 4.f * nDotV * nDotL + 0.0001;

    float SpecBRDF = SpecBRDFnom / SpecBRDFdenom;

    vec3 fLambert = vec3(0.f);

    fLambert = vec3(col);

    vec3 DiffuseBRDF = kd * fLambert / PI;

    vec3 FinalColor = (DiffuseBRDF + SpecBRDF) * lightIntensity * nDotL;

    return vec4(FinalColor, col.w);
}

vec4 calculateLighting(vec4 col)
{
    vec4 totalLight;
    for (int i = 0; i < min(int(activeLights), 255); i++)
    {
        iteration = i;
        totalLight += calculatePBR(col);
        totalLight = totalLight / (totalLight + vec4(1.f));
    }

    totalLight /= activeLights;

    vec4 finalLight = vec4(pow(vec3(totalLight), vec3(1.0/2.2)), col.w);

    return finalLight;
}

void main()
{
    vec4 col = texture(Texture, texCoord)+color;
    if (activeLights == 0.f)
    {
        FragColor = col;
    }
    else
    {
        FragColor = calculateLighting(col);
    }

    FragColor = min(max(FragColor, vec4(0,0,0,0)), vec4(1,1,1,1));
}