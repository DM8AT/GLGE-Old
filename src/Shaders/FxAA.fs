/**
 * great article for FxAA: https://blog.simonrodriguez.fr/articles/2016/07/implementing_fxaa.html
*/
#version 450 core

out vec4 FragCol;

in vec2 texCoords;

#include <glgeCamera>
uniform sampler2D glgeMainImage;
uniform sampler2D glgePositionMap;
uniform sampler2D glgeNormalMap;
uniform sampler2D glgeRoughnessMap;
uniform sampler2D glgeDepthMap;

//settings
//minimum threashold for an edge, range:
//   0.0312 - quality
//   0.0625 - fast / quality
//   0.0833 - fast
float FXAA_THRESHOLD_MIN = 0.0312;

//maximum threashold for an edge, range:
//   0.333 - too little (faster)
//   0.250 - low quality
//   0.166 - quality / speed
//   0.125 - high quality 
//   0.063 - overkill (slower)
float FXAA_THRESHOLD_MAX = 0.125;
//amount of iterations to do for the edge search
int FXAA_ITERATIONS = 12;
//constant quality speedup
const float FXAA_QUALITY[] = float[](1.f,1.f,1.f,1.f,1.f,1.5f, 2.f, 2.f, 2.f, 2.f, 4.f, 8.f);
//the amount of subpixel anti aliasing
float FXAA_SUBPIXEL = 0.75;

float calcLuma(vec3 rgb)
{
    //compute the percived luminance of the color
    return dot(rgb, vec3(0.299, 0.587, 0.114));
}

void main()
{
    //compute the inverse texture size (multiplication is often faster than division)
    vec2 invTexSize = 1.f / textureSize(glgeMainImage, 0);

    //sample all the axis
    vec3 rgbN = texture(glgeMainImage, texCoords + (vec2( 0, 1)*invTexSize)).rgb;
    vec3 rgbE = texture(glgeMainImage, texCoords + (vec2( 1, 0)*invTexSize)).rgb;
    vec3 rgbS = texture(glgeMainImage, texCoords + (vec2( 0,-1)*invTexSize)).rgb;
    vec3 rgbW = texture(glgeMainImage, texCoords + (vec2(-1, 0)*invTexSize)).rgb;
    //sample the own pixel
    vec3 rgbM = texture(glgeMainImage, texCoords).rgb;

    //compute luminance for direct neigbours
    float lumaN  = calcLuma(rgbN);
    float lumaE  = calcLuma(rgbE);
    float lumaS  = calcLuma(rgbS);
    float lumaW  = calcLuma(rgbW);
    //compute luminance for own pixel
    float lumaM  = calcLuma(rgbM);

    //get the minimum luminance of all samples
    float lumaMin = min(lumaM, min(min(lumaN, lumaE), min(lumaS, lumaW)));
    //get the maximum luminance of all samples
    float lumaMax = max(lumaM, max(max(lumaN, lumaE), max(lumaS, lumaW)));
    //compute the range of the luminance
    float lumaRange = lumaMax - lumaMin;

    //check if this pixel should be blended
    if (lumaRange < max(FXAA_THRESHOLD_MIN, FXAA_THRESHOLD_MAX*lumaMax))
    {
        FragCol = vec4(rgbM,1);
        return;
    }

    //sample the diagonal axes
    vec3 rgbNW = texture(glgeMainImage, texCoords + (vec2(-1.0,-1.0)*invTexSize)).rgb;
    vec3 rgbNE = texture(glgeMainImage, texCoords + (vec2( 1.0,-1.0)*invTexSize)).rgb;
    vec3 rgbSW = texture(glgeMainImage, texCoords + (vec2(-1.0, 1.0)*invTexSize)).rgb;
    vec3 rgbSE = texture(glgeMainImage, texCoords + (vec2( 1.0, 1.0)*invTexSize)).rgb;
    //compute luminance for diagonal neighbours
    float lumaNW = calcLuma(rgbNW);
    float lumaNE = calcLuma(rgbNE);
    float lumaSW = calcLuma(rgbSW);
    float lumaSE = calcLuma(rgbSE);

    //combine the edge's lumas for the vertical edges
    float lumaVert = lumaN + lumaS;
    //combine the edge's lumas for the horizontal edges
    float lumaHori = lumaE + lumaW;

    //combine the luma values for the corners
    //for the left corners
    float lumaLeftC = lumaNW + lumaSW;
    //for the down corners
    float lumaDownC = lumaSE + lumaSW;
    //for the right corners
    float lumaRighC = lumaNE + lumaSE;
    //for the up corners
    float lumaUpC   = lumaNE + lumaNW;

    //compute an estimate for the gradient along the horizontal edge
    float edgeHori =  abs(-2.f * lumaW + lumaLeftC) + abs(-2.f * lumaM + lumaVert) * 2.f + abs(-2.f * lumaE + lumaRighC);
    //compute an estimate for the gradient along the vertical edge
    float edgeVert =    abs(-2.f * lumaN + lumaUpC) + abs(-2.f * lumaM + lumaHori) * 2.f + abs(-2.f * lumaS + lumaDownC);

    //store if the edge is horizontal or vertical
    bool isHori = edgeHori >= edgeVert;

    //select the two neighboring texels lumas in the opposite direction to the local edge.
    float luma1 = isHori ? lumaS : lumaW;
    float luma2 = isHori ? lumaN : lumaE;
    //compute gradients in this direction.
    float gradient1 = luma1 - lumaM;
    float gradient2 = luma2 - lumaM;

    //get the steepest direction
    bool is1Steepest = abs(gradient1) >= abs(gradient2);

    //compute the gradient in the corresponding direction
    float gradientScaled = max(abs(gradient1),abs(gradient2)) * (1.f / 4.f);

    //choose the step size (one pixel) according to the edge direction.
    float stepLength = isHori ? invTexSize.y : invTexSize.x;

/**
 * Code from : https://blog.simonrodriguez.fr/articles/2016/07/implementing_fxaa.html, 
 * all creadit to Simonrodriguez
*/

    //average luma in the correct direction.
    float lumaLocalAverage = 0.0;

    //check if the the first computed direction was the steepest
    if(is1Steepest)
    {
        //invert the sign
        stepLength = - stepLength;
        //store the local average luma
        lumaLocalAverage = 0.5*(luma1 + lumaM);
    }
    else
    {
        //don't invert the sign and store the local average lum
        lumaLocalAverage = 0.5*(luma2 + lumaM);
    }

    //shift UV in the correct direction by half a pixel.
    vec2 uv = texCoords;
    //check if the shift should occour horizontal
    if(isHori)
    {
        //shift up / down by half a pixel
        uv.y += stepLength * 0.5;
    }
    else
    {
        //shift left / right by half a pixel
        uv.x += stepLength * 0.5;
    }

    //compute offset (for each iteration step) in the right direction.
    vec2 offset = isHori ? vec2(invTexSize.x,0.0) : vec2(0.0,invTexSize.y);
    //compute UVs to explore on each side of the edge, orthogonally. The QUALITY allows us to step faster.
    vec2 uv1 = uv - offset;
    vec2 uv2 = uv + offset;

    //read the lumas at both current extremities of the exploration segment
    float lumaEnd1 = calcLuma(texture(glgeMainImage,uv1).rgb);
    float lumaEnd2 = calcLuma(texture(glgeMainImage,uv2).rgb);
    //compute the delta wrt to the local average luma
    lumaEnd1 -= lumaLocalAverage;
    lumaEnd2 -= lumaLocalAverage;

    //if the luma deltas at the current extremities are larger than the local gradient, we have reached the side of the edge.
    bool reached1 = abs(lumaEnd1) >= gradientScaled;
    bool reached2 = abs(lumaEnd2) >= gradientScaled;
    bool reachedBoth = reached1 && reached2;

    // If the side is not reached, we continue to explore in this direction.
    if(!reached1)
    {
        uv1 -= offset;
    }
    if(!reached2)
    {
        uv2 += offset;
    }

    //if both sides have not been reached, continue to explore.
    if(!reachedBoth)
    {
        //iterate as long as the safty limit is not met
        for(int i = 2; i < FXAA_ITERATIONS; i++)
        {
            //if needed, read luma in 1st direction, compute delta.
            if(!reached1)
            {
                lumaEnd1 = calcLuma(texture(glgeMainImage, uv1).rgb);
                lumaEnd1 = lumaEnd1 - lumaLocalAverage;
            }
            //if needed, read luma in opposite direction, compute delta.
            if(!reached2)
            {
                lumaEnd2 = calcLuma(texture(glgeMainImage, uv2).rgb);
                lumaEnd2 = lumaEnd2 - lumaLocalAverage;
            }
            //if the luma deltas at the current extremities is larger than the local gradient, we have reached the side of the edge.
            reached1 = abs(lumaEnd1) >= gradientScaled;
            reached2 = abs(lumaEnd2) >= gradientScaled;
            reachedBoth = reached1 && reached2;

            //if the side is not reached, we continue to explore in this direction, with a variable quality.
            if(!reached1)
            {
                uv1 -= offset * FXAA_QUALITY[i];
            }
            if(!reached2)
            {
                uv2 += offset * FXAA_QUALITY[i];
            }

            //if both sides have been reached, stop the exploration.
            if(reachedBoth){ break; }
        }
    }

    //compute the distances to each extremity of the edge.
    float distance1 = isHori ? (texCoords.x - uv1.x) : (texCoords.y - uv1.y);
    float distance2 = isHori ? (uv2.x - texCoords.x) : (uv2.y - texCoords.y);

    //in which direction is the extremity of the edge closer
    bool isDirection1 = distance1 < distance2;
    float distanceFinal = min(distance1, distance2);

    //length of the edge.
    float edgeThickness = (distance1 + distance2);

    //UV offset: read in the direction of the closest side of the edge.
    float pixelOffset = - distanceFinal / edgeThickness + 0.5;

    //is the luma at center smaller than the local average
    bool islumaMSmaller = lumaM < lumaLocalAverage;

    //if the luma at center is smaller than at its neighbour, the delta luma at each end should be positive (same variation).
    //(in the direction of the closer side of the edge.)
    bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0) != islumaMSmaller;

    //if the luma variation is incorrect, do not offset.
    float finalOffset = correctVariation ? pixelOffset : 0.0;

/**
 * Simonrodriguez code block ends here
*/

    //Subpixel anti-aliasing
    
    //compute the weighted average luma of all 3x3 samples
    float lumaAv = (1.f/12.f) * (2.f * (lumaVert + lumaHori) + lumaUpC + lumaDownC);

    //compute the first part of the subpixel offset by saturating the global average
    float suboff = clamp(abs(lumaAv - lumaM) / lumaRange,0.f,1.f);
    //comute the second part of the subpixel offset by basicaly applying a squared modified smoothstep
    suboff = (-2.f * suboff + 3.f) * suboff * suboff;

    //compute the final subpixel offset
    float subpixFin = suboff * suboff * FXAA_SUBPIXEL;

    //compute the final offset by choosing the maximum of the subpixel offset and the previous offset
    finalOffset = max(finalOffset, subpixFin);

    //store the final uv sample coordinate
    vec2 finUV = texCoords;
    //check if the edge is horizontal
    if(isHori)
    {
        //if it is, change the y-coordinate by the offset
        finUV.y += finalOffset * stepLength;
    }
    else
    {
        //else, change the x-coordinate by the offset
        finUV.x += finalOffset * stepLength;
    }

    //sample the color and use it as the fragment color
    FragCol = vec4(texture(glgeMainImage, finUV).rgb,1);
}