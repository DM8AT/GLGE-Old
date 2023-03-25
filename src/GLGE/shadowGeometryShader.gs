#version 300 es

#extension GL_EXT_geometry_shader : enable
#extension GL_OES_geometry_shader : enable

precision highp float;

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos;

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        // Sets the face we are currently working on
        gl_Layer = face;
        for(int i = 0; i < 3; i++)
        {
            // Make transformed vertex
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}