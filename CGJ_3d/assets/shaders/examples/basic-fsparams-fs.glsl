#version 330 core

uniform vec4 Color;
out vec4 FragmentColor;

void main(void)
{
	FragmentColor = Color;
}
