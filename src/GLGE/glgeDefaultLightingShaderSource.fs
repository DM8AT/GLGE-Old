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
    vec3 V = normalize(glgeCameraPos - pos);

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
    vec3 color = ambient + Lo;
	
    return (color);
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
    color = texture(glgeAlbedoMap, texCoords);
    normal = (texture(glgeNormalMap, texCoords).xyz - vec3(0.5,0.5,0.5)) * vec3(2,2,2);
    pos = texture(glgePositionMap, texCoords).xyz;
    roughness  = texture(glgeRoughnessMap, texCoords).r;
    metallic = texture(glgeRoughnessMap, texCoords).g;

    if (normal == vec3(-5,-5,-5) || (glgeActiveLights == 0))
    {
        FragColor.rgb = -color.rgb;
    }
    else
    {
        FragColor = vec4(calculateLightingPBR(color.rgb), color.w);
    }
}