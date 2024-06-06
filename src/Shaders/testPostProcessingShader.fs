#version 450 core

precision highp float;

uniform sampler2D glgeMainImage;
uniform sampler2D glgeNormalMap;
uniform sampler2D glgePositionMap;
uniform sampler2D glgeRoughnessMap;
uniform sampler2D glgeDepthMap;

#include <glgeCamera>

uniform vec2 glgeWindowSize;

in vec4 color;
in vec2 texCoords;

out vec4 FragColor;

const float gamma = 2.2f;
const float exposure = 1.0f;

vec2 camDir;
vec2 uv;
float aspect;
vec3 normal;
vec3 pos;
vec3 viewPos;
const mat4 viewMat = glgeCamTransfMat * glgeCamRotMat;
const mat4 invView = inverse(viewMat);

//settings for lens distortion
float lenseDistStrength = 0.f;
vec2 lenseDistCenter = vec2(0.5);
vec2 lenseDistZoom = vec2(1);
//compute the lense distortion
void lesneDistortion()
{
    //simple lense distortion effect
    vec2 fUV = texCoords - lenseDistCenter;
    float uva = atan(fUV.x, fUV.y);
    float uvd = length(fUV);
    uvd = uvd*(1.0 + lenseDistStrength*uvd*uvd);
    uv = lenseDistCenter + vec2(sin(uva), cos(uva))*uvd;
    uv *= lenseDistZoom;
}

//Settings for the vingette effect
vec3 vinCol = vec3(0);
vec2 vinPos = vec2(0.5);
float vinStren = 0.f;
bool vinCircle = false;
//compute the vingette effect
float vingette()
{
    //compute the vingette position
    vec2 pos = texCoords - vinPos;
    //if the vingette sould always be a circle, correct the apsect, else do nothing
    pos.x *= vinCircle ? aspect : 1.f;
    //compute the vingette effect strengt
    float vin = sqrt(dot(pos,pos)) * vinStren;
    vin = (vin*vin)+1.f;
    vin = 1.f / (vin*vin);
    //return the effects strength clamped between 0 and 1
    return clamp(vin, 0.f, 1.f);
}

//function for barrel distortion, used for chromatic aberration
vec2 distBarrelCA(vec2 uv, float amt)
{
	vec2 cc = uv - 0.5;
	float dist = dot(cc, cc);
	return (uv + cc * dist * amt) * lenseDistZoom;
}
//Settings for chromatic aberration
float chromStrength = 0.f;
//compute the chromatic aberration
vec3 chromaticAberration()
{
    //compute the red channel
    float chromR = texture(glgeMainImage, uv).r;
    //compute the green channel
    float chromG = texture(glgeMainImage, distBarrelCA(uv, chromStrength)).g;
    //compute the blue channel
    float chromB = texture(glgeMainImage, distBarrelCA(uv, chromStrength*2.f)).b;
    //return the final color
    return vec3(chromR, chromG, chromB);
}

//settings for the posterization
float postColAmount = 0.f;
//compute the posterization
void posterization(inout vec3 col)
{
    if (postColAmount == 0.f) {return;}
    col *= postColAmount;
    col = vec3(int(col.x),int(col.y),int(col.z));
    col /= postColAmount;
}

//settings for the crt effect
int crtLineAmount = 0;
int crtLineSize = 0;
float crtLineOpaquness = 1.f;
//compute the crt effect
float CRTscanlines()
{
    //compute the pixel y position
    int py = int(texCoords.y * glgeWindowSize.y);
    //mod the y position by the line amount
    int modPY = py % crtLineAmount;
    //use the step function to determin wich lines are shown and wich not
    return 1.f - clamp((1.f - step(crtLineSize, modPY)) - (1.f - crtLineOpaquness), 0.f, 1.f);
}

void main()
{
    //compute the lense distortion
    if (lenseDistStrength != 0.f) {lesneDistortion();}
    else {uv = texCoords;}
    //compute helper variables
    aspect = glgeWindowSize.x / glgeWindowSize.y;
    camDir = uv * 2.f - 1.f;
    normal = texture(glgeNormalMap, texCoords).rgb;
    pos = texture(glgePositionMap, texCoords).rgb;
    viewPos = (vec4(pos,1) * viewMat).xyz;
    //read the pixels data from the g-buffer
    vec3 roughData = texture(glgeRoughnessMap, uv).rgb;
    //get the color and compute the chromatic aberration
    vec3 col;
    if (chromStrength > 0.f) {col = chromaticAberration();}
    else {col = texture(glgeMainImage, uv).rgb;}
    //apply tone mapping if the object is lit
    if (roughData.b == 1.f)
    {
        //tone mapping
        col = vec3(1.0) - exp(-col * exposure);
        //gamma correction
        col = pow(col, vec3(1.0 / gamma));
    }
    //compute the postterization
    posterization(col);
    //add the crt effect
    col *= CRTscanlines();
    //add the vingette effect
    col = mix(vinCol, col, vingette());
    //output the final color to the screen
    FragColor = vec4(col, 1.f);
}