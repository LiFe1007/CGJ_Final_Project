#version 330 core

in vec3 exPosition;
in vec3 exNormal;
out vec4 FragmentColor;

uniform float Time0_1;
const float PI = 3.14159265358979323846264;

void main(void)
{
	vec3 N = normalize(exNormal);

	vec3 pos, neg;
	pos.x = (N.x > 0.0) ? N.x : 0.0;
	neg.x = (N.x < 0.0) ? -N.x : 0.0;
	pos.y = (N.y > 0.0) ? N.y : 0.0;
	neg.y = (N.y < 0.0) ? -N.y : 0.0;
	pos.z = (N.z > 0.0) ? N.z : 0.0;
	neg.z = (N.z < 0.0) ? -N.z : 0.0;

	vec3 color;
	color.r = pos.x + neg.y + neg.z;
	color.g = pos.y + neg.x + neg.z;
	color.b = pos.z + neg.x + neg.y;

	float k = sin(Time0_1 * 2 * PI);
	color *= 1 - 0.5 * k;

	FragmentColor = vec4(color, 1.0);
}
