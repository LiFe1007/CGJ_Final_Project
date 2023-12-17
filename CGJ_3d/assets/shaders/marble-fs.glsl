#version 330 core

in vec3 exNormal;
in vec3 exColor;
in vec3 FragPos;
in vec3 exViewPos;
out vec4 FragmentColor;
//uniform vec3 lightPosition;
//uniform sampler2D;

vec3 lightPosition = vec3(1.0f,10.0f,5.0f);
vec3 lightColor = vec3(1.0f,1.0f,1.0f);
vec3 ambient = vec3(0.1f,0.1f,0.3f);
float specularStrength = 0.4;

vec3 Color(vec3 C, vec3 N);

void main(void)
{
	vec3 N = normalize(exNormal);
	vec3 C = normalize(exColor);
	
	vec3 color = Color(C, N); //Should be Perlin Noise for marble

	vec3 Lpos = normalize(lightPosition - FragPos); 
	float diff = max(dot(N, Lpos), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(exViewPos - FragPos);
	vec3 reflectDir = reflect(-Lpos, N);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  

	vec3 result = (ambient + diffuse + specular) * color;
	FragmentColor = vec4(result, 1.0);
}

vec3 Color(vec3 C, vec3 N)
{
	vec3 c = (C + vec3(1.0)) *0.5;
	vec3 pos, neg;
	pos.x = (N.x > 0.0) ? -N.x : 0.0;
	neg.x = (N.x < 0.0) ? N.x : 0.0;
	pos.y = (N.y > 0.0) ? -N.y : 0.0;
	neg.y = (N.y < 0.0) ? N.y : 0.0;
	pos.z = (N.z > 0.0) ? -N.z : 0.0;
	neg.z = (N.z < 0.0) ? N.z : 0.0;

	c.r = C.x - (pos.x + neg.y + neg.z)*0.2f;
	c.g = C.y - (pos.y + neg.x + neg.z)*0.2f;
	c.b = C.z - (pos.z + neg.x + neg.y)*0.2f;
	return c;
}

/** / 
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