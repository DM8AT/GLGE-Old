#version 300 es

#define PI 3.14159265

precision highp float;

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D glgeAlbedoMap;
uniform sampler2D glgeNormalMap;
uniform sampler2D glgePositionMap;
uniform sampler2D glgeRoughnessMap;

//light data
uniform vec3 glgeLightColor[255];
uniform float glgeLightInt[255];
uniform vec3 glgeLightPos[255];
uniform int glgeActiveLights;

//camera data
uniform vec3 glgeCameraPos;
uniform float glgeFarPlane;

float ambient = 0.1;

float gamma = 2.2f;
float exposure = 1.f;

int iteration = 0;

float biasAngle = 0.0005;
vec3 actualNormal = vec3(0,0,0);

vec4 lightCol[] = vec4[](vec4(1,1,1,1));
vec3 lightPos[] = vec3[](vec3(2,5,0));
float lightInt[] = float[](250.f);

float rough;

float lightRadius = 0.1;
float a = 0.f;
float b = 0.f;

vec4 color;
vec3 normal;
vec3 pos;
float roughness;

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
    float shadow = 1.f;

    vec3 lightIntensity = glgeLightColor[iteration] * glgeLightInt[iteration];

    vec3 l = vec3(0.f);

    l = glgeLightPos[iteration] - pos;
    float lightToPixelDist = length(l);
    l = normalize(l);
    lightIntensity /= (lightToPixelDist * lightToPixelDist);

    vec3 n = normalize(normal);

    vec3 FragNormal = n;

    vec3 v = normalize((glgeCameraPos) - pos);
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

    vec3 FinalColor = ((DiffuseBRDF + SpecBRDF) * lightIntensity * vec3(shadow)) * nDotL;

    return vec4(FinalColor, col.w);
}

vec4 calculateLightingPBR(vec4 col)
{
    vec4 totalLight;
    for (int i = 0; i < min(int(glgeActiveLights), 255); i++)
    {
        iteration = i;
        totalLight += calculatePBR(col);
        totalLight = totalLight / (totalLight + vec4(1.f));
    }

    totalLight /= float(glgeActiveLights);

    vec4 finalLight = vec4(pow(vec3(totalLight), vec3(1.0/2.2)), col.w);

    return finalLight;
}

vec4 calculateLighting(vec4 color, vec3 normal, vec3 pos, float roughness)
{
    vec3 col = color.rgb;

    vec3 lightDir = normalize(glgeLightPos[0] - pos);
    float diff = max(dot(normal.xyz, lightDir), 0.0);

    col *= diff;

    return vec4(col, color.w);
}

void main()
{
    vec4 col = vec4(0,0,0,0);

    color      = texture(glgeAlbedoMap, texCoords);
    normal     = (texture(glgeNormalMap, texCoords).xyz - vec3(0.5,0.5,0.5)) * vec3(2,2,2);
    pos        = texture(glgePositionMap, texCoords).xyz;
    roughness  = texture(glgeRoughnessMap, texCoords).r;

    if (normal == vec3(-5,-5,-5))
    {
        FragColor = color;
    }
    else
    {
        FragColor = calculateLightingPBR(color);
    }

    FragColor = vec4(vec3(1.f) - exp(-vec3(FragColor) * exposure),FragColor.w);
    
    //float grey = (FragColor.r + FragColor.g + FragColor.b) / 3.f;
    //FragColor.rgb = vec3(grey);
}