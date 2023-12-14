#version 300 es

precision mediump float;

layout(location = 4) out vec4 FragColor;

uniform sampler2D glgeTranspAccumTexture;
uniform sampler2D glgeTranspCountTexture;

void main()
{
    vec4 accum = texelFetch(glgeTranspAccumTexture, ivec2(gl_FragCoord.xy), 0);
    float n = max(1.0, texelFetch(glgeTranspCountTexture, ivec2(gl_FragCoord.xy), 0).b);
    FragColor = vec4(accum.rgb / max(accum.a, 0.0001), pow(max(0.0, 1.0 - accum.a / n), n));
}