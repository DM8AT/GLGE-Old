#version 300 es
#extension GL_OES_shader_io_blocks : enable

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 normal;

uniform mat4 camMat;
uniform mat4 modelMat;
uniform mat4 rotMat;

out DATA{
    vec3 normal;
    vec4 color;
    vec2 texCoord;
    vec3 currentPosition;
    mat4 projection;
    mat4 modelMat; 
} dataOut;

void main()
{
    dataOut.color = color;
    dataOut.texCoord = texCoord;
    if (rotMat == mat4(0))
    {
        dataOut.normal = normal;
    }
    else
    {
        dataOut.normal = normalize(vec3(vec4(normal,1)*rotMat));
    }
    dataOut.projection = camMat;
    dataOut.modelMat = modelMat;
    dataOut.currentPosition = vec3(vec4(pos, 1)*modelMat);
    
    gl_Position = vec4(pos, 1.f) * modelMat;
}