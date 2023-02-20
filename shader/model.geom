#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in TES_OUT {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} gs_in[];

out GS_OUT {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} gs_out;

uniform mat4 gProj;

void main() {
    mat4 gProj_T = transpose(gProj);

    gl_Position = gProj_T * (vec4(gs_in[0].position, 1.0f));
    gs_out.position = gs_in[0].position;
    gs_out.normal = gs_in[0].normal;
    gs_out.texCoord = gs_in[0].texCoord;
    EmitVertex();

    gl_Position = gProj_T * (vec4(gs_in[1].position, 1.0f));
    gs_out.position = gs_in[1].position;
    gs_out.normal = gs_in[1].normal;
    gs_out.texCoord = gs_in[1].texCoord;
    EmitVertex();

    gl_Position = gProj_T * (vec4(gs_in[2].position, 1.0f));
    gs_out.position = gs_in[2].position;
    gs_out.normal = gs_in[2].normal;
    gs_out.texCoord = gs_in[2].texCoord;
    EmitVertex();

    EndPrimitive();
}