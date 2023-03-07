#version 300 es

in DATA {
    vec3 pos;
    vec4 color;
    vec2 texCoord;
    vec3 normal;
} dataInp[];

in vec3 pos;
in vec4 vColor;
in vec2 vTexCoord;
in vec3 vNormal;

uniform mat4 moveMat;
uniform mat4 modelMat;

out vec4 color;
out vec2 texCoord;
out vec3 normal;
out vec3 currentPosition;

void main()
{
    color = dataInp[0].color;
    texCoord = dataInp[0].texCoord;
    normal = dataInp[0].normal;

    currentPosition = vec3(vec4(dataInp[0].pos, 1)*modelMat);
    gl_Position = vec4(dataInp[0].pos, 1)*moveMat;
}