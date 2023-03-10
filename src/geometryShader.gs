#version 300 es

#extension GL_EXT_geometry_shader : enable
#extension GL_OES_geometry_shader : enable

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 normal;
out vec4 color;
out vec2 texCoord;
out vec3 currentPos;
out mat3 TBN;

in DATA{
    vec3 normal;
    vec4 color;
    vec2 texCoord;
    vec3 currentPosition;
    mat4 projection;
    mat4 modelMat; 
} data_in[];

void main()
{
    for (int i = 0; i < 3; i++)
    {
        gl_Position = gl_in[i].gl_Position * data_in[i].projection;

        normal = data_in[i].normal;

        color = data_in[i].color;
        texCoord = data_in[i].texCoord;
        currentPos = data_in[i].currentPosition;
        EmitVertex();
    }

    EndPrimitive();
}