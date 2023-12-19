#version 300 es

#define PI 3.14159265

precision highp float;

layout(location = 3) out vec4 Lit;
layout(location = 5) out vec4 Max;
layout(location = 7) out vec4 Col;

uniform sampler2D glgeLitMap;

in vec4 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPos;

uniform int glgeObjectUUID;
uniform vec2 glgeScreenResolution;
uniform vec4 glgeColor;
uniform float glgeRough;
uniform float glgeMetalic;
uniform bool glgeHasTexture;
uniform sampler2D glgeTexture;
uniform sampler2D glgeAmbientMap;
uniform sampler2D glgeNormalMap;
uniform sampler2D glgeRoughMap;
uniform samplerCube shadowMap;
uniform bool NormalMapIsActive;
uniform bool glgeLit;
uniform bool roughMapIsActive;
uniform vec3 cameraPos;
uniform float farPlane;
uniform bool glgePass;

float rough = 0.f;
vec3 n = vec3(0);

float ambient = 0.1;

int iteration = 0;

float biasAngle = 0.0005;
vec3 actualNormal = vec3(0,0,0);

//light data
uniform vec3 glgeLightColor[255];
uniform float glgeLightInt[255];
uniform vec3 glgeLightPos[255];
uniform int glgeActiveLights;

float lightRadius = 0.1;
float a = 0.f;
float b = 0.f;

vec3 pos;
float roughness;

float metallic = 0.f;

//attenuation values for the light sources
//constant attenuation, isn't influenced by distance
const float constantAttenuation = 1.f;
//linear attenuation, increases linearly as the distance increases
const float linearAttenuation = 0.f;
//quadratic attenuation, increses quadraticaly as the distnace increses
const float quadraticAttenuation = 1.f;

// for more information on pbr-lighting see : https://learnopengl.com/PBR/Lighting
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 calculateLightingPBR(vec3 col)
{
    vec3 N = normalize(normal);
    vec3 V = normalize(cameraPos - pos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, col, metallic);
	           
    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < glgeActiveLights; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(glgeLightPos[i] - pos);
        vec3 H = normalize(V + L);
        float dist    = length(glgeLightPos[i] - pos);
        float attenuation = 1.f/(constantAttenuation + linearAttenuation*dist + quadraticAttenuation*(dist * dist));
        vec3 radiance     = glgeLightColor[i] * attenuation;        
        
        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);        
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	  
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;  
            
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * col / PI + specular) * radiance * NdotL * (glgeLightInt[i]); 
    }   
  
    vec3 ambient = vec3(0.03) * col * ambient;
    vec3 color = ambient + max(Lo, vec3(0));
	
    return (color);
}

void main()
{
    vec2 uv = gl_FragCoord.rg/glgeScreenResolution;

    vec4 col = glgeColor;
    col.rgb += color.rgb;
    col.w += clamp(color.w,0.f,1.f);
    if (glgeHasTexture)
    {
        vec4 texColor = texture(glgeTexture, texCoord);
        col.rgb += texColor.rgb;
        col.w *= clamp(texColor.w,0.f,1.f);
    }
    
    col.w = clamp(col.w, 0.f, 1.f);

    if (col.w == 0.f){discard;}
    if (!glgePass && col.w!=1.f){discard;}
    if (glgePass && col.w==1.f){discard;}

    if (roughMapIsActive)
    {
        rough = texture(glgeRoughMap, texCoord).r;
    }
    else
    {
        rough = glgeRough;
    }
    n = normalize(normal);
    if (NormalMapIsActive)
    {
        n = normalize(normal + (texture(glgeNormalMap, texCoord).rgb));
    }

    if (glgeLit)
    {
        pos = currentPos;
        roughness = rough;
        col.rgb = calculateLightingPBR(col.rgb);
    }

    Col = col;
    Max = vec4(0,0,1,1);
    Lit.b = 1.f;

    gl_FragDepth = gl_FragCoord.z;
}