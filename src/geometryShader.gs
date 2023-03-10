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
    //calculate the edge vectors
    vec3 edge0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 edge1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;

    // Lengths of UV differences
    vec2 deltaUV0 = data_in[1].texCoord - data_in[0].texCoord;
    vec2 deltaUV1 = data_in[2].texCoord - data_in[0].texCoord;

    // one over the determinant
    float invDet = 1.0f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

    vec3 tangent = vec3(invDet * (deltaUV1.y * edge0 - deltaUV0.y * edge1));
    vec3 bitangent = vec3(invDet * (-deltaUV1.x * edge0 + deltaUV0.x * edge1));

    vec3 T = normalize(vec3(data_in[0].modelMat * vec4(tangent, 0.0f)));
    vec3 B = normalize(vec3(data_in[0].modelMat * vec4(bitangent, 0.0f)));
    vec3 N = normalize(vec3(data_in[0].modelMat * vec4(cross(edge1, edge0), 0.0f)));

    mat3 tbn = mat3(T, B, N);
    // TBN is an orthogonal matrix and so its inverse is equal to its transpose
    tbn = transpose(tbn);

    //output the TBN matrix
    TBN = tbn;

    for (int i = 0; i < 3; i++)
    {
        gl_Position = gl_in[i].gl_Position * data_in[i].projection;

        normal = cross(edge0, edge1);

        color = data_in[i].color;
        texCoord = data_in[i].texCoord;
        currentPos = data_in[i].currentPosition;
        EmitVertex();
    }

    EndPrimitive();
}