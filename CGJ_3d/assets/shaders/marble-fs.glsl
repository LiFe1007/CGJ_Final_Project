#version 330 core

in vec3 exNormal;
in vec3 exColor;
in float exAlpha;

in vec3 FragPos;
in vec3 exViewPos;

uniform sampler2D inTex;
in vec2 exTexcoord;


out vec4 FragmentColor;





void main(void)
{
	vec3 lightPosition = vec3(1.0f,10.0f,5.0f);
	vec3 lightColor = vec3(1.0f,1.0f,1.0f);
	vec3 ambient = vec3(0.1f,0.1f,0.3f);
	float specularStrength = 1.5;


	vec3 N = normalize(exNormal);

	vec3 Lpos = normalize(lightPosition - FragPos); 
	float diff = max(dot(N, Lpos), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(exViewPos - FragPos);
	vec3 reflectDir = reflect(-Lpos, N);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  


	float noise = texture(inTex, exTexcoord).r;
	float k = cos(exTexcoord.x+noise)*0.5+0.5;
	vec3 color = mix(vec3(0.0f,0.0f,0.0f),exColor, k);


	vec3 result = (ambient + diffuse + specular) * color;
	FragmentColor = vec4(result, exAlpha);
}
