#version 450

layout (location = 1) in vec3 aPosition;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord;

out VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} vs_out;

uniform mat4 gWorld;
uniform mat4 gRot;

void main() {
    mat4 gWorld_T = transpose(gWorld);
    mat4 gRot_T = transpose(gRot);
    vs_out.position = vec3((gWorld_T * vec4(aPosition, 1.0f)).xyz);
    vs_out.normal = vec3(normalize((gRot_T * vec4(aNormal, 1.0f)).xyz));
    vs_out.texCoord = aTexCoord;
}