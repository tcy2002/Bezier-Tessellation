#version 450

layout(quads, equal_spacing, ccw) in;

in TCS_OUT {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} tes_in[];

out TES_OUT {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} tes_out;

vec3 calcVertex2(vec3 v1, vec3 n1, vec3 v2, vec3 n2) {
    if (length(n1 - n2) < 1e-6 || dot(v2 - v1, n1) * dot(v2 - v1, n2) > -1e-6) {
        return (v1 + v2) / 2.0f;
    }
    vec3 avg = (n1 + n2) / 2.0f;
    mat3 mat_left = transpose(mat3(n1, n2, cross(v2 - v1, avg)));
    vec3 vec_right = vec3(dot(v1, n1), dot(v2, n2), dot(cross(v1, v2), avg));
    return inverse(mat_left) * vec_right;
}

vec3 calcVertex3(vec3 v1, vec3 n1, vec3 v2, vec3 n2, vec3 v3, vec3 n3) {
    if (length(n2 - n3) < 1e-6 || dot(v3 - v2, n2) * dot(v3 - v2, n3) > -1e-6) {
        return calcVertex2(v1, n1, v2, n2) + (v3 - v2) / 2.0f;
    }
    mat3 mat_left = mat3(n1, n2, n3);
    vec3 vec_right = vec3(dot(v1, n1), dot(v2, n2), dot(v3, n3));
    return vec_right * inverse(mat_left);
}

vec3 calcBezierSample3(vec3 v1, vec3 v2, vec3 v3, float t) {
    float rt = 1.0f - t;
    vec3 vert1 = t * v1 + rt * v2;
    vec3 vert2 = t * v2 + rt * v3;
    return t * vert1 + rt * vert2;
}

void main() {
    float u = gl_TessCoord.x;
    float ru = 1 - u;
    float v = gl_TessCoord.y;
    float rv = 1 - v;

    tes_out.normal =
    u * v * tes_in[0].normal +
    ru * v * tes_in[1].normal +
    ru * rv * tes_in[2].normal +
    u * rv * tes_in[3].normal;

    tes_out.texCoord =
    u * v * tes_in[0].texCoord +
    ru * v * tes_in[1].texCoord +
    ru * rv * tes_in[2].texCoord +
    u * rv * tes_in[3].texCoord;

//    tes_out.position =
//    u * v * tes_in[0].position +
//    ru * v * tes_in[1].position +
//    ru * rv * tes_in[2].position +
//    u * rv * tes_in[3].position;

    vec3[3][3] vertices;

    vertices[0][0] = tes_in[3].position;
    vertices[0][2] = tes_in[2].position;
    vertices[2][0] = tes_in[0].position;
    vertices[2][2] = tes_in[1].position;

    vertices[2][1] = calcVertex2(tes_in[0].position, tes_in[0].normal, tes_in[1].position, tes_in[1].normal);
    vertices[1][2] = calcVertex2(tes_in[1].position, tes_in[1].normal, tes_in[2].position, tes_in[2].normal);
    vertices[0][1] = calcVertex2(tes_in[2].position, tes_in[2].normal, tes_in[3].position, tes_in[3].normal);
    vertices[1][0] = calcVertex2(tes_in[3].position, tes_in[3].normal, tes_in[0].position, tes_in[0].normal);

    vertices[1][1] = calcVertex3(tes_in[2].position, tes_in[2].normal, tes_in[3].position, tes_in[3].normal, tes_in[0].position, tes_in[0].normal);

    vec3 vert1 = calcBezierSample3(vertices[0][0], vertices[0][1], vertices[0][2], u);
    vec3 vert2 = calcBezierSample3(vertices[1][0], vertices[1][1], vertices[1][2], u);
    vec3 vert3 = calcBezierSample3(vertices[2][0], vertices[2][1], vertices[2][2], u);
    tes_out.position = calcBezierSample3(vert3, vert2, vert1, v);
}