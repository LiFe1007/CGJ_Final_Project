#version 330 core

in vec4 mcPosition;
in vec4 ccPosition;

out vec4 FragmentColor;

void main(void)
{
/**/
	vec3 color = (mcPosition.xyz / mcPosition.w  + vec3(1.0)) * 0.5;
/** /
	vec3 color = (ccPosition.xyz / ccPosition.w + vec3(1.0))* 0.5;
/**/
	FragmentColor = vec4(color, 1.0);
}
