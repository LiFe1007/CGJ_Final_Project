#version 330 core

uniform mat4 ModelMatrix;


uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

in vec3 inPosition;
in vec3 inNormal;
uniform vec3 inColor;
out vec3 exNormal;
out vec3 exColor;
out vec3 FragPos;
out vec3 exViewPos;

void main(void)
{
	exNormal = inNormal;
	exColor = inColor;
	FragPos = vec3(ModelMatrix * vec4(inPosition, 1.0));
	exViewPos = vec3(inverse(ViewMatrix) * vec4(0.0f, 7.0f, 0.0f, 1.0f));


	vec4 mcPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * mcPosition;
}
