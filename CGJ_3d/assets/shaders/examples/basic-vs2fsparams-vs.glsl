// STEP 6 - "Passing" parameters from vertex program to fragment program
// Copyright (c) 2015-23 by Carlos Martinho

#version 330 core

uniform mat4 ModelMatrix;
uniform Camera {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;	
};

in vec3 inPosition;

out vec4 mcPosition;
out vec4 ccPosition;

void main(void)
{	
	mcPosition = vec4(inPosition, 1.0);
	ccPosition = ProjectionMatrix * ViewMatrix * ModelMatrix * mcPosition;
	gl_Position = ccPosition;
}
