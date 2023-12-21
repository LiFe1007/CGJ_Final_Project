// STEP 5 - Passing parameters from application to fragment program
// Copyright (c) 2015-23 by Carlos Martinho

#version 330 core

uniform mat4 ModelMatrix;
uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;	
};

in vec3 inPosition;

void main(void)
{
	vec4 mcPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * mcPosition;
}
