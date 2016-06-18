#version 430

layout (location=0) out vec4 color;
uniform sampler2D inputTexture;
in vec2 p;

void main()
{
	vec2 coords = 0.5 * p + 0.5;
	color = texture(inputTexture, coords);
}