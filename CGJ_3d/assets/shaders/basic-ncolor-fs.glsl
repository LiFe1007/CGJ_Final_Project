#version 330 core

in vec3 exNormal;
in vec3 exColor;
out vec4 FragmentColor;

void main(void)
{
	vec3 N = normalize(exNormal);
	vec3 C = normalize(exColor);

	vec3 color;
/**/
	color = (C + vec3(1.0)) *0.5;
/**/
	vec3 pos, neg;
	pos.x = (N.x > 0.0) ? -N.x : 0.0;
	neg.x = (N.x < 0.0) ? N.x : 0.0;
	pos.y = (N.y > 0.0) ? -N.y : 0.0;
	neg.y = (N.y < 0.0) ? N.y : 0.0;
	pos.z = (N.z > 0.0) ? -N.z : 0.0;
	neg.z = (N.z < 0.0) ? N.z : 0.0;

	color.r = C.x - (pos.x + neg.y + neg.z)*0.2f;
	color.g = C.y - (pos.y + neg.x + neg.z)*0.2f;
	color.b = C.z - (pos.z + neg.x + neg.y)*0.2f;
/**/
	FragmentColor = vec4(color, 1.0);
}
