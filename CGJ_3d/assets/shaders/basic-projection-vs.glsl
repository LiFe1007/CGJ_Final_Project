// STEP 2 - Projection
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
  vec4 mcPosition = vec4(inPosition.x, 0.0, inPosition.z, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * mcPosition;
}

//	Alternative using a scale matrix
//	mat4 S = mat4(1.0); // Identity Matrix
//	S[1][1] = 0.0;
//	mcPosition = S * mcPosition;
