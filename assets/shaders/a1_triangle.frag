#version 330
in vec3 color;
out vec4 fragColor;

uniform vec3 u_color;

void main()
{
    fragColor = vec4(color * 1.0);
}
