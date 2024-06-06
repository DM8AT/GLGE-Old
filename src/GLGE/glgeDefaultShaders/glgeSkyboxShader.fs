#version 450 core
precision mediump float;
uniform samplerCube glgeSkybox;

in vec3 texCoords;
layout(location = 0) out vec4 Albedo;
layout(location = 1) out vec4 Normal;
layout(location = 2) out vec4 Position;
layout(location = 3) out vec4 Roughness;
layout(location = 6) out vec4 Lit;

void main()
{
    Albedo = texture(glgeSkybox, texCoords);
    Normal = vec4(0,0,0,1);
    Position = vec4(0,0,0,1);
    Roughness = vec4(0,0,0,1);
    Lit = Albedo;
    gl_FragDepth = -1.f;
}