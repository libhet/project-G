#version 330 core

out vec4 color;

in vec3 ourColor;

uniform vec4 ourColor2;

void main()
{
   color = vec4(ourColor.r * ourColor2.r, ourColor.g * ourColor2.g, ourColor.b * ourColor2.b, 1.0);
}