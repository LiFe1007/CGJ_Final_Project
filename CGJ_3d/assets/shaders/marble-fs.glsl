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

/** / 
void Texture2D::PerlinNoise(int 256,5,5,2,2,8) { 
PerlinNoise(256,5,5,2,2,8)

shader fs 
uniform sampler2d 
noisetexture
uniform light position 
specularcomponent 
k_diffuse=Levers[0] 
V position 
N normalize(normal) 
Lpos(viewmatrix*vvec4()lightoosition,1.0) 
L normalzize(L_pos-V) 
R relfect(-l,n) 
E=normalize(-v) 

final=base*diffuse 

intensity 
wposition 
noise=texture
k=cos(v.x+noise(v))*0.5+0.5 
mix(marble_color,vein_color, k) )
/**/