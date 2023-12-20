#version 330 core

in vec3 exNormal;
in vec3 exColor;
in float exAlpha;

in vec3 FragPos;
in vec3 exViewPos;

uniform sampler2D inTex;
in vec2 exTexcoord;


out vec4 FragmentColor;



vec3 lightPosition = vec3(1.0f,10.0f,5.0f);
vec3 lightColor = vec3(1.0f,1.0f,1.0f);
vec3 ambient = vec3(0.1f,0.1f,0.3f);
float specularStrength = 0.4;


void main(void)
{
	vec3 N = normalize(exNormal);
	vec3 C = normalize(exColor);
	
	vec3 color; //Should be Perlin Noise for marble

	vec3 Lpos = normalize(lightPosition - FragPos); 
	float diff = max(dot(N, Lpos), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(exViewPos - FragPos);
	vec3 reflectDir = reflect(-Lpos, N);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  


	float noise = texture(inTex, exTexcoord).r;
	float k = cos(exTexcoord.x+noise)*0.5+0.5;
	color = mix(vec3(1.0f,0.0f,0.0f),exColor, k);


	vec3 result = (ambient + diffuse + specular) * color;
	FragmentColor = vec4(result, exAlpha);
	//FragmentColor = vec4(exTexcoord,0.0, exAlpha);


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