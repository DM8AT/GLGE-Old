#version 300 es

#define PI 3.14159265

precision highp float;

out vec4 FragColor;

in vec4 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPos;

//mesh data
float emmission = 0.2f;
uniform float roughness;

uniform sampler2D AmbientMap;
uniform sampler2D NormalMap;
uniform sampler2D RoughnessMap;
uniform sampler2D Texture;

float exposure = 0.01;

uniform int usedTextures;

uniform bool NormalMapIsActive;
uniform bool RoughnessMapIsActive;

uniform vec3 cameraPos;

//light data
uniform vec3 lightColor[255];
uniform float lightInt[255];
uniform vec3 lightPos[255];
uniform float activeLights;

float ambient = 0.1;

float gamma = 2.2f;

int iteration = 0;

float rough;

vec3 schlickFresnel(float vDotH)
{
    vec3 F0 = vec3(0.04f);

    vec3 ret = F0 + (1.f - F0) * pow(clamp(1.f - vDotH, 0.f, 1.f), 5.f);

    return ret;
}

float geomSmith(float dp)
{
    float k = (rough + 1.f) * (rough + 1.f) / 8.f;
    float denom = dp * (1.f - k) + k;
    return dp / denom;
}


float ggxDistribution(float nDotH)
{
    float alpha2 = rough * rough * rough * rough;
    float d = nDotH * nDotH * (alpha2 - 1.f) + 1.f;
    float ggxdistrib = alpha2 / (PI * d * d);
    return ggxdistrib;
}

vec4 calculatePBR(vec4 col)
{
    vec3 lightIntensity = lightColor[iteration] * lightInt[iteration];

    vec3 l = vec3(0.f);

    l = lightPos[iteration] - currentPos;
    float lightToPixelDist = length(l);
    l = normalize(l);
    lightIntensity /= (lightToPixelDist * lightToPixelDist);

    vec3 n = normalize(normal);

    if (NormalMapIsActive)
    {
        n = normalize(normal + (texture(NormalMap, texCoord).rgb * 2.f - 1.f));
    }

    vec3 v = normalize((cameraPos) - currentPos);
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

    float SpecBRDF = float(SpecBRDFnom) / SpecBRDFdenom;

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
    vec4 col = texture(Texture, texCoord);

    if (usedTextures > 1)
    {
        col = vec4(vec3(col * texture(AmbientMap, texCoord).r), col.w);
    }

    col += color;

    if (RoughnessMapIsActive)
    {
        rough = texture(RoughnessMap, texCoord).r;
    }
    else
    {
        rough = roughness;
    }

    //apply tonemapping
    col = vec4(vec3(1.f) - exp(-vec3(col) * exposure),col.w);

    if (activeLights == 0.f)
    {
        FragColor = col;
    }
    else
    {
        FragColor = calculateLighting(col);
    }

    vec4 minColor = col * ambient;

    FragColor = min(max(FragColor, minColor), vec4(1,1,1,1));
}