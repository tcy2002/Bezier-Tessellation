#version 450

layout (location = 1) in vec2 aPosition;

uniform float aspect;

void main() {
    vec2 position = aPosition;
    if (aspect > 1.0f)
        position.x /= aspect;
    else
        position.y *= aspect;
    gl_Position = vec4(position, 0.0, 1.0);
}