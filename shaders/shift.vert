#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ourColor;

uniform vec2 Shift;

void main() {
    float x = position.x + Shift.x;
    gl_Position = vec4(x, position.y, position.z, 1.0);
    ourColor = vec3(position.x, position.y, position.z);
};