// STEP 3 - Passing parameters from application to vertex program
// Copyright (c) 2015-23 by Carlos Martinho

#version 330 core

uniform mat4 ModelMatrix;
uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;	
};
uniform float ModelScale;

in vec3 inPosition;

void main(void)
{
	vec4 mcPosition = vec4(inPosition, 1.0);
	mcPosition *= vec4(ModelScale, 0.0, 1.0, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * mcPosition;
}
