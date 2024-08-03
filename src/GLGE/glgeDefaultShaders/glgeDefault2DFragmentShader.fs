#version 450 core

precision mediump float;

layout(location = 0) out vec4 Albedo;
layout(location = 1) out vec4 Normal;
layout(location = 2) out vec4 Position;
layout(location = 3) out vec4 Roughness;
layout(location = 5) out vec4 DepthAndAlpha;

in vec4 fColor;
in vec2 fTexCoord;

uniform sampler2D Texture;

void main()
{
    vec4 col = texture(Texture, fTexCoord);

    col.rgb += fColor.rgb;
    col.w *= fColor.w;

    Albedo = col;
    Normal = vec4(0,0,0,1);
    Position = vec4(0,0,0,1);
    Roughness = vec4(0,0,0,1);
    DepthAndAlpha = vec4(0,0,0,1);
}