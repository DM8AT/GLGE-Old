#version 450 core
#extension GL_ARB_bindless_texture : require

precision highp float;

layout(location = 0) out vec4 Albedo;
layout(location = 1) out vec4 Normal;
layout(location = 2) out vec4 Position;
layout(location = 3) out vec4 Roughness;
layout(location = 5) out vec4 DepthAndAlpha;

#include <glgeObject>
#include <glgeCamera>

in vec4 color;
in vec2 texCoord;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec3 fragPos;
in vec3 vPos;

#include <glgeMaterial>

struct FragmentData
{
    vec4 color;
    vec3 pos;
    vec3 normal;
    vec2 uv;
};

FragmentData parallaxMapping(mat3 TBN)
{
    //pack the whole fragment data together
    FragmentData f = FragmentData(color, fragPos, normal, texCoord);

    //check if parallax occlusion mapping should compute
    if (!bool(glgeDisplacementMapActive)) { return f; }
    //if yes, calculate the view direction in tangent space
    vec3 viewDir = normalize((glgeCameraPos*TBN) - (fragPos*TBN));

    //compute the actual amount of depth layers used for the current angle
    float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));
    //compute the difference of depth from one layer to another
    float layerDepth = 1.f / numLayers;
    //store the current depth
    float currentLayerDepth = 0.0;
    //compute the direction the texture coordinates are changing
    vec2 P = viewDir.xy * dispStrength; 
    //compute the difference in texture coords from one step to the next one
    vec2 deltaTexCoords = P * layerDepth;

    //store the current texture coordinates
    vec2 currentTexCoords = f.uv;
    //store the current depth read from the displacement map
    float currentDepthMapValue = texture(sampler2D(glgeDisplacementMap), currentTexCoords).r;
    
    //loop while the depth is less than the displacement map
    while(currentLayerDepth < currentDepthMapValue)
    {
        //displace the texture coordinates
        currentTexCoords -= deltaTexCoords;
        //get the new depth value
        currentDepthMapValue = texture(sampler2D(glgeDisplacementMap), currentTexCoords).r;  
        //increment the depth
        currentLayerDepth += layerDepth;
    }

    //store the amount to step each layer
    float depthStep = -layerDepth / 2.f;
    //binary refinement for the hitpoint
    for (int i = 0; i < binarySteps; i++)
    {
        //go the step
        currentTexCoords -= P * depthStep;
        //go the step in depth
        currentLayerDepth += depthStep;
        //read the depth from the texture
        currentDepthMapValue = texture(sampler2D(glgeDisplacementMap), currentTexCoords).r;
        //divide the depth step by two and switch the sign if needed
        depthStep /= 2.f * sign(currentLayerDepth - currentDepthMapValue);
    }

    //increment the position by the normal multiplied by the depth
    f.pos += currentLayerDepth*f.normal;
    //store the new uv map
    f.uv = currentTexCoords;

    //discard the pixel if the uv map is out of the normal space
    if (f.uv.x > 1.0 || f.uv.y > 1.0 || f.uv.x < 0.0 || f.uv.y < 0.0) { discard; }
    //return the fragment data
    return f;
}

void main()
{
    //compute tangent and bitangent
	vec3 Q1 = dFdx(fragPos);
	vec3 Q2 = dFdy(fragPos);
	vec2 st1 = dFdx(texCoord);
	vec2 st2 = dFdy(texCoord);
	
    //calculate the tangent and bitangent
	vec3 tangent = normalize(Q1*st2.t - Q2*st1.t);
	vec3 bitangent = normalize(-Q1*st2.s + Q2*st1.s);
	
	// the transpose of texture-to-eye space matrix
	mat3 TBN = mat3(tangent,bitangent,normal);

    //compute parallax occlusion mapping, if a depth map is bound
    FragmentData frag = parallaxMapping(TBN);
    //read the color for the object
    vec4 col = glgeColor + frag.color;
    col.rgb *= (1-int(glgeAmbientMapActive));
    col.rgb += texture(sampler2D(glgeAmbientMap), frag.uv).rgb * int(glgeAmbientMapActive);
    //no luck getting arround this if statement. Alpha clipping is enabled by default
    if(col.w < 0.5){discard;}

    //re-compute tangent and bitangent
	Q1 = dFdx(frag.pos);
	Q2 = dFdy(frag.pos);
	st1 = dFdx(frag.uv);
	st2 = dFdy(frag.uv);
	
    //calculate tangent and bitangent again
	tangent = normalize(Q1*st2.t - Q2*st1.t);
	bitangent = normalize(-Q1*st2.s + Q2*st1.s);

    //TBN = mat3(tangent, bitangent, normal);

    //if a normal map is active, get the normal of the normal map and use it, else use vec3(0,0,1)
    vec3 n = mix(vec3(0,0,1),normalize(texture(sampler2D(glgeNormalMap),frag.uv).rgb * 2.f - 1.f),float(glgeNormalMapActive));
    n = normalize(TBN * n);

    //read the roughness and again use multiplikation like for the albedo texture
    float rough = texture(sampler2D(glgeRoughnessMap), frag.uv).r * float(int(glgeRoughnessMapActive));
    //add the default roughness, multiplied with the inverted scalar (like an else, but faster)
    rough += glgeRoughness * (1.f - float(int(glgeRoughnessMapActive)));
    
    //store the color
    Albedo = vec4(col.rgb, vPos.x);
    //store the normal
    Normal = vec4(n, vPos.y);
    //store the position data
    Position = vec4(frag.pos, vPos.z);
    //store the roughness data
    Roughness = vec4(rough,glgeMetalic,int(glgeLit),1);
    //store the depth data and object uuid
    DepthAndAlpha = vec4(gl_FragCoord.z,glgeObjectUUID,0,1);
}