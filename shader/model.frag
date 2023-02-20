#version 450

in GS_OUT {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} fs_in;

out vec4 FragColor;

uniform int gUseTex;
uniform sampler2D gSampler;

vec4 gRawColor = vec4(0.44f, 0.57f, 0.75f, 1.0f);
vec3 gEyePosition = vec3(0.0f, 0.0f, -3.0f);
vec3 gPointLightColor = vec3(1.0f, 1.0f, 1.0f);
vec3 gPointLightPosition = vec3(-0.3f, 0.7f, -3.0f);
float gAmbientLight = 0.6f;
float gDiffuseIntensity = 0.8f;
float gSpecularIntensity = 0.8f;
float gSpecularPower = 8.0f;
float gConstant = 1.2f;
float gLinear = 0.6f;
float gExp = 0.2f;

vec4 calcAmbientLight(float light) {
    return vec4(vec3(gAmbientLight), 1.0f);
}

vec4 calcDiffuseLight(vec3 color, vec3 direction, float intensity) {
    float factor = dot(normalize(fs_in.normal), normalize(-direction));
    if (factor > 0) {
        return vec4(color * intensity * factor, 1.0f);
    } else {
        return vec4(vec3(0.0f), 1.0f);
    }
}

vec4 calcSpecularLight(vec3 color, vec3 direction, vec3 eyePosition, float intensity, float power) {
    float sign = dot(fs_in.normal, -direction);
    if (sign > 0) {
        vec3 vertexToEye = normalize(eyePosition - fs_in.position);
        vec3 reflection = normalize(reflect(direction, fs_in.normal));
        float factor = dot(vertexToEye, reflection);
        if (factor > 0) {
            factor = pow(factor, power);
            return vec4(color * intensity * factor, 1.0f);
        } else {
            return vec4(vec3(0.0f), 1.0f);
        }
    }
    return vec4(vec3(0.0f), 1.0f);
}

vec4 calcPointLight(vec3 color, vec3 position) {
    vec3 lightDirection = fs_in.position - position;
    float distance = length(lightDirection);
    vec4 ambientLight = calcAmbientLight(gAmbientLight);
    vec4 diffuseLignt = calcDiffuseLight(color, lightDirection, gDiffuseIntensity);
    vec4 specularLight = calcSpecularLight(color, lightDirection, gEyePosition, gSpecularIntensity, gSpecularPower);
    float attenutation = gConstant + gLinear * distance + gExp * distance * distance;
    return ambientLight + (diffuseLignt + specularLight) / attenutation;
}

void main() {
    vec4 light = calcPointLight(gPointLightColor, gPointLightPosition);
    if (gUseTex == 1) {
        FragColor = vec4(texture2D(gSampler, fs_in.texCoord.xy) * light);
    } else {
        FragColor = vec4(gRawColor * light);
    }
}