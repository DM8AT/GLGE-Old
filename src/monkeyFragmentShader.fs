#version 300 es

#define PI 3.14159265

precision highp float;//highp float;

out vec4 FragColor;

in vec4 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPosition;

//mesh data
float emmission = 0.f;
float roughness = 0.05;

uniform sampler2D Texture;

uniform vec3 cameraPos;

//light data
vec3 lightColor = vec3(1,1,1);
float lightStrength = 10.f;
vec3 lightPos = vec3(2,5,0);

float gamma = 2.2f;

vec3 schlickFresnel(float vDotH)
{
    vec3 F0 = vec3(0.04);

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
    vec3 lightIntensity = lightColor * lightStrength;

    vec3 l = vec3(0.f);

    l = lightPos - currentPosition;
    float lightToPixelDist = length(l);
    l = normalize(l);
    lightIntensity /= (lightToPixelDist * lightToPixelDist);

    vec3 n = normal;
    vec3 v = normalize(cameraPos - currentPosition);
    vec3 h = normalize(v+l);

    float nDotH = max(dot(n, h), 0.005f);
    float vDotH = max(dot(v, h), 0.005f);
    float nDotL = max(dot(n, l), 0.005f);
    float nDotV = max(dot(n, v), 0.005f);

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
    vec4 totalLight = calculatePBR(col);

    totalLight = totalLight / (totalLight + vec4(1.f));

    vec4 finalLight = vec4(pow(vec3(totalLight), vec3(1.0/2.2)), col.w);

    return finalLight;
}

void main()
{
    vec4 col = texture(Texture, texCoord)+color;
    FragColor = calculateLighting(col);

    FragColor = min(max(FragColor, vec4(0,0,0,0)), vec4(1,1,1,1));
}