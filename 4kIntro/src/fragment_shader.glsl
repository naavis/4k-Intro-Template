#version 430

layout (location=0) uniform vec4 fpar[4];
layout (location=0) out vec4 color;
in vec2 p;


void main()
{
	color = vec4(0.5 * p.x + 0.5, 0.5 * p.y + 0.5, 0.0, 1.0);
}