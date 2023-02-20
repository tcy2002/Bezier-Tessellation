#version 450

layout (vertices = 4) out;

in VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} tcs_in[];

out TCS_OUT {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} tcs_out[];

vec3 gEyePosition = vec3(0.0f, 0.0f, -3.0f);

float calcDistance(vec3 a, vec3 b) {
    vec3 diff = a - b;
    return sqrt(dot(diff, diff));
}

void main() {
    float dist1 = calcDistance(tcs_in[0].position, gEyePosition);
    float dist2 = calcDistance(tcs_in[1].position, gEyePosition);
    float dist3 = calcDistance(tcs_in[2].position, gEyePosition);
    float dist4 = calcDistance(tcs_in[3].position, gEyePosition);

    float n_dist1 = (dist1 + dist2) / 2.0f;
    float n_dist2 = (dist2 + dist3) / 2.0f;
    float n_dist3 = (dist3 + dist4) / 2.0f;
    float n_dist4 = (dist4 + dist1) / 2.0f;

    float n_total = (n_dist1 + n_dist3) / 2.0f;

//    gl_TessLevelInner[0] = 1.0f;
//    gl_TessLevelInner[1] = 1.0f;
//
//    gl_TessLevelOuter[0] = 1.0f;
//    gl_TessLevelOuter[1] = 1.0f;
//    gl_TessLevelOuter[2] = 1.0f;
//    gl_TessLevelOuter[3] = 1.0f;

    gl_TessLevelInner[0] = max(3.0f - n_total / 3.0f, 1.0f);
    gl_TessLevelInner[1] = max(3.0f - n_total / 3.0f, 1.0f);

    gl_TessLevelOuter[0] = max(3.0f - n_dist2 / 3.0f, 1.0f);
    gl_TessLevelOuter[1] = max(3.0f - n_dist3 / 3.0f, 1.0f);
    gl_TessLevelOuter[2] = max(3.0f - n_dist4 / 3.0f, 1.0f);
    gl_TessLevelOuter[3] = max(3.0f - n_dist1 / 3.0f, 1.0f);

    tcs_out[gl_InvocationID].position = tcs_in[gl_InvocationID].position;
    tcs_out[gl_InvocationID].normal = tcs_in[gl_InvocationID].normal;
    tcs_out[gl_InvocationID].texCoord = tcs_in[gl_InvocationID].texCoord;
}