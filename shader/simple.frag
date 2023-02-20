#version 450

out vec4 FragColor;

uniform vec4 myColor;

void main() {
    FragColor = myColor;
}