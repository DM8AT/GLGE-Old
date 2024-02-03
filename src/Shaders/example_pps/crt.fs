#version 300 es
precision highp float;
/*

    CRT Shader by JuNi4

 */

// shader settings //

// chromatic aboration strength, for LSD sim, use between .1 and .5
const float chromaticAborationIntensity      = 0.002f;
const vec2  chromaticAborationRedIntensity   = vec2( 0, 1 );
const vec2  chromaticAborationGreenIntensity = vec2( -1,0 );
const vec2  chromaticAborationBlueIntensity  = vec2( 1, 0 );

// weather or not to use the crt scan line emulation
const bool enableScanLines = true;
// the size of the scanlines, the smaller, the better but also float inprecision and stuff
const int scanlineSize = 4;

// Shader stuff //

uniform sampler2D glgeMainImage;
uniform sampler2D glgeRoughnessMap;
uniform vec2 screenSize;

in vec4 color;
in vec2 texCoords;

out vec4 FragColor;

const float gamma = 2.2f;
const float exposure = 1.0f;

float screenHeight = 0.0f;

vec3 colorRemap( vec3 col, float cols )
{
    // remap the colors

    col *= cols;

    col = vec3(int(col.x),int(col.y),int(col.z));

    col /= cols;

    return col;
}

vec3 chromaticAboration( float intensity, vec2 red, vec2 green, vec2 blue )
{
    // chromatic aboration red channel offset
    vec3 colr = texture(glgeMainImage, texCoords + red   *intensity ).rgb;
    // chromatic aboration green channel offset
    vec3 colg = texture(glgeMainImage, texCoords + green *intensity ).rgb;
    // chromatic aboration blue channel offset
    vec3 colb = texture(glgeMainImage, texCoords + blue  *intensity ).rgb;
    // construct final color
    return vec3( colr.x, colg.y, colb.z );
}

void main()
{
    if ( screenSize.y == 0.0f ) { screenHeight = 1080.0f; } else { screenHeight = screenSize.y; }

    // get color of current pixel
    vec3 col = texture(glgeMainImage, texCoords).rgb;
    vec3 mapped = col;


    // if chromatic aboration is active, get responding pixels for different colors
    if ( chromaticAborationIntensity != 0.f ) {
        mapped = chromaticAboration( chromaticAborationIntensity,
                                     chromaticAborationRedIntensity,
                                     chromaticAborationGreenIntensity,
                                     chromaticAborationBlueIntensity );
    }


    vec3 roughData = texture(glgeRoughnessMap, texCoords).rgb;

    //apply tone mapping if the object is lit
    if (roughData.b == 1.f)
    {
        // tone mapping
        mapped = vec3(1.0) - exp(-mapped * exposure);
        // gamma correction
        mapped = pow(mapped, vec3(1.0 / gamma));

        //mapped = vec3(0);
    }

    mapped = colorRemap(mapped, 16.0f);

    // crt phosphor layout
    if ( ( int(texCoords.y * screenHeight) % scanlineSize ) == 0 && enableScanLines ) { mapped = vec3(0); }

    //FragColor = vec4(mapped, 1.f);
    FragColor = vec4(mapped, 1.f);
}