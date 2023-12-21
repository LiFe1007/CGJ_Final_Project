// STEP 1 - Project setting, minimum shaders, and matrices
// Copyright (c) 2015-2023 by Carlos Martinho

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

// OLD SYNTAX (GLSL 120)
// gl_Position = ftransform();
// gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
// gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
