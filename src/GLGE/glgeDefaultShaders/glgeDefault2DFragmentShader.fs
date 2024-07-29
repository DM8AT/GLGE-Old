#version 450 core

precision mediump float;

out vec4 FragColor;
in vec4 fColor;
in vec2 fTexCoord;

uniform sampler2D Texture;

void main()
{
    vec4 col = texture(Texture, fTexCoord);

    col.rgb += fColor.rgb;
    col.w *= fColor.w;

    FragColor = col;
}